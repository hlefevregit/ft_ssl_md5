/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:33:50 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/29 11:23:16 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"
#include "../../includes/ft_types.h"

char *md5_of_buffer(const void *data, size_t len) {
	t_context ctx;
	ft_memset(&ctx, 0, sizeof(t_context));

	// copier le buffer dans une string null-terminated temporaire
	char *tmp = malloc(len + 1);
	if (!tmp) return NULL;
	memcpy(tmp, data, len);
	tmp[len] = '\0';

	t_input input = { .type = INPUT_STRING, .data = tmp, .next = NULL };
	ctx.inputs = &input;
	ctx.type = "md5";

	char *digest_str = md5_hash(&ctx);  // renvoie une string hexadécimale

	free(tmp);
	return digest_str;
}

static void	derive_key_iv_from_password(t_context *ctx)
{
	unsigned char	buffer[MD5_DIGEST_LENGTH * 2];
	unsigned char	*password = (unsigned char *)ctx->des_flags.password;
	unsigned char	salt[8];
	
	if (ctx->des_flags.salt_hex) {
		if (hexstr_to_bytes(ctx->des_flags.salt_hex, salt, 8) != 0) {
			write(2, "ft_ssl des: invalid salt format\n", 32);
			return;
		}
	} else {
		for (int i = 0; i < 8; i++)
			salt[i] = rand() % 256;

		memcpy(ctx->des_flags.salt, salt, 8);
		
	}

	uint8_t md5_input[512];
	int pass_len = ft_strlen((char *)password);
	
	ft_memcpy(md5_input, password, pass_len);
	ft_memcpy(md5_input + pass_len, salt, 8);
	
	unsigned char digest1[MD5_DIGEST_LENGTH];
	md5_hash_buffer(md5_input, pass_len + 8, digest1);
	
	ft_memcpy(ctx->des_flags.key, digest1, 8);
	
	ft_memcpy(md5_input, digest1, MD5_DIGEST_LENGTH);
	ft_memcpy(md5_input + MD5_DIGEST_LENGTH, password, pass_len);
	ft_memcpy(md5_input + MD5_DIGEST_LENGTH + pass_len, salt, 8);
	
	unsigned char digest2[MD5_DIGEST_LENGTH];
	md5_hash_buffer(md5_input, MD5_DIGEST_LENGTH + pass_len + 8, digest2);

	ft_memcpy(ctx->des_flags.iv, digest2, 8);
}


int	prepare_des_crypt_params(t_context *ctx)
{
	if (ctx->des_flags.key_hex) {
		if (hexstr_to_bytes(ctx->des_flags.key_hex, ctx->des_flags.key, 8) != 0) {
			write(2, "ft_ssl des: invalid key hex string\n", 35);
			return 1;
		}
	} else if (ctx->des_flags.password) {
		derive_key_iv_from_password(ctx);
	
	} else if (!ctx->des_flags.password) {
		if (ctx->des_flags.print_key) {
			char *pwd1 = getpass("Enter password: ");
			char *pwd2 = getpass("Verifying - Enter password: ");

			if (!pwd1 || !pwd2 || ft_strcmp(pwd1, pwd2) != 0) {
				write(2, "ft_ssl des: passwords do not match\n", 35);
				return 1;
			}

			ctx->des_flags.password = ft_strdup(pwd1);

		} else {
			write(2, "ft_ssl des: must provide either key (-k) or password (-p)\n", 58);
			return 1;
		}
	} else {
		write(2, "ft_ssl des: must provide either key (-k) or password (-p)\n", 58);
	}

	if (ctx->des_flags.iv_hex) {
		if (hexstr_to_bytes(ctx->des_flags.iv_hex, ctx->des_flags.iv, 8) != 0) {
			write(2, "ft_ssl des: invalid IV format\n", 30);
			return 1;
		}
	}
	return 0;
}
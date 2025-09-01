/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:33:50 by hulefevr          #+#    #+#             */
/*   Updated: 2025/09/01 16:38:37 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"
#include "../../includes/ft_types.h"



int generate_random_bytes(uint8_t *buf, size_t len)
{
	if (!buf || len == 0)
		return 1;

	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return 1;

	ssize_t read_len = read(fd, buf, len);
	close(fd);

	if (read_len != (ssize_t)len)
		return 1;

	return 0;
}


void print_hex(uint8_t *buf, size_t len)
{
    char hex[3];
    for (size_t i = 0; i < len; ++i) {
        snprintf(hex, sizeof(hex), "%02X", buf[i]);
        write(1, hex, 2);
    }
}

char *md5_hash_buffer(const void *data, size_t len) {
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

int derive_key_iv_from_password(t_context *ctx)
{
	t_des_flags *params = &ctx->des_flags;

	if (params->has_key)
		return 0;

	if (!params->password) {
		write(2, "ft_ssl des: no key or password provided\n", 40);
		return 1;
	}

	// Generate salt if not provided
	if (!params->has_salt) {
		if (generate_random_bytes(params->salt, 8) != 0) {
			write(2, "ft_ssl des: failed to generate salt\n", 36);
			return 1;
		}
		params->has_salt = 1;
	}

	// Concat password + salt
	size_t pwd_len = ft_strlen(params->password);
	unsigned char *concat = malloc(pwd_len + 8);
	if (!concat)
		return 1;
	ft_memcpy(concat, params->password, pwd_len);
	ft_memcpy(concat + pwd_len, params->salt, 8);

	// MD5(password + salt)
	t_context tmp_ctx = {0};
	t_input input = { .type = INPUT_STRING, .data = (char *)concat, .next = NULL };
	tmp_ctx.inputs = &input;
	tmp_ctx.type = "md5";

	char *digest_hex = md5_hash(&tmp_ctx);
	free(concat);

	if (!digest_hex)
		return 1;

	uint8_t digest[16];
	if (hexstr_to_bytes(digest_hex, digest, 16) != 0) {
		free(digest_hex);
		return 1;
	}
	free(digest_hex);

	ft_memcpy(params->key, digest, 8);
	ft_memcpy(params->iv, digest + 8, 8);
	params->has_key = 1;
	params->has_iv = 1;

	return 0;
}

int	prepare_des_crypt_params(t_context *ctx)
{
	if (ctx->des_flags.key_hex) {
		if (hexstr_to_bytes(ctx->des_flags.key_hex, ctx->des_flags.key, 8) != 0) {
			write(2, "ft_ssl des: invalid key hex string\n", 35);
			return 1;
		}
	} else if (ctx->des_flags.password) {
		if (derive_key_iv_from_password(ctx))
			return 1;
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
	if (ctx->des_flags.print_key) {
		write(1, "salt=", 5);
		print_hex(ctx->des_flags.salt, 8);
		write(1, "\nkey=", 5);
		print_hex(ctx->des_flags.key, 8);
		write(1, "\niv=", 4);
		print_hex(ctx->des_flags.iv, 8);
		write(1, "\n", 1);
	}
	return 0;
}
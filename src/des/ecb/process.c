/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:08:16 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/29 17:57:30 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_types.h"
#include "../../../includes/ft_ssl_md5.h"

char	*des_cbc_process(t_context *ctx)
{
	
	if (prepare_des_crypt_params(ctx) != 0)
		return NULL;
	char *input_str = ctx->des_flags.infile ? read_file(ctx->des_flags.infile) : read_stdin();
	
	size_t len = ft_strlen(input_str);
	uint8_t *data = malloc(len);
	if (!data) return NULL;
	ft_memcpy(data, input_str, len);
	free(input_str);

	
	len = pad_data(&data, len, 8);

	uint8_t *output = malloc(len);

	for (size_t i = 0; i < len; i += 8) {
		des_encrypt_block(data + i, output + i, ctx->des_flags.key, ctx->des_flags.iv);
	}

	char *result = NULL;
	if (ctx->des_flags.base64) {
		result = base64_encode((const char *)output);
	} else {
		result = malloc(len + 1);
		ft_memcpy(result, output, len);
		result[len] = '\0';
	}

	if (ctx->des_flags.outfile) {
		write_file(ctx->des_flags.outfile, result);
		
	} else {
		write(1, result, ft_strlen(result));
		write(1, "\n", 1);
	}
	
	free(data);
	free(output);
	if (!ctx->des_flags.outfile) {
		return result;
	}
	free(result);
	return NULL;
}

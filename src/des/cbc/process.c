/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:50:59 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/09/02 17:59:50 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_ssl_md5.h"
#include "../../../includes/ft_types.h"

char *des_cbc_process(t_context *ctx)
{
	if (prepare_des_crypt_params(ctx) != 0)
		return NULL;

	char *input_str = ctx->des_flags.infile ? read_file(ctx->des_flags.infile) : read_stdin();
	if (!input_str) {
		write(2, "ft_ssl des: failed to read input\n", 33);
		return NULL;
	}

	size_t len = 0;

	if (ctx->des_flags.base64 && !ctx->des_flags.encrypt) {
		size_t decoded_len;
		uint8_t *decoded = (uint8_t *)base64_decode(input_str, &decoded_len);
		if (!decoded) {
			write(2, "ft_ssl des: base64 decode error\n", 32);
			return NULL;
		}
		free(input_str);
		input_str = (char *)decoded;
		len = decoded_len;
	} else {
		len = ft_strlen(input_str);
	}

	uint8_t *data = malloc(len);
	if (!data) return NULL;
	ft_memcpy(data, input_str, len);
	free(input_str);

	if (ctx->des_flags.encrypt)
		len = pad_data(&data, len, 8);

	uint8_t *output = malloc(len);
	if (!output) {
		free(data);
		return NULL;
	}

	uint8_t prev_block[8];
	ft_memcpy(prev_block, ctx->des_flags.iv, 8);  // init with IV

	for (size_t i = 0; i < len; i += 8) {
		uint8_t temp[8];

		if (ctx->des_flags.encrypt) {
			xor_blocks(data + i, prev_block, temp);
			des_encrypt_block(temp, output + i, ctx->des_flags.key, ctx->des_flags.iv);
			ft_memcpy(prev_block, output + i, 8);  // update previous block
		} else {
			des_decrypt_block(data + i, temp, ctx->des_flags.key, ctx->des_flags.iv);
			xor_blocks(temp, prev_block, output + i);
			ft_memcpy(prev_block, data + i, 8);  // update previous cipher block
		}
	}

	if (!ctx->des_flags.encrypt)
		len = unpad_data(output, len);

	char *result = NULL;
	if (ctx->des_flags.base64 && ctx->des_flags.encrypt) {
		result = base64_encode((const char *)output, len);
	} else {
		result = malloc(len + 1);
		if (!result) {
			free(data);
			free(output);
			return NULL;
		}
		ft_memcpy(result, output, len);
		result[len] = '\0';
	}

	if (ctx->des_flags.outfile) {
		write_file(ctx->des_flags.outfile, result);
		free(data);
		free(output);
		free(result);
		return NULL;
	}

	free(data);
	free(output);
	return result;
}
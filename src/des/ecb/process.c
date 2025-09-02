/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:08:16 by hulefevr          #+#    #+#             */
/*   Updated: 2025/09/02 17:29:45 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_types.h"
#include "../../../includes/ft_ssl_md5.h"


 char *des_ecb_process(t_context *ctx)
{
	if (prepare_des_crypt_params(ctx) != 0)
		return NULL;

	char *input_str = ctx->des_flags.infile ? read_file(ctx->des_flags.infile) : read_stdin();
	if (!input_str) {
		write(2, "ft_ssl des: failed to read input\n", 33);
		return NULL;
	}

	uint8_t *data = NULL;
	size_t len = 0;
	size_t raw_len = ft_strlen(input_str);
	while (raw_len > 0 && (input_str[raw_len - 1] == '\n' || input_str[raw_len - 1] == '\r'))
		raw_len--;
	input_str[raw_len] = '\0';

	if (ctx->des_flags.base64 && ctx->des_flags.encrypt == 0) {
		// DECRYPT with base64
		if (!(data = (uint8_t *)base64_decode(input_str, &len))) {
			write(2, "ft_ssl des: base64 decode error\n", 32);
			free(input_str);
			return NULL;
		}
		free(input_str);
	} else {
		// No base64 → use input directly
		len = ft_strlen(input_str);
		data = malloc(len);
		if (!data) {
			free(input_str);
			return NULL;
		}
		ft_memcpy(data, input_str, len);
		free(input_str);
	}

	// Padding if encrypting
	if (ctx->des_flags.encrypt == 1)
		len = pad_data(&data, len, 8);

	uint8_t *output = malloc(len);
	if (!output) {
		free(data);
		return NULL;
	}

	// Process block by block
	for (size_t i = 0; i < len; i += 8) {
		if (ctx->des_flags.encrypt == 0) {
			des_decrypt_block(data + i, output + i, ctx->des_flags.key, ctx->des_flags.iv);
		} else {
			des_encrypt_block(data + i, output + i, ctx->des_flags.key, ctx->des_flags.iv);
		}
	}

	if (ctx->des_flags.encrypt == 0)
		len = unpad_data(output, len);

	char *result = NULL;

	if (ctx->des_flags.base64 && ctx->des_flags.encrypt == 1) {
		// ENCRYPT + base64 encode output
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
		free(result);
		result = NULL;
	}

	free(data);
	free(output);

	return result;
}
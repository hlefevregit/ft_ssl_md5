/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   result.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:26:02 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 14:29:07 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"

static const char HEX[] = "0123456789abcdef";

static void encode_uint32_le_hex(uint32_t value, char *output)
{
	for (int i = 0; i < 4; i++) {
		output[i * 2] = HEX[(value >> (i * 8 + 4)) & 0x0F];
		output[i * 2 + 1] = HEX[(value >> (i * 8)) & 0x0F];
	}
}

char	*md5_format_result(uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
	char *result = (char *)malloc(33);
	if (!result)
		return NULL;

	encode_uint32_le_hex(A, result);
	encode_uint32_le_hex(B, result + 8);
	encode_uint32_le_hex(C, result + 16);
	encode_uint32_le_hex(D, result + 24);
	result[32] = '\0';
	return result;
}
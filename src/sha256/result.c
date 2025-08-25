/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   result.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:26:02 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 17:39:10 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"

static const char HEX[] = "0123456789abcdef";

static void	write_uint32_be_hex(uint32_t value, char *output)
{
	for (int i = 0; i < 4; i++) {
		uint8_t byte = (value >> (24 - i * 8)) & 0xFF;
		output[i * 2]	 	= HEX[byte >> 4];
		output[i * 2 + 1] 	= HEX[byte & 0x0F];
	}
}

char	*sha256_format_result(uint32_t H[8])
{
	char *result = malloc(65); // 64 caractères + '\0'
	if (!result)
		return NULL;

	for (int i = 0; i < 8; i++) {
		write_uint32_be_hex(H[i], result + i * 8);
	}
	result[64] = '\0';
	return result;
}
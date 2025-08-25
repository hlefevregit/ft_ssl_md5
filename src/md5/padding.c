/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:31:04 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 13:41:34 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"

uint8_t	*md5_pad(const uint8_t *msg, size_t len, size_t *new_len_out)
{
	size_t padded_len;

	padded_len = ((len + 8) / 64 + 1) * 64; // Passe la len à la prochaine multiple de 64 après avoir ajouté 8 octets

	uint8_t *padded = ft_calloc(padded_len, 1);
	if (!padded)
		return NULL;
	
	ft_memcpy(padded, msg, len);
	padded[len] = 0x80; // Ajoute le bit '1' suivi de '0's

	uint64_t bit_len = (u_int64_t)len * 8;
	ft_memcpy(padded + padded_len - 8, &bit_len, 8); // Ajoute la longueur en bits à la fin

	*new_len_out = padded_len;
	return padded;
}
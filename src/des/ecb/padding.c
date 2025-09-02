/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:12:07 by hulefevr          #+#    #+#             */
/*   Updated: 2025/09/02 15:50:36 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_types.h"
#include "../../../includes/ft_ssl_md5.h"

size_t	pad_data(uint8_t **data, size_t len, size_t block_size)
{
	size_t pad_len = block_size - (len % block_size);
	if (pad_len == 0)
		pad_len = block_size;

	uint8_t *new_data = malloc(len + pad_len);
	if (!new_data)
		return len; // erreur d'allocation, retourner la longueur originale

	ft_memcpy(new_data, *data, len);
	for (size_t i = 0; i < pad_len; i++)
		new_data[len + i] = (uint8_t)pad_len;

	free(*data);
	*data = new_data;
	return len + pad_len;
}

size_t unpad_data(uint8_t *data, size_t len)
{
	if (len == 0)
		return 0;

	uint8_t pad_len = data[len - 1];

	// Vérification du padding (PKCS#7)
	if (pad_len == 0 || pad_len > 8 || pad_len > len)
		return len;

	for (size_t i = 0; i < pad_len; i++) {
		if (data[len - 1 - i] != pad_len)
			return len;  // padding invalide, on retourne les données complètes
	}

	return len - pad_len;
}
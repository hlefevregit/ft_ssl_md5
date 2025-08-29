/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:12:07 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/29 16:29:57 by hulefevr         ###   ########.fr       */
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
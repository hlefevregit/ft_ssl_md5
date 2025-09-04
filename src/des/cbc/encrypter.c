/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:56:02 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/09/02 17:59:35 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_ssl_md5.h"
#include "../../../includes/ft_types.h"

void des_encrypt_block_cbc(const uint8_t *in, uint8_t *out,
						   uint8_t key[8], uint8_t iv[8],
						   uint8_t prev_block[8])
{
	uint8_t temp[8];
	xor_blocks(in, prev_block, temp);
	des_encrypt_block(temp, out, key, iv);
}

void des_decrypt_block_cbc(const uint8_t *in, uint8_t *out,
						   uint8_t key[8], uint8_t iv[8],
						   const uint8_t prev_block[8])
{
	uint8_t decrypted_block[8];

	// Déchiffrement standard (comme en ECB)
	des_decrypt_block((uint8_t *)in, decrypted_block, key, iv);

	// XOR avec le vecteur précédent (IV ou bloc précédent)
	xor_blocks(decrypted_block, prev_block, out);
}
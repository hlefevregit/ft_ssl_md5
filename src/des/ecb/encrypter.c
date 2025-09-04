/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:57:20 by hulefevr          #+#    #+#             */
/*   Updated: 2025/09/02 16:48:00 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_types.h"
#include "../../../includes/ft_ssl_des.h"

// Tableau de permutation initiale (IP)
static const uint8_t IP[64] = {
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};


static const int IP_INV[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9,  49, 17, 57, 25
};

uint64_t initial_permutation(uint64_t input)
{
	uint64_t output = 0;
	for (int i = 0; i < 64; i++)
	{
		output <<= 1;
		output |= (input >> (64 - IP[i])) & 0x01;
	}
	return output;
}

uint64_t final_permutation(uint64_t input)
{
	uint64_t output = 0;
	for (int i = 0; i < 64; i++)
	{
		output <<= 1;
		output |= (input >> (64 - IP_INV[i])) & 0x01;
	}
	return output;
}

void des_encrypt_block(uint8_t in[8], uint8_t out[8], uint8_t key[8], uint8_t iv[8])
{
	(void)iv; // IV is not used in ECB mode
	uint64_t block = 0;

	for (int i = 0; i < 8; i++)
	{
		block <<= 8;
		block |= in[i];
	}

	uint64_t subkeys[16];
	des_key_schedule(key, subkeys);

	block = initial_permutation(block);
	
	uint32_t left = (block >> 32) & 0xFFFFFFFF;
	uint32_t right = block & 0xFFFFFFFF;

	for (int i = 0; i < 16; i++)
	{
		uint32_t tmp = right;
		right = left ^ des_f(right, subkeys[i]);
		left = tmp;
	}
	
	uint64_t preoutput = ((uint64_t)right << 32) | left;

	// Final permutation
	uint64_t ciphertext = final_permutation(preoutput);
	for (int i = 0; i < 8; i++)
	{
		out[7 - i] = ciphertext & 0xFF;
		ciphertext >>= 8;
	}
	
}
void des_decrypt_block(uint8_t in[8], uint8_t out[8], uint8_t key[8], uint8_t iv[8])
{
	(void)iv; // IV not used in ECB mode

	uint64_t block = 0;
	for (int i = 0; i < 8; i++) {
		block <<= 8;
		block |= in[i];
	}

	uint64_t subkeys[16];
	des_key_schedule(key, subkeys);  // generate all 16 keys

	block = initial_permutation(block);

	uint32_t left = (block >> 32) & 0xFFFFFFFF;
	uint32_t right = block & 0xFFFFFFFF;

	for (int i = 15; i >= 0; i--) {
		uint32_t tmp = right;
		right = left ^ des_f(right, subkeys[i]);
		left = tmp;
	}

	// FINAL SWAP 👇 (crucial)
	uint64_t preoutput = ((uint64_t)right << 32) | left;

	uint64_t plaintext = final_permutation(preoutput);
	for (int i = 0; i < 8; i++) {
		out[7 - i] = plaintext & 0xFF;
		plaintext >>= 8;
	}
}
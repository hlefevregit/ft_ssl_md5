/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:57:20 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/29 18:17:22 by hulefevr         ###   ########.fr       */
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

static const int E[48] = {
	32, 1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9,  10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
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

static uint64_t permute(uint64_t input, const int *table, int out_bits, int in_bits)
{
	uint64_t output = 0;
	for (int i = 0; i < out_bits; i++) {
		uint64_t bit = (input >> (in_bits - table[i])) & 1;
		output |= (bit << (out_bits - 1 - i));
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

	uint64_t subkeys[10];
	des_key_schedule(key, subkeys);

	block = initial_permutation(block);
	
	uint32_t left = (block >> 32) & 0xFFFFFFFF;
	uint32_t right = block & 0xFFFFFFFF;

	for (int i = 0; i < 16; i++)
	{
		uint16_t tmp = right;
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
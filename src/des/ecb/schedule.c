/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:11:11 by hulefevr          #+#    #+#             */
/*   Updated: 2025/09/02 15:11:22 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ft_types.h"
#include "../../../includes/ft_ssl_des.h"

// Tableau de permutation PC-1 : 64 bits -> 56 bits
static const int PC1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

// Tableau des décalages à gauche pour chaque round
static const int SHIFTS[16] = {
	1, 1, 2, 2,
	2, 2, 2, 2,
	1, 2, 2, 2,
	2, 2, 2, 1
};

static const int PC2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};




void des_key_schedule(const uint8_t key[8], uint64_t subkeys[16])
{
	uint64_t key64 = 0;
	for (int i = 0; i < 8; i++)
	{
		key64 <<= 8;
		key64 |= key[i];
	}

	uint64_t permuted_key = permute(key64, PC1, 56, 64);

	uint32_t C = (permuted_key >> 28) & 0x0FFFFFFF;
	uint32_t D = permuted_key & 0x0FFFFFFF;

	for (int i = 0; i < 16; i++)
	{
		C = ((C << SHIFTS[i]) | (C >> (28 - SHIFTS[i]))) & 0x0FFFFFFF;
		D = ((D << SHIFTS[i]) | (D >> (28 - SHIFTS[i]))) & 0x0FFFFFFF;
		uint64_t CD = ((uint64_t)C << 28) | D;
		subkeys[i] = permute(CD, PC2, 48, 56);
	}	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:46:16 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 16:36:01 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"

static const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

char *sha256_hash(t_context *ctx)
{
	char *input = ctx->inputs->data;
	size_t len = ft_strlen(input);
	size_t padded_len;
	uint8_t *padded = sha_pad((const uint8_t *)input, len, &padded_len);
	if (!padded)
		return NULL;
	
	uint32_t H[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	
	for (size_t offset = 0; offset < padded_len; offset += 64) {

		const uint8_t *block = padded + offset;
		uint32_t W[64];
		
		for (int i = 0; i < 16; i++) {
			W[i] =	(block[i * 4] << 24) |
					(block[i * 4 + 1] << 16) |
				   	(block[i * 4 + 2] << 8) |
					(block[i * 4 + 3]);
			
		}

		// Extension du message
		for (int i = 16; i < 64; i++) {
			uint32_t s0 = LIL_SIG0(W[i - 15]);
			uint32_t s1 = LIL_SIG1(W[i - 2]);

			W[i] = W[i - 16] + s0 + W[i - 7] + s1;
		}

		uint32_t a = H[0];
		uint32_t b = H[1];
		uint32_t c = H[2];
		uint32_t d = H[3];
		uint32_t e = H[4];
		uint32_t f = H[5];
		uint32_t g = H[6];
		uint32_t h = H[7];

		for (int i = 0; i < 64; i++) {
			uint32_t T1 = h + BIG_SIG1(e) + CH(e, f, g) + K[i] + W[i];
			uint32_t T2 = BIG_SIG0(a) + MAJ(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
			
		}

		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;

	}

	free(padded);
	
    // Formatter les 8 registres H en hex et retourner (comme pour md5_format_result)
	return sha256_format_result(H);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:46:16 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 14:25:21 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_md5.h"

static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    // Round 2
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    // Round 3
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    // Round 4
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t S[64] = {
	// Round 1
	7, 12, 17, 22, 7, 12, 17, 22,
	7, 12, 17, 22, 7, 12, 17, 22,
	// Round 2
	5, 9, 14, 20, 5, 9, 14, 20,
	5, 9, 14, 20, 5, 9, 14, 20,
	// Round 3
	4, 11, 16, 23, 4, 11, 16, 23,
	4, 11, 16, 23, 4, 11, 16, 23,
	// Round 4
	6, 10, 15, 21, 6, 10, 15, 21,
	6, 10, 15, 21, 6, 10, 15, 21
};

char	*md5_hash(const char *input)
{
	size_t len = ft_strlen(input);
	size_t padded_len;
	uint8_t *padded = md5_pad((const uint8_t *)input, len, &padded_len);
	if (!padded)
		return NULL;

	uint32_t A = BLOCK_A;
	uint32_t B = BLOCK_B;
	uint32_t C = BLOCK_C;
	uint32_t D = BLOCK_D;

	for (size_t offset = 0; offset < padded_len; offset += 64) {
		
		const uint8_t *block = padded + offset;

		uint32_t M[16];
		for (int i = 0; i < 16; i++) {
			M[i] = (uint32_t)block[i * 4] |
				   ((uint32_t)block[i * 4 + 1] << 8) |
				   ((uint32_t)block[i * 4 + 2] << 16) |
				   ((uint32_t)block[i * 4 + 3] << 24); 
		}

		uint32_t a = A;
		uint32_t b = B;
		uint32_t c = C;
		uint32_t d = D;

		for (int i = 0; i < 64; i++) {
			uint32_t F_res, g;

			if (i < 16) {
				F_res = F(b, c, d);
				g = i;
			} else if (i < 32) {
				F_res = G(b, c, d);
				g = (5 * i + 1) % 16;
			} else if (i < 48) {
				F_res = H(b, c, d);
				g = (3 * i + 5) % 16;
			} else {
				F_res = I(b, c, d);
				g = (7 * i) % 16;
			}

			uint32_t tmp = d;
			d = c;
			c = b;
			b = b + ROTATE_LEFT((a + F_res + K[i] + M[g]), S[i]);
			a = tmp;
		}

		A += a;
		B += b;
		C += c;
		D += d;
		
	}


	free(padded);

	return md5_format_result(A, B, C, D);
}


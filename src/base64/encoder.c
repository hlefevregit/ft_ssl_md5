/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:07:09 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/28 17:58:16 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_des.h"

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8_t decode_table[256] = {
	['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,  ['F'] = 5,  ['G'] = 6,  ['H'] = 7,
	['I'] = 8,  ['J'] = 9,  ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
	['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19, ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
	['Y'] = 24, ['Z'] = 25,
	['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33,
	['i'] = 34, ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39, ['o'] = 40, ['p'] = 41,
	['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
	['y'] = 50, ['z'] = 51,
	['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
	['8'] = 60, ['9'] = 61,
	['+'] = 62, ['/'] = 63
};

char	*base64_decode(const char *input, size_t *decoded_len)
{
	if (!input)
		return NULL;

	size_t input_len = ft_strlen(input);
	if (input_len % 4 != 0)
		return NULL;

	*decoded_len = (input_len / 4) * 3;

	if (input_len >= 1 && input[input_len - 1] == '=')
		(*decoded_len)--;
	if (input_len >= 2 && input[input_len - 2] == '=')
		(*decoded_len)--;

	char *output = malloc(*decoded_len);
	if (!output)
		return NULL;


	size_t i = 0, j = 0;

	while (i < input_len)
	{
		if (input[i] != '=' && decode_table[(unsigned char)input[i]] == 0 && input[i] != 'A') {
			write(2, "ft_ssl: base64: Invalid base64 input\n", 37);
			free(output);
			return NULL;
		}
		uint32_t sextet_a = input[i] == '=' ? 0 & i++ : decode_table[(unsigned char)input[i++]];
		uint32_t sextet_b = input[i] == '=' ? 0 & i++ : decode_table[(unsigned char)input[i++]];
		uint32_t sextet_c = input[i] == '=' ? 0 & i++ : decode_table[(unsigned char)input[i++]];
		uint32_t sextet_d = input[i] == '=' ? 0 & i++ : decode_table[(unsigned char)input[i++]];

		uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

		if (j < *decoded_len) output[j++] = (triple >> 16) & 0xFF;
		if (j < *decoded_len) output[j++] = (triple >> 8) & 0xFF;
		if (j < *decoded_len) output[j++] = triple & 0xFF;
	}

	output[j] = '\0';
	return output;
}

char	*base64_encode(const char *input)
{
	if (!input)
		return NULL;

	size_t input_len = ft_strlen(input);
	size_t output_len = 4 * ((input_len + 2) / 3);

	char *output = (char *)malloc(output_len + 1);

	if (!output)
		return NULL;

	size_t i = 0, j = 0;
	while (i < input_len)
	{
		uint32_t octet_a = i < input_len ? (unsigned char)input[i++] : 0;
		uint32_t octet_b = i < input_len ? (unsigned char)input[i++] : 0;
		uint32_t octet_c = i < input_len ? (unsigned char)input[i++] : 0;

		uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

		output[j++] = base64_table[(triple >> 18) & 0x3F];
		output[j++] = base64_table[(triple >> 12) & 0x3F];
		output[j++] = (i > input_len + 1) ? '=' : base64_table[(triple >> 6) & 0x3F];
		output[j++] = (i > input_len) ? '=' : base64_table[triple & 0x3F];
		
	}
	output[j] = '\0';
	return output;
}
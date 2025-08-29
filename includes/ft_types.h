/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:21:55 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/29 11:23:56 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_TYPES_H
# define FT_SSL_TYPES_H

#ifndef MD5_DIGEST_LENGTH
# define MD5_DIGEST_LENGTH 16
#endif

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>

typedef struct s_b64_flags {
	int   encode;     // 1 = encode, 0 = decode
	char *infile;
	char *outfile;
}	t_b64_flags;

typedef enum {
	INPUT_STDIN,
	INPUT_STRING,
	INPUT_FILE
}	t_input_type;

typedef struct s_input {
	t_input_type    type;
	char            *data;
	struct s_input  *next;
}	t_input;

typedef struct s_flags {
	int     p;
	int     q;
	int     r;
	int     s;
}	t_flags;

typedef struct s_des_flags {
	const char *infile;
	const char *outfile;
	const char *key_hex;
	const char *salt_hex;
	const char *iv_hex;
	const char *password;

	uint8_t key[8];
	uint8_t iv[8];
	uint8_t salt[8];
	int     has_key;
	int     has_iv;
	int     has_salt;
	int     base64;
	int     encrypt;
	int     print_key;
} t_des_flags;

typedef struct s_context {
	t_flags     flags;
	t_input     *inputs;
	t_b64_flags	b64_flags;
	t_des_flags des_flags;
	char        *type;
	int			ac;
	char		**av;
}	t_context;

/*=============== UTILS ===========*/

int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		hexstr_to_bytes(const char *hex, uint8_t *out, size_t expected_len);

void	print_str(const char *s);
void	write_file(const char *filename, const char *data);
void	free_context(t_context *ctx);
void	ft_memcpy(void *dst, const void *src, size_t n);
void    *ft_memset(void *b, int c, size_t len);
void	*ft_calloc(size_t count, size_t size);

char	*ft_strdup(const char *s);

size_t	ft_strlen(const char *s);


#endif
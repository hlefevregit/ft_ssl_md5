/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:05:09 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 14:28:38 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_MD5
# define FT_SSL_MD5


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

#define BLOCK_A 0x67452301
#define BLOCK_B 0xEFCDAB89
#define BLOCK_C 0x98BADCFE
#define BLOCK_D 0x10325476

#define ROTATE_LEFT(x, s) (((x) << (s)) | ((x) >> (32 - (s))))
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))


/*=============== STRUCTURES ===========*/

typedef enum {
	INPUT_STDIN,
	INPUT_STRING,
	INPUT_FILE
}               t_input_type;

typedef struct s_input {
	t_input_type    type;
	char            *data;
	struct s_input  *next;
}               t_input;

typedef struct s_flags {
	int     p;
	int     q;
	int     r;
	int     s;
}           t_flags;

typedef struct s_context {
	t_flags     flags;
	t_input     *inputs;
	char         *type;
}               t_context;


/*=============== MAIN ===========*/

void	process_input(t_context *ctx);
char	*run_hash(const char *type, const char *input);

/*=============== PARSER ===========*/

int		parse_args(t_context *ctx, int ac, char **av);
void	print_inputs(t_context *ctx);

/*=============== UTILS ===========*/

int		ft_strcmp(const char *s1, const char *s2);
void	print_str(const char *s);
void	*ft_calloc(size_t count, size_t size);
void	free_context(t_context *ctx);
void	ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);

/*=============== READER ===========*/

char	*read_stdin(void);
char	*read_file(const char *filename);

/*================ MD5 =============*/

uint8_t	*md5_pad(const uint8_t *msg, size_t len, size_t *new_len_out);
char	*md5_hash(const char *input);
char	*md5_format_result(uint32_t A, uint32_t B, uint32_t C, uint32_t D);

#endif
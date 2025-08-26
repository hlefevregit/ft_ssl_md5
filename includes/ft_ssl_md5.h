/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:05:09 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 11:20:10 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_MD5
# define FT_SSL_MD5


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

/*=================== MD5 ============*/

#define SHA_BLOCK_A 0x67452301
#define SHA_BLOCK_B 0xEFCDAB89
#define SHA_BLOCK_C 0x98BADCFE
#define SHA_BLOCK_D 0x10325476

#define ROTATE_LEFT(x, s) (((x) << (s)) | ((x) >> (32 - (s))))
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

/*=================== SHA256 ============*/

#define SHA256_BLOCK_A 0x6a09e667
#define SHA256_BLOCK_B 0xbb67ae85
#define SHA256_BLOCK_C 0x3c6ef372
#define SHA256_BLOCK_D 0xa54ff53a
#define SHA256_BLOCK_E 0x510e527f
#define SHA256_BLOCK_F 0x9b05688c
#define SHA256_BLOCK_G 0x1f83d9ab
#define SHA256_BLOCK_H 0x5be0cd19

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define ROTRIGHT(x, n) (((x) >> (n)) | ((x) << (32 - (n)))) 

#define LIL_SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define LIL_SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
#define BIG_SIG0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define BIG_SIG1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))


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

int handle_flag_q(t_context *ctx, char **av, int *i);
int handle_flag_r(t_context *ctx, char **av, int *i);
int handle_flag_p(t_context *ctx, char **av, int *i);
int handle_flag_s(t_context *ctx, char **av, int *i);

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

/*================ SHA256 =============*/

uint8_t	*sha_pad(const uint8_t *msg, size_t len, size_t *padded_len_out);
char	*sha256_hash(const char *input);
char	*sha256_format_result(uint32_t H[8]);


/*==================== FUNCTIONS PTR ======================*/

typedef char *(*t_hash_fn)(const char *input);

typedef struct s_hash_dispatch {
	const char  *name;
	t_hash_fn    func;
}	t_hash_dispatch;

static const t_hash_dispatch g_hashes[] = {
	{ "md5", md5_hash },
	{ "sha256", sha256_hash },
	{ NULL, NULL }
};

typedef int (*t_flag_handler)(t_context *ctx, char **av, int *i);

typedef struct s_flag_dispatch {
	const char        *flag;
	t_flag_handler     handler;
}	t_flag_dispatch;

static const t_flag_dispatch g_flags[] = {
	{ "-q", handle_flag_q },
	{ "-r", handle_flag_r },
	{ "-p", handle_flag_p },
	{ "-s", handle_flag_s },
	{ NULL, NULL }
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:21:55 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 18:19:48 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_TYPES_H
# define FT_SSL_TYPES_H

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

typedef struct s_context {
	t_flags     flags;
	t_input     *inputs;
	t_b64_flags	b64_flags;
	char        *type;
	int			ac;
	char		**av;
}	t_context;

/*=============== UTILS ===========*/

int		ft_strcmp(const char *s1, const char *s2);
void	print_str(const char *s);
void	*ft_calloc(size_t count, size_t size);
void	free_context(t_context *ctx);
void	ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);
void	write_file(const char *filename, const char *data);
void    *ft_memset(void *b, int c, size_t len);
#endif
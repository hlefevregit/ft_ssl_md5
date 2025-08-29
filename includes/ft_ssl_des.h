/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_des.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:27:46 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/29 11:21:46 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_DES_H
# define FT_SSL_DES_H

#include "ft_types.h"


// Fonctions base64
char	*base64_encode(const char *input);
char	*base64_decode(const char *input, size_t *decoded_len);

char	*base64_process(t_context *ctx);

int		parse_base64_args(t_context *ctx, int ac, char **av);
int		parse_des_args(t_context *ctx, int ac, char **av);

int		prepare_des_crypt_params(t_context *ctx);


/*================FLAGS ============*/

int handle_b64_d(t_context *ctx, char **av, int *i);
int handle_b64_e(t_context *ctx, char **av, int *i);
int handle_b64_i(t_context *ctx, char **av, int *i);
int handle_b64_o(t_context *ctx, char **av, int *i);

int handle_des_flag_d(t_context *ctx, char **av, int *i);
int handle_des_flag_e(t_context *ctx, char **av, int *i);
int handle_des_flag_a(t_context *ctx, char **av, int *i);
int handle_des_flag_i(t_context *ctx, char **av, int *i);
int handle_des_flag_o(t_context *ctx, char **av, int *i);
int handle_des_flag_k(t_context *ctx, char **av, int *i);
int handle_des_flag_p(t_context *ctx, char **av, int *i);
int handle_des_flag_s(t_context *ctx, char **av, int *i);
int handle_des_flag_v(t_context *ctx, char **av, int *i);
int handle_des_flag_P(t_context *ctx, char **av, int *i);

/*===================== BASE64 ======================*/

typedef int (*t_flag_handler_b64)(t_context *ctx, char **av, int *i);

typedef struct s_b64_flag_dispatch {
	const char            *flag;
	t_flag_handler_b64     handler;
}	t_b64_flag_dispatch;

static const t_b64_flag_dispatch g_b64_flags[] = {
	{ "-d", handle_b64_d },
	{ "-e", handle_b64_e },
	{ "-i", handle_b64_i },
	{ "-o", handle_b64_o },
	{ NULL, NULL }
};

/*===================== DES ======================*/

typedef int (*t_des_flag_handler)(t_context *ctx, char **av, int *i);

typedef struct s_des_flag_dispatch {
	const char *flag;
	t_des_flag_handler handler;
}	t_des_flag_dispatch;

static const t_des_flag_dispatch g_des_flags[] = {
	{ "-d", handle_des_flag_d },
	{ "-e", handle_des_flag_e },
	{ "-a", handle_des_flag_a },
	{ "-i", handle_des_flag_i },
	{ "-o", handle_des_flag_o },
	{ "-k", handle_des_flag_k },
	{ "-p", handle_des_flag_p },
	{ "-s", handle_des_flag_s },
	{ "-v", handle_des_flag_v },
	{ "-P", handle_des_flag_P }, // Pour afficher la clé et l'IV
	{ NULL, NULL }
};

#endif
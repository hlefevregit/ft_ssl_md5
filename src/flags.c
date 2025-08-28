/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:24:58 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/28 17:25:15 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

int handle_flag_q(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->flags.q = 1;
	return 0;
}

int handle_flag_r(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->flags.r = 1;
	return 0;
}

int handle_flag_p(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->flags.p = 1;
	add_input(ctx, INPUT_STDIN, NULL);
	return 0;
}

int handle_flag_s(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= 0 && av[*i] && av[*i][0] != '-') {
		add_input(ctx, INPUT_STRING, av[*i]);
		return 0;
	}
	write(2, "ft_ssl: ", 8);
	write(2, ctx->type, ft_strlen(ctx->type));
	write(2, ": -s: No such file or directory\n", 32);
	return 1;
}

int handle_b64_d(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->b64_flags.encode = 0;
	return 0;
}

int handle_b64_e(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->b64_flags.encode = 1;
	return 0;
}

int handle_b64_i(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= 0 && av[*i]) {
		ctx->b64_flags.infile = av[*i];
		return 0;
	}
	write(2, "ft_ssl: base64: missing input file\n", 35);
	return 1;
}

int handle_b64_o(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= 0 && av[*i]) {
		ctx->b64_flags.outfile = av[*i];
		return 0;
	}
	write(2, "ft_ssl: base64: missing output file\n", 36);
	return 1;
}

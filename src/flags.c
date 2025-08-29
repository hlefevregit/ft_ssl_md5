/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:24:58 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/29 11:21:53 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

/*==================================================*/
/*=================== SHA / MD5 ====================*/
/*==================================================*/


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

/*==================================================*/
/*=================== BASE64 =======================*/
/*==================================================*/

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


/*==================================================*/
/*=================== DES ==========================*/
/*==================================================*/

int handle_des_flag_d(t_context *ctx, char **av, int *i)
{
	(void)av;
	ctx->des_flags.encrypt = 0;
	return 0;
}

int handle_des_flag_e(t_context *ctx, char **av, int *i)
{
	(void)av;
	ctx->des_flags.encrypt = 1;
	return 0;
}

int handle_des_flag_a(t_context *ctx, char **av, int *i)
{
	(void)av;
	ctx->des_flags.base64 = 1;
	return 0;
}

int handle_des_flag_i(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing input file after -i\n", 40);
		return 1;
	}
	ctx->des_flags.infile = av[*i];
	return 0;
}

int handle_des_flag_o(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing output file after -o\n", 41);
		return 1;
	}
	ctx->des_flags.outfile = av[*i];
	return 0;
}

int handle_des_flag_k(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing key after -k\n", 33);
		return 1;
	}
	ctx->des_flags.key_hex = av[*i];
	return 0;
}

int handle_des_flag_p(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing password after -p\n", 37);
		return 1;
	}
	ctx->des_flags.password = av[*i];
	return 0;
}

int handle_des_flag_s(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing salt after -s\n", 33);
		return 1;
	}
	ctx->des_flags.salt_hex = av[*i];
	return 0;
}

int handle_des_flag_v(t_context *ctx, char **av, int *i)
{
	if (++(*i) >= ctx->ac) {
		write(2, "ft_ssl des: missing iv after -v\n", 31);
		return 1;
	}
	ctx->des_flags.iv_hex = av[*i];
	return 0;
}

int handle_des_flag_P(t_context *ctx, char **av, int *i)
{
	(void)av;
	(void)i;
	ctx->des_flags.print_key = 1;
	return 0;
}
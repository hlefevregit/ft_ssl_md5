/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:30:43 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 18:20:30 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_des.h"
#include "../../includes/ft_ssl_md5.h"

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

int parse_base64_args(t_context *ctx, int ac, char **av)
{
	int i = 2;

	ctx->b64_flags.encode = 1; // encode par défaut

	while (i < ac) {
		if (!strcmp(av[i], "-d"))
			ctx->b64_flags.encode = 0;
		else if (!strcmp(av[i], "-e"))
			ctx->b64_flags.encode = 1;
		else if (!strcmp(av[i], "-i")) {
			if (++i >= ac) {
				write(2, "ft_ssl: base64: missing input file\n", 35);
				return 1;
			}
			ctx->b64_flags.infile = av[i];
		}
		else if (!strcmp(av[i], "-o")) {
			i++;
			if (i >= ac || av[i][0] == '-') {
				write(2, "ft_ssl: base64: missing output file\n", 36);
				return 1;
			}
			ctx->b64_flags.outfile = av[i];
		}
		else {
			write(2, "ft_ssl: base64: invalid option: ", 32);
			write(2, av[i], strlen(av[i]));
			write(2, "\n", 1);
			return 1;
		}
		i++;
	}
	return 0;
}

char    *base64_process(t_context *ctx)
{
	char *input_data = NULL;
	char *output_data = NULL;

	char *(*read_input)(const char *) = ctx->b64_flags.infile ? read_file : (char *(*)(const char *))read_stdin;

	input_data = read_input(ctx->b64_flags.infile);
	if (!input_data)
	{
		write(2, "ft_ssl: base64: Error reading input\n", 36);
		return NULL;
	}

	char *(*transform)(const char *) = ctx->b64_flags.encode ? base64_encode : base64_decode;

	output_data = transform(input_data);


	if (ctx->b64_flags.outfile) {

		write_file(ctx->b64_flags.outfile, output_data);
		// printf("OUTFILE\n");
	}
	else {
		write(1, output_data, ft_strlen(output_data));
		write(1, "\n", 1);
		
	}
	
	free(input_data);
	return output_data;
}
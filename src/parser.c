/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:51:57 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/28 18:09:28 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

void	add_input(t_context *ctx, t_input_type type, char *data)
{
	t_input *new_input = (t_input *)malloc(sizeof(t_input));
	if (!new_input)
		return;

	new_input->type = type;
	new_input->data = data;
	new_input->next = NULL;
	
	if (!ctx->inputs) {
		ctx->inputs = new_input;
	} else {
		t_input *current = ctx->inputs;
		while (current->next) {
			current = current->next;
		}
		current->next = new_input;
	}

}




int parse_base64_args(t_context *ctx, int ac, char **av)
{
	int i = 2;
	int has_input = 0;

	ctx->b64_flags.encode = 1;

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
			has_input = 1;
		}
		else if (!strcmp(av[i], "-o")) {
			if (++i >= ac || av[i][0] == '-') {
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

	// Si aucun fichier input ET stdin est un terminal => rien à lire
	if (!has_input && isatty(STDIN_FILENO)) {
		write(2, "usage: ft_ssl base64 [-e|-d] [-i file] [-o file]\n", 49);
		return 1;
	}

	return 0;
}

int	parse_args(t_context *ctx, int ac, char **av)
{
	int i = 2;
	int saw_stdin = 0;
	int saw_valid_input = 0;
	if (ft_strcmp(ctx->type, "sha256") == 0 || ft_strcmp(ctx->type, "md5") == 0) {
		while (i < ac) {
			int matched = 0;

			// Check if the arg is a known flag
			for (int j = 0; g_flags[j].flag; j++) {
				if (!ft_strcmp(av[i], g_flags[j].flag)) {
					if (g_flags[j].handler(ctx, av, &i))
						return 1;
					if (!ft_strcmp(av[i], "-p"))
						saw_stdin = 1;
					saw_valid_input = 1;
					matched = 1;
					break;
				}
			}
			if (!matched) {
				if (av[i][0] == '-' && av[i][1] != '\0') {
					write(2, "ft_ssl: ", 8);
					write(2, ctx->type, ft_strlen(ctx->type));
					write(2, ": ", 2);
					write(2, av[i], ft_strlen(av[i]));
					write(2, ": Invalid option\n", 17);
					return 1;
				}
				add_input(ctx, INPUT_FILE, av[i]);
				saw_valid_input = 1;
			}
			i++;
		}
		if (!saw_valid_input && !saw_stdin) {
			write(2, "usage: ft_ssl ", 14);
			write(2, ctx->type, ft_strlen(ctx->type));
			write(2, " [flags] [file/string]\n", 24);
			return 1;
		}

		if (!ctx->inputs && !saw_stdin)
			add_input(ctx, INPUT_STDIN, NULL);
	} 

	return 0;
}


void	print_inputs(t_context *ctx)
{
	t_input *current = ctx->inputs;
	while (current)
	{
		if (current->type == INPUT_STDIN)
			write(1, "Input Type: STDIN\n", 18);
		else if (current->type == INPUT_STRING)
			write(1, "Input Type: STRING\n", 19);
		else if (current->type == INPUT_FILE)
			write(1, "Input Type: FILE\n", 17);
		if (current->data)
		{
			write(1, "Data: ", 6);
			write(1, current->data, strlen(current->data));
			write(1, "\n", 1);
		}
		current = current->next;
	}
	
	if (ctx->flags.p)
		write(1, "Flag -p is set\n", 15);
	if (ctx->flags.q)
		write(1, "Flag -q is set\n", 15);
	if (ctx->flags.r)
		write(1, "Flag -r is set\n", 15);
	if (ctx->flags.s)
		write(1, "Flag -s is set\n", 15);
}
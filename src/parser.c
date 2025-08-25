/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:51:57 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 14:40:15 by hugolefevre      ###   ########.fr       */
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

int		parse_args(t_context *ctx, int ac, char **av)
{
	int i = 2; // Start after the command (md5/sha256)

	while (i < ac) {
		if (!ft_strcmp(av[i], "-q")) {
			ctx->flags.q = 1;
		} else if (!ft_strcmp(av[i], "-p")) {
			ctx->flags.p = 1;
		} else if (!ft_strcmp(av[i], "-r")) {
			ctx->flags.r = 1;
		} else if (!ft_strcmp(av[i], "-s")) {
			if (++i >= ac) {
				printf("ft_ssl: %s: option requires an argument -- s\n", ctx->type);
				return (1);
			}
			add_input(ctx, INPUT_STRING, av[i]);
			
		} else if (!ft_strcmp(av[i], "-")) {
			printf("ft_ssl: %s: invalid option\n", ctx->type);
			return 1;
		} else {
			add_input(ctx, INPUT_FILE, av[i]);
		}
		i++;
	}
	if (!ctx->inputs) {
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
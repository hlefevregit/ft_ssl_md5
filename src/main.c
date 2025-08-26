/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:09:41 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 11:14:40 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

char	*run_hash(const char *type, const char *input)
{
	if (!input || !type)
		return NULL;

	for (int i = 0; g_hashes[i].name; i++) {
		if (strcmp(type, g_hashes[i].name) == 0)
			return g_hashes[i].func(input);
	}
	return NULL;
}

static void handle_file(t_context *ctx, t_input *cur)
{
	char *input_data = read_file(cur->data);
	if (!input_data) {
		write(2, "ft_ssl: ", 8);
		write(2, cur->data, ft_strlen(cur->data));
		write(2, ": No such file or directory\n", 29);
		return;
	}
	char *hash = run_hash(ctx->type, input_data);

	if (ctx->flags.q)
		printf("%s\n", hash);
	else if (ctx->flags.r)
		printf("%s %s\n", hash, cur->data);
	else
		printf("%s (%s) = %s\n", ctx->type, cur->data, hash);

	free(input_data);
	free(hash);
}

static void handle_string(t_context *ctx, t_input *cur)
{
	char *input_data = cur->data;
	char *hash = run_hash(ctx->type, input_data);

	if (ctx->flags.q)
		printf("%s\n", hash);
	else if (ctx->flags.r)
		printf("%s \"%s\"\n", hash, input_data);
	else
		printf("%s (\"%s\") = %s\n", ctx->type, input_data, hash);

	free(hash);
}

static void handle_stdin(t_context *ctx)
{
	char *input_data = read_stdin();
	if (ctx->flags.p)
		write(1, input_data, ft_strlen(input_data));
	char *hash = run_hash(ctx->type, input_data);

	if (ctx->flags.q)
		printf("%s\n", hash);
	else if (ctx->flags.r)
		printf("%s %s\n", hash, input_data);
	else
		printf("(\"%s\")= %s\n", input_data, hash);

	free(input_data);
	free(hash);
}

void	process_input(t_context *ctx)
{
	t_input *cur = ctx->inputs;

	while (cur) {
		if (cur->type == INPUT_STDIN)
			handle_stdin(ctx);
		else if (cur->type == INPUT_STRING)
			handle_string(ctx, cur);
		else if (cur->type == INPUT_FILE)
			handle_file(ctx, cur);
		cur = cur->next;
	}
}


int main(int ac, char **av)
{
	if (ac < 2 || (ft_strcmp(av[1], "md5") != 0 && ft_strcmp(av[1], "sha256") != 0))
	{
		write(2, "usage: ft_ssl command [command opts] [command args]\n", 53);
		return (1);
	}
	
	t_context *ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (1);
	ctx->flags.p = 0;
	ctx->flags.q = 0;
	ctx->flags.r = 0;
	ctx->flags.s = 0;
	ctx->inputs = NULL;
	if (ft_strcmp(av[1], "md5") == 0)
		ctx->type = "md5";
	else if (ft_strcmp(av[1], "sha256") == 0)
		ctx->type = "sha256";
	else
	{
		write(2, "ft_ssl: Error: Invalid command\n", 31);
		return (1);
	}
	
	if (parse_args(ctx, ac, av) != 0) {
		free_context(ctx);
		return (1);
	}

	// print_inputs(ctx);

	process_input(ctx);

	free_context(ctx);

	return 0;
}

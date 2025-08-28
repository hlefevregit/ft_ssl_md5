/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:09:41 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/28 18:13:41 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

void	print_usage(const char *invalid)
{
	if (invalid) {
		write(2, "ft_ssl: Error: '", 17);
		write(2, invalid, ft_strlen(invalid));
		write(2, "' is an invalid command.\n", 26);
	}
	write(2, "Commands:\n", 10);
	for (int i = 0; g_commands[i].name; i++) {
		write(2, g_commands[i].name, ft_strlen(g_commands[i].name));
		write(2, "\n", 1);
	}
	write(2, "Flags:\n-p -q -r -s\n", 19);
}

char	*run_hash(const char *type, t_context *ctx)
{
	if (!ctx || !type)
		return NULL;

	for (int i = 0; g_hashes[i].name; i++) {
		if (strcmp(type, g_hashes[i].name) == 0)
			return g_hashes[i].func(ctx);
	}
	return NULL;
}

static void handle_file(t_context *ctx, t_input *cur)
{
	char *input_data = read_file(cur->data);
	if (!input_data) {
		write(2, "ft_ssl: ", 8);
		write(2, cur->data, ft_strlen(cur->data)); // cur->data est le nom du fichier (ex: "e")
		write(2, ": No such file or directory\n", 29);
		return;
	}

	// Associe les données à cur->data pour les fonctions de hash
	cur->data = input_data;

	char *hash = run_hash(ctx->type, ctx);

	if (ctx->flags.q)
		printf("%s\n", hash);
	else if (ctx->flags.r)
		printf("%s %s\n", hash, ctx->inputs->next ? ctx->inputs->next->data : "(stdin)");
	else
		printf("%s (%s) = %s\n", ctx->type, ctx->inputs->next ? ctx->inputs->next->data : "(stdin)", hash);

	free(input_data);
	free(hash);
}


static void handle_string(t_context *ctx, t_input *cur)
{
	char *input_data = cur->data;
	ctx->inputs->data = input_data;
	char *hash = run_hash(ctx->type, ctx);

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
	ctx->inputs->data = input_data;
	char *hash = run_hash(ctx->type, ctx);

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
int	main(int ac, char **av)
{
	if (ac < 2) {
		write(2, "usage: ft_ssl command [flags] [file/string]\n", 45);
		return 1;
	}

	t_context *ctx = malloc(sizeof(t_context));
	if (!ctx)
		return 1;
	ft_memset(ctx, 0, sizeof(t_context));
	ctx->ac = ac;
	ctx->av = av;

	const t_command *cmd = NULL;
	for (int i = 0; g_commands[i].name; i++) {
		if (!ft_strcmp(av[1], g_commands[i].name)) {
			cmd = &g_commands[i];
			break;
		}
	}

	if (!cmd) {
		print_usage(av[1]);
		free_context(ctx);
		return 1;
	}

	ctx->type = (char *)cmd->name;

	if (cmd->parser && cmd->parser(ctx, ac, av) != 0) {
		free_context(ctx);
		return 1;
	}

	if (cmd->needs_inputs)
		process_input(ctx);
	else {
		char *output = cmd->func(ctx);
		if (output) {
			write(1, output, ft_strlen(output));
			write(1, "\n", 1);
			free(output);
		}
	}

	free_context(ctx);
	return 0;
}

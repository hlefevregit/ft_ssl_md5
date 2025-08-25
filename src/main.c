/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:09:41 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/25 17:40:42 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

char	*run_hash(const char *type, const char *input) {
    if (!input)
        return NULL;
    if (strcmp(type, "md5") == 0)
        return md5_hash(input);
    else if (strcmp(type, "sha256") == 0)
        return sha256_hash(input);
    return NULL;
}

void	process_input(t_context *ctx)
{
	t_input *cur = ctx->inputs;
	
	while (cur) {
		char *input_data = NULL;

		if (cur->type == INPUT_STDIN) {
			input_data = read_stdin();
			char *hash = run_hash(ctx->type, input_data);

			if (ctx->flags.q == 1) {
				printf("%s\n", hash);
			} else if (ctx->flags.r == 1) {
				printf("%s %s\n", hash, input_data);
			} else if (ctx->flags.p == 1) {
				printf("%s (\"%s\") = %s\n", ctx->type, input_data, hash);
			} else {
				printf("(stdin)= %s\n", hash);
			}
			if (input_data)
				free(input_data);
			if (hash)
				free(hash);
		} else if (cur->type == INPUT_STRING) {
			input_data = cur->data;
			char *hash = run_hash(ctx->type, input_data);

			if (ctx->flags.q == 1) {
				printf("%s\n", hash);
			} else if (ctx->flags.r == 1) {
				printf("%s \"%s\"\n", hash, input_data);
			} else {
				printf("%s (\"%s\") = %s\n", ctx->type, input_data, hash);
			}
			if (hash)
				free(hash);
			
		} else if (cur->type == INPUT_FILE) {
			input_data = read_file(cur->data);
			char *hash = run_hash(ctx->type, input_data);
			if (!input_data) {
				printf("ft_ssl: %s: No such file or directory\n", cur->data);
				cur = cur->next;
				continue;
			}

			if (ctx->flags.q == 1) {
				printf("%s\n", hash);
			} else if (ctx->flags.r == 1) {
				printf("%s %s\n", hash, cur->data);
			} else {
				printf("%s (%s) = %s\n", ctx->type, cur->data, hash);
			}
			if (input_data)
				free(input_data);
			if (hash)
				free(hash);
		}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:30:43 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/09/02 16:12:57 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl_des.h"
#include "../../includes/ft_ssl_md5.h"


char *base64_process(t_context *ctx)
{
	char *input_data = NULL;
	char *output_data = NULL;

	if (ctx->b64_flags.infile)
		input_data = read_file(ctx->b64_flags.infile);
	else
		input_data = read_stdin();

	if (!input_data) {
		write(2, "ft_ssl: base64: Error reading input\n", 36);
		return NULL;
	}


	if (ctx->b64_flags.encode)
	{
		output_data = base64_encode(input_data, ft_strlen(input_data));
	}
	else
	{
		size_t decoded_len;
		output_data = base64_decode(input_data, &decoded_len);
		if (!output_data)
			return NULL;

		if (ctx->b64_flags.outfile)
			write_file(ctx->b64_flags.outfile, output_data);
		else
			write(1, output_data, decoded_len);

		free(input_data);
		free(output_data);
		return NULL;
	}

	if (ctx->b64_flags.outfile) {
		write_file(ctx->b64_flags.outfile, output_data);
		free(input_data);
		free(output_data);
		return NULL; // pas besoin de ré-afficher
	}

	free(input_data);
	return output_data;  // sera affiché dans le main
}

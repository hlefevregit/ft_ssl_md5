/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:08:07 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/09/04 15:34:41 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

char	*read_stdin(void)
{
	char buffer[BUFFER_SIZE + 1];
	int ret;
	char *res = NULL;
	int total = 0;

	while ((ret = read(0, buffer, BUFFER_SIZE)) > 0) {
		char *new_res = (char *)malloc(total + ret + 1);
		if (!new_res) {
			free(res);
			return NULL;
		}
		for (int i = 0; i < total; i++)
			new_res[i] = res ? res[i] : 0;
		for (int i = 0; i < ret; i++)
			new_res[total + i] = buffer[i];

		new_res[total + ret] = '\0';
		if (res)
			free(res);
		res = new_res;
		total += ret;
	}
	return res;
}

char	*read_file(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("ft_ssl: %s: No such file or directory\n", filename);
		return NULL;
	}

	char buffer[BUFFER_SIZE + 1];
	int ret;
	char *res = NULL;
	int total = 0;

	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0) {
		char *new_res = (char *)malloc(total + ret + 1);
		if (!new_res) {
			free(res);
			close(fd);
			return NULL;
		}
		for (int i = 0; i < total; i++)
			new_res[i] = res[i];
		for (int i = 0; i < ret; i++)
			new_res[total + i] = buffer[i];
		new_res[total + ret] = '\0';
		free(res);
		res = new_res;
		total += ret;
	}
	close(fd);
	return res;
}
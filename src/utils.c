/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugolefevre <hugolefevre@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:53:12 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/26 18:19:26 by hugolefevre      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

void print_str(const char *s) {
    write(1, s, strlen(s));
}

void *ft_calloc(size_t count, size_t size) {
    void *mem = malloc(count * size);
    if (!mem) return NULL;
    for (size_t i = 0; i < count * size; i++)
        ((unsigned char*)mem)[i] = 0;
    return mem;
}

void free_context(t_context *ctx) {
    t_input *cur = ctx->inputs;
    t_input *next;

    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(ctx);
}

void ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;
	for (size_t i = 0; i < n; i++)
		d[i] = s[i];
}

void    *ft_memset(void *b, int c, size_t len)
{
    unsigned char *p = b;
    while (len--)
        *p++ = (unsigned char)c;
    return b;
}

size_t ft_strlen(const char *s)
{
	size_t len = 0;
	while (s[len])
		len++;
	return len;
}
void write_file(const char *filename, const char *data) {
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("open");
		return;
	}
	write(fd, data, strlen(data));
	close(fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:53:12 by hugolefevre       #+#    #+#             */
/*   Updated: 2025/08/29 11:39:06 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"
#include <limits.h>
#include <errno.h>

int ft_strcmp(const char *s1, const char *s2) {
	while (*s1 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int ft_strncmp(const char *s1, const char *s2, size_t n) {
	while (n && *s1 && *s1 == *s2) {
		s1++;
		s2++;
		n--;
	}
	if (n == 0) return 0;
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

int ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\n' ||
			c == '\r' || c == '\t' || c == '\v');
}

int ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

long    ft_strtol(const char *nptr, char **endptr, int base)
{
	const char *s = nptr;
	unsigned long acc = 0;
	int c;
	int neg = 0, any = 0, cutoff, cutlim;

	while (ft_isspace((unsigned char)*s))
		s++;
	if (*s == '-') {
		neg = 1;
		s++;
	} else if (*s == '+')
		s++;
	if ((base == 0 || base == 16) &&
		s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = s[0] == '0' ? 8 : 10;

	cutoff = neg ? -(LONG_MIN / base) : LONG_MAX / base;
	cutlim = neg ? -(LONG_MIN % base) : LONG_MAX % base;

	for (; ; s++) {
		c = (unsigned char)*s;
		if (isdigit(c))
			c -= '0';
		else if (ft_isalpha(c))
			c -= ft_isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr)
		*endptr = (char *)(any ? s : nptr);
	return acc;
}

int	ft_hexchar_to_val(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1; // invalide
}


int	hexstr_to_bytes(const char *hex, uint8_t *out, size_t expected_len)
{
	size_t len = ft_strlen(hex);
	if (len != expected_len * 2)
		return 1;

	for (size_t i = 0; i < expected_len; i++) {
		char high = hex[i * 2];
		char low = hex[i * 2 + 1];

		int hi_val = ft_hexchar_to_val(high);
		int lo_val = ft_hexchar_to_val(low);

		if (hi_val < 0 || lo_val < 0)
			return 1;

		out[i] = (uint8_t)((hi_val << 4) | lo_val);
	}
	return 0;
}

char    *ft_strdup(const char *s)
{
	size_t len = ft_strlen(s);
	char *dup = (char *)malloc(len + 1);
	if (!dup) return NULL;
	for (size_t i = 0; i <= len; i++)
		dup[i] = s[i];
	return dup;
}
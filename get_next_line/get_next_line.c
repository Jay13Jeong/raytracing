/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 11:32:27 by jjeong            #+#    #+#             */
/*   Updated: 2021/06/14 11:32:33 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	split_str(unsigned char *frame, unsigned char **str)
{
	int				idx;
	unsigned char	*temp;

	idx = 0;
	while (frame[idx])
		++idx;
	if (idx == 0)
	{
		free(*str);
		*str = NULL;
		return ;
	}
	temp = malloc(sizeof(unsigned char) * (idx + 1));
	if (temp != NULL)
	{
		idx = 0;
		while (frame[idx])
		{
			temp[idx] = frame[idx];
			++idx;
		}
		temp[idx] = '\0';
	}
	free(*str);
	*str = temp;
}

void	alloc_line(unsigned char *str, char **line, int len)
{
	int				idx;
	unsigned char	*temp;

	temp = malloc(sizeof(unsigned char) * (len + 1));
	if (temp == NULL)
		return ;
	idx = 0;
	while (idx < len)
	{
		temp[idx] = str[idx];
		++idx;
	}
	temp[idx] = '\0';
	*line = (char *)temp;
}

int		find_enter(unsigned char **str, char **line)
{
	int				idx;
	unsigned char	*dest;

	dest = *str;
	idx = 0;
	while (*str != NULL && dest[idx])
	{
		if (dest[idx] == '\n')
		{
			alloc_line(*str, line, idx);
			split_str(*str + idx + 1, str);
			return (1);
		}
		++idx;
	}
	return (0);
}

void	join_str(unsigned char **str, unsigned char *buf, int read_size)
{
	int				len_str;
	int				idx;
	unsigned char	*temp;
	unsigned char	*dest;

	dest = *str;
	len_str = 0;
	while (dest != NULL && dest[len_str])
		++len_str;
	temp = malloc(sizeof(unsigned char) * (len_str + read_size + 1));
	if (temp != NULL)
	{
		len_str = 0;
		while (dest != NULL && dest[len_str])
		{
			temp[len_str] = dest[len_str];
			++len_str;
		}
		idx = -1;
		while (++idx < read_size)
			temp[len_str + idx] = buf[idx];
		temp[len_str + idx] = '\0';
	}
	free(*str);
	*str = temp;
}

int		get_next_line(int fd, char **line)
{
	static unsigned char	*collection[OPEN_MAX];
	unsigned char			buf[BUFFER_SIZE + 1];
	int						read_size;

	if (BUFFER_SIZE < 1 || fd < 0 || line == NULL)
		return (-1);
	*line = NULL;
	while (1)
	{
		read_size = read(fd, buf, BUFFER_SIZE);
		if (read_size == -1)
			return (-1);
		buf[read_size] = '\0';
		if (read_size > 0)
			join_str(&collection[fd], buf, read_size);
		if (find_enter(&collection[fd], line))
			return (proccess_line(1, line));
		else if (read_size == 0)
		{
			if (collection[fd] != NULL && *collection[fd])
				alloc_line_all(&collection[fd], line);
			return (proccess_line(0, line));
		}
	}
}

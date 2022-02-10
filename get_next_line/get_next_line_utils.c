/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 11:32:21 by jjeong            #+#    #+#             */
/*   Updated: 2021/06/14 11:32:34 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		proccess_line(int result, char **line)
{
	unsigned char *temp;

	if (*line == NULL)
	{
		temp = malloc(sizeof(unsigned char));
		if (temp != NULL)
			temp[0] = '\0';
		*line = (char *)temp;
	}
	return (result);
}

void	alloc_line_all(unsigned char **str, char **line)
{
	unsigned char	*temp;
	unsigned char	*dest;
	int				idx;

	dest = *str;
	idx = 0;
	while (dest != NULL && dest[idx])
		++idx;
	temp = malloc(sizeof(unsigned char) * (idx + 1));
	if (temp != NULL)
	{
		idx = 0;
		while (dest != NULL && dest[idx])
		{
			temp[idx] = dest[idx];
			++idx;
		}
		temp[idx] = '\0';
	}
	*line = (char *)temp;
	free(*str);
	*str = NULL;
}

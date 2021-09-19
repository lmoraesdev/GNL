/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbatista <lbatista@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 13:07:34 by lbatista          #+#    #+#             */
/*   Updated: 2021/09/19 14:54:07 by lbatista         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*extract_line(char **backup)
{
	int		i;
	char	*line;
	char	*t_free;

	i = 0;
	while ((*backup)[i] != '\0' && (*backup)[i] != '\n')
		i++;
	t_free = *backup;
	line = ft_substr(t_free, 0, i + 1);
	*backup = ft_strdup(&(*backup)[i + 1]);
	free_ptr(&t_free);
	return (line);
}

static int	read_file(int fd, char **buffer, char **backup)
{
	int		b_read;
	char	*t_free;

	b_read = 1;
	while (!ft_strchr(*backup, '\n') && b_read)
	{
		b_read = read(fd, *buffer, BUFFER_SIZE);
		if (b_read == -1)
			return (b_read);
		(*buffer)[b_read] = '\0';
		t_free = *backup;
		*backup = ft_strjoin(t_free, *buffer);
		free_ptr(&t_free);
	}
	return (b_read);
}

static char	*get_line(int fd, char **buffer, char **backup)
{
	int		b_read;
	char	*t_free;

	if (ft_strchr(*backup, '\n'))
		return (extract_line(backup));
	b_read = read_file(fd, buffer, backup);
	if ((b_read == 0 || b_read == -1) && !**backup)
	{
		free_ptr(backup);
		return (NULL);
	}
	if (ft_strchr(*backup, '\n'))
		return (extract_line(backup));
	if (!ft_strchr(*backup, '\n') && **backup)
	{
		t_free = ft_strdup(*backup);
		free_ptr(backup);
		return (t_free);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char		*backup[OPEN_MAX];
	char			*buffer;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!backup[fd])
		backup[fd] = ft_strdup("");
	line = get_line(fd, &buffer, &backup[fd]);
	free_ptr(&buffer);
	return (line);
}

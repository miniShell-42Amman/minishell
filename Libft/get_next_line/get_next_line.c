/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:53:51 by oissa             #+#    #+#             */
/*   Updated: 2024/10/07 17:53:51 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define LIST_SIZE 4096

static char	*ft_read(int fd, char (*reminder)[LIST_SIZE], char *buffer)
{
	char	*line;
	char	*tmp;
	ssize_t	read_bytes;

	line = ft_strdup(*reminder);
	(*reminder)[0] = '\0';
	read_bytes = 1;
	while (read_bytes != 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(line);
			return (NULL);
		}
		buffer[read_bytes] = '\0';
		tmp = line;
		line = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n') || read_bytes == 0)
			break ;
	}
	return (line);
}

static char	*ft_save_line(char *line, char (*reminder)[LIST_SIZE])
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		i++;
	tmp = ft_substr(line, 0, i);
	ft_strlcpy(*reminder, &line[i], LIST_SIZE);
	free(line);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	reminder[LIST_SIZE];
	char		*buffer;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	line = ft_read(fd, &reminder, buffer);
	free(buffer);
	if (!line || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (ft_save_line(line, &reminder));
}

/*int main()
{
    int fd;
    char *line;

    fd = open("f.txt", O_RDONLY);
    // while (())
    // {
        line = get_next_line(fd);
        printf("%s", line);
        free(line);
    // }
    close(fd);
    return (0);
}*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 18:20:31 by oissa             #+#    #+#             */
/*   Updated: 2024/10/07 18:20:31 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#define MAX_FD 1024
#define LIST_SIZE 4096

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

char	*get_next_line_bonus(int fd)
{
	static char	reminder[MAX_FD][LIST_SIZE];
	char		*line;
	char		*buffer;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	line = ft_read(fd, &reminder[fd], buffer);
	free(buffer);
	if (!line || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (ft_save_line(line, &reminder[fd]));
}

/*int main()
{
    int fd;
    char *line;

    fd = open("f.txt", O_RDONLY);
    // while ((line = get_next_line(fd)))
    // {
    line = get_next_line(fd);
    printf("%s", line);
    free(line);
    // }
    close(fd);
    return (0);
}*/
/*int main ()
{
    int fd1 , fd2 , fd3;
    char *line;

    fd1 = open("f.txt", O_RDONLY);
    fd2 = open("f2.txt", O_RDONLY);
    fd3 = open("f3.txt", O_RDONLY);
    printf("fd1\n");
    printf("\n");
    while ((line = get_next_line(fd1)))
    {
        printf("%s", line);
        free(line);
    }
    printf("\n");
    printf("fd2\n");
    while ((line = get_next_line(fd2)))
    {
        printf("%s", line);
        free(line);
    }
    printf("\n");
    printf("fd3\n");
    while ((line = get_next_line(fd3)))
    {
        printf("%s", line);
        free(line);
    }
}*/
/*int main()
{
    int fd1, fd2, fd3;
    char *line;

    fd1 = open("f.txt", O_RDONLY);
    fd2 = open("f2.txt", O_RDONLY);
    fd3 = open("f3.txt", O_RDONLY);
    printf("fd1\n");
    printf("\n");

    line = get_next_line(fd1);
    printf("%s", line);
    free(line);

    printf("\n");
    printf("fd2\n");

    line = get_next_line(fd2);
    printf("%s", line);
    free(line);

    printf("\n");
    printf("fd3\n");
    line = get_next_line(fd3);
    printf("%s", line);
    free(line);
}*/
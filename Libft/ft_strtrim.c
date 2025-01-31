/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 11:28:47 by oissa             #+#    #+#             */
/*   Updated: 2024/09/06 18:46:53 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (0);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	ret = malloc(sizeof(char) * (end - start + 1));
	if (!ret)
		return (0);
	ft_strlcpy(ret, s1 + start, end - start + 1);
	return (ret);
}
/*#include <stdio.h>
int main()
{
    const char *str = "  Hello, World!  ";
    const char *set = " ";
    char *trimmed_str;

    // طباعة السلسلة النصية الأصلية والمجموعة
    printf("Original string: \"%s\"\n", str);
    printf("Set of characters to trim: \"%s\"\n", set);

    // استدعاء ft_strtrim لإزالة الأحرف المحددة من بداية ونهاية السلسلة النصية
    trimmed_str = ft_strtrim(str, set);

    // طباعة النتيجة
    if (trimmed_str)
    {
        printf("Trimmed string: \"%s\"\n", trimmed_str);
        free(trimmed_str); // تحرير الذاكرة المخصصة
    }
    else
    {
        printf("Failed to trim string.\n");
    }

    return 0;
}*/
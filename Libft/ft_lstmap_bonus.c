/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:46:31 by oissa             #+#    #+#             */
/*   Updated: 2024/09/03 09:25:47 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_llist;
	t_list	*new_element;
	void	*content;

	if (!lst)
		return (NULL);
	new_llist = NULL;
	while (lst)
	{
		content = (*f)(lst->content);
		new_element = ft_lstnew(content);
		if (!new_element)
		{
			if (content)
				del(content);
			ft_lstclear(&new_llist, del);
			return (NULL);
		}
		ft_lstadd_back(&new_llist, new_element);
		lst = lst->next;
	}
	return (new_llist);
}

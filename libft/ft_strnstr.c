/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:50:47 by shechong          #+#    #+#             */
/*   Updated: 2023/05/09 15:26:56 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *small, size_t n)
{
	size_t	i;

	i = 0;
	if (small[0] == '\0')
		return ((char *)big);
	if (!small[i] || small == big)
		return ((char *)big);
	if (n == 0)
		return (NULL);
	while (big[i] && i + ft_strlen(small) <= n)
	{
		if (ft_strncmp(&big[i], small, ft_strlen(small)) == 0)
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}

/* #include <stdio.h>
int main()
{
	printf("%s", ft_strnstr("aaabcabcd", "aaabc", 5));
} */
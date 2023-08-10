/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 11:03:23 by shechong          #+#    #+#             */
/*   Updated: 2023/07/03 11:06:35 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free_array(void **array, int ret, int tofree)
{
	size_t	i;

	i = 0;
	if (tofree < 0)
		return (ret);
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (ret);
}

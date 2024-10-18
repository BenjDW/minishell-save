/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill <tcavaill@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 13:42:41 by tcavaill          #+#    #+#             */
/*   Updated: 2023/10/03 13:42:41 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append(char *s1, char *s2, char c)
{
	char	*new_s;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	new_s = malloc(sizeof(char) * (i + j + 2));
	i = -1;
	j = -1;
	while (s1[++i])
		new_s[i] = s1[i];
	new_s[i++] = c; 
	while (s2[++j])
		new_s[i + j] = s2[j];
	new_s[i + j] = '\0';
	return (new_s);
}

char	*join_str(char **table, char c)
{
	char	*new_str;
	char	*old_str;

	if (*table == NULL)
		return (NULL);
	new_str = duplicate_str(table[0]);
	while(*++table)
	{
		old_str = new_str;
		new_str = append(old_str, *table, c);
		free(old_str);
	}
	return (new_str);
}
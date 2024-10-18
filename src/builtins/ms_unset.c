/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:56:17 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/14 02:56:17 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_unset(t_minishell minishell, char **words)
{
	(void)words;
	return (minishell);
}*/

int	search_var(char *search, char *senvp)
{
	int	i;

	i = 0;
	while (search[i] != '\0')
	{
		if (search[i] != senvp[i])
			return (-1);
		i++;
	}
	if (senvp[i] == '=')
		return (0);
	return (-1);
}

int	ms_unset(t_parser *data, int i, int j, char **multi)
{
	printf("arg unset %s\n", data->command_table->command->arguments);
	multi = split_str(data->command_table->command->arguments, ' ');
	data->cmd_builtin++;
	if (data->command_table->command->arguments == NULL)
		return (set_error(data, 127), free_split_str(multi), 1);
	while (multi[++j])
	{
		i = -1;
		while (data->envp[++i] != NULL)
		{
			if (search_var(multi[j], data->envp[i]) == 0)
				break ;
		}
		printf("apres search result %s\n", data->envp[i]);
		if (data->envp[i] != NULL) //1 = supr
			data->envp = suprlineenvp(data, i);/* 1,*/
		else
			printf("error missing var %s\n",
				data->command_table->command->arguments);
	}
	free_split_str(multi);
	return (0);
}

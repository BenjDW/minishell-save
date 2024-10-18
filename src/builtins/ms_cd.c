/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:55:18 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/14 02:55:18 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_cd(t_minishell minishell, char **words)
{
	(void)words;
	return (minishell);
}*/

int	ms_cd(t_parser *data)/*, pid_t *pid*/
{
	//pid = -1;
	data->cmd_builtin++;
	if (data->command_table->command->arguments == NULL)
		return (printf("error: no argument\n"), 0);
	// Change directory
	if (chdir(data->command_table->command->arguments) != 0)
		perror("cd");
	else
	{
		printf("Directory changed to %s\n",
			data->command_table->command->arguments);
		//ms_pwd(data, 0);
	}
	return (0);
}

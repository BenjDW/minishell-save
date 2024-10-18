/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:55:30 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/14 02:55:30 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_exit(t_minishell minishell, char **words)
{
	(void)words;
	minishell.flag_exit = TRUE;
	return (minishell);
}*/

int	ms_exit(t_parser *data, int i)
{
	int	er;

	er = data->handle_error;
	if (data->command_table->command->arguments != NULL)
	{
		while (data->command_table->command->arguments[++i] != '\0')
		{
			if (data->command_table->command->arguments[i] < 48
				|| data->command_table->command->arguments[i] > 57)
			{
				data->cmd_builtin++;
				return (set_error(data, 0), 1);
			}
		}
		if (ft_atoi(data->command_table->command->arguments) >= 256)
			set_error(data,
				(ft_atoi(data->command_table->command->arguments) % 256));
		free_exit(data);
		exit(ft_atoi(data->command_table->command->arguments));
	}
	else
		return (free_exit(data), exit(er), 0);
}

void	free_exit(t_parser *data)
{
	free_ex(data);
	free_all(data);
	//free_parser(data, -1);
}

/* free l'ensemble de la liste , fonctionne
erreur 3 4 conditionnal jump en cas de ctrl d sans precedent
possible invalide free etc
void	free_parser(t_parser *data, int i)
{
	if (data->envp != NULL)
		free_split_str(data->envp); // Libère l'envp
	if (data->tab_path != NULL)
		free_split_str(data->tab_path);

	// Libération des pipes
	if (data->pipefd != NULL)
	{
		while(++i != data->size_li)
		{
			printf("free data->pipefd[%d] size %d\n", i, data->size_li);
			free(data->pipefd[i]);
		}
		free(data->pipefd);
		data->pipefd = NULL;
	}
	// Libérer la liste chaînée des commandes
	if (data->command_table != NULL)
		free_command_table(data->command_table);
	if (data->redirection_in != NULL)
		free(data->redirection_in);
	if (data->redirection_out != NULL)
		free(data->redirection_out);
	free(data);
}

void	free_command_table(t_command_table *table)
{
	t_command_table	*tmp;

	while (table != NULL)
	{
		tmp = table;          // Sauvegarde du pointeur actuel
		free_command(table->command); // Libère les commandes et arguments
		table = table->next;  // Passe au nœud suivant
		free(tmp);            // Libère le nœud actuel
	}
}

void	free_command(t_command *cmd)
{
	if (cmd->command)
		free(cmd->command);
	if (cmd->arguments)
		free(cmd->arguments);
	if (cmd->cmd_ex != NULL)
		free_split_str(cmd->cmd_ex); // Fonction qui libère un tableau de chaînes
	free(cmd);
}
*/
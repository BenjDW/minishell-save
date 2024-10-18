/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:55:40 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/14 02:55:40 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_env(t_minishell minishell, char **words)
{
	(void)words;
	return (minishell);
}*/

/*int		ms_env(t_parser *data, pid_t *pid)
{
	int i;

	i = 0;
	pid[data->index + data->cmd_builtin] = fork();
	if (pipe_redir(data) == -1)
		return (set_error(data, 126), perror("fd"), 1);
	close_pipe_child(data);
	if (data->command_table->command->arguments != NULL)
		return (write(2, "env: invalid option\n", 20), 1);
	if (data->envp == NULL)
		return (write(2, "empty env", 10), 1);
	while (data->envp[i] != NULL)
	{
		ft_putstr_fd(data->envp[i], 1);
		//printf("\n");
		i++;
	}
	return (0);
}*/

int	ms_env(t_parser *data, pid_t *pid, int i)
{
	pid[data->index + data->cmd_builtin] = fork();
	if (pid[data->index + data->cmd_builtin] == 0)
	{
		if (pipe_redir(data) == -1)
			return (set_error(data, 126), perror("fd"), 1);
		close_pipe_child(data);
		if (data->command_table->command->arguments != NULL)
		{
			write(2, "env: invalid option\n", 20);
			return (set_error(data, 127), exit(1), 1);
		}
		if (data->envp == NULL)
		{
			write(2, "empty env\n", 10);
			return (set_error(data, 127), exit(1), 1);
		}
		//PRINT que les var avec une value var=value sinon skip
		while (data->envp[++i] != NULL)
			printf("%s\n", data->envp[i]);
		exit(0);
	}
	//data->command_table = data->command_table->next;
	waitpid(pid[data->index + data->cmd_builtin], &data->status, 0);
	data->cmd_builtin++;
	return (0);
}

/*void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:41:52 by tcavaill          #+#    #+#             */
/*   Updated: 2024/06/10 12:41:52 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_pwd(t_minishell minishell, char **words)
{
	(void)words;
	return (minishell);
}*/

int	ms_pwd(t_parser *data, pid_t *pid)
{
	char	path[1028];

	pid[data->index + data->cmd_builtin] = fork();
	if (pid[data->index + data->cmd_builtin] == 0)
	{
		if (pipe_redir(data) == -1)
			return (set_error(data, 126), perror("fd"), 1);
		close_pipe_child(data);
		getcwd(path, 1028);
		printf("%s\n", path);
		exit(0);
	}
	waitpid(pid[data->index + data->cmd_builtin], &data->status, 0);
	data->cmd_builtin++;
	return (0);
}

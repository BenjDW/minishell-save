/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 08:36:06 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/17 08:36:06 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint(int sign)//ctrl-C
{
	sign = 0;
	printf("\n");
	rl_on_new_line();// Réinitialiser la ligne
	rl_replace_line("", 0);// Effacer la ligne en cours dans `readline`
	rl_redisplay();
}

void	init_sign(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell		minishell;
	t_parser		*data;
	char			*line;

	(void)argc;
	(void)argv;
	init_sign();
	minishell.flag_exit = FALSE;
	minishell.flag_error = NO_ERROR;
	data = malloc(sizeof(t_parser));
	init_envp(data, env);
	data->handle_error = 0;
	while (!minishell.flag_exit)
	{
		line = readline("TEST:");
		if (line == NULL)
			return (printf("exit\n"), free_all(data), 0);
		else
		{
			add_history(line);
			//printf("%s str\n", str_var(data, line, NULL, -1));
			minishell = parser(minishell, line, data);
		}
	}
	return (/*free_all(data), */minishell.flag_error);//free le reste pas deja free
}

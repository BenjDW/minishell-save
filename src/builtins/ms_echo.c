/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:41:19 by tcavaill          #+#    #+#             */
/*   Updated: 2024/06/10 12:41:19 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_echo(t_minishell minishell, char **words)
{
	int	i;

	i = 0;
	while(words[++i])
		printf("%s ", words[i]);
	printf("\n");
	return (minishell);
}*/

/*
J'ai 2 methodes pour echo
1- parsing

le parsing faire la pluspart du boulot var etc et je print juste le message avec -n ou non

2- exec

je recupere se que j'ai deja et je decortique l'arguments pour cree la phrase 
-n au debut ou pas
et mettre les var $ a la main en les cherchant dans l'envp print la val a l'emplacement
si var introuvable skip 

*/

void	ms_echo(t_parser *data)
{
	//redir
	//close()

	if (data->command_table->command->arguments != NULL)
		printf("ECHO sans \n");
	printf("ECHO\n");
	data->cmd_builtin++;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 11:00:02 by tcavaill          #+#    #+#             */
/*   Updated: 2024/07/19 11:00:02 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_command(char *command_text, char *arguments)
{
	t_command	*command;

	command = malloc(sizeof(t_command)); 
	if (command == NULL)
		return (NULL);
	command->command = command_text;
	command->arguments = arguments;
	command->cmd_ex = NULL;//j'ai change ici free
	return (command);
}

void	print_command(t_command *command)
{
	printf("{\n");
	printf("\tcommand   : %s\n", command->command);
	printf("\targuments : %s\n", command->arguments);
	printf("}\n");
}

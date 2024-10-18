/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:28:10 by tcavaill          #+#    #+#             */
/*   Updated: 2024/07/19 10:28:10 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_table	*new_table(void)
{
	t_command_table	*table;

	table = malloc(sizeof(t_command_table));
	table->command = NULL;
	table->next = NULL;
	return (table);
}

void add_command(t_command_table *command_table, t_command *command)
{
	t_command_table	*temp;

	temp = command_table;
	if (command_table->command == NULL)
	{
		command_table->command = command;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_table();
	add_command(temp->next, command);
}

void	print_table(t_command_table *command_table)
{
	while (command_table->next != NULL)
	{
		if (command_table->command != NULL)
			print_command(command_table->command);
		command_table = command_table->next;
	}
	if (command_table->command != NULL)
		print_command(command_table->command);
}
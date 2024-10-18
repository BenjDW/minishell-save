/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:17:22 by tcavaill          #+#    #+#             */
/*   Updated: 2024/06/10 12:17:22 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	is_char_in_str(int c, char *s)
{
	size_t	s_len;

	s_len = get_str_len(s) + 1;
	while (s_len--)
		if (s[s_len] == (char)c)
			return (1);
	return (0);
}


char	*get_redirection_in(char *line)
{
	char	**separation;
	int		start;
	int		end;
	char	*redirection_in;

	if (!is_char_in_str('<', line))
		return (NULL);
	separation = split_str(line, '<');
	start = -1;
	while(separation[1][++start] && separation[1][start] == ' ')
		continue ;
	end = start;
	while(separation[1][++end] && separation[1][end] != ' ' && separation[1][end] != '>' && separation[1][end] != '<' && separation[1][end] != '|')
		continue ;
	separation[1][end] = '\0';
	redirection_in = duplicate_str(separation[1] + start);
	separation[1][end] = '\1';
	free_split_str(separation);
	return (redirection_in);
}


char	*get_redirection_out(char *line)
{
	char	**separation;
	int		start;
	int		end;
	char	*redirection_out;

	if (!is_char_in_str('>', line))
		return (NULL);
	separation = split_str(line, '>');
	start = -1;
	while(separation[1][++start] && separation[1][start] == ' ')
		continue ;
	end = start;
	while(separation[1][++end] && separation[1][end] != ' ' && separation[1][end] != '>' && separation[1][end] != '<' && separation[1][end] != '|')
		continue ;
	separation[1][end] = '\0';
	redirection_out = duplicate_str(separation[1] + start);
	separation[1][end] = '\1';
	free_split_str(separation);
	return (redirection_out);
}

















char	*skip_space(char *str)
{
	char	*new_str;

	while(*str && *str == ' ')
		str++;
	new_str = duplicate_str(str);
	return (new_str);
}

char	*ignore_redirection(char *str)
{
	char	*temp_str;
	char	*new_str;
	int		i;

	i = -1;
	while (str[++i] && str[i] != '<' && str[i] != '>')
		continue ;
	temp_str = duplicate_str(str);
	temp_str[i] = '\0';
	new_str = duplicate_str(temp_str);
	temp_str[i] = '\1';
	free(str);
	free(temp_str);
	return (new_str);
}

t_command_table	*get_command_table(char *line)
{
	t_command_table	*command_table;
	char			**separation;
	int				i;
	char			*command_text;
	char			**command_split;

	command_table = new_table();
	if (!is_char_in_str('|', line))
	{
		command_text = skip_space(line);
		command_text = ignore_redirection(command_text);
		if (!is_char_in_str(' ', command_text))
			add_command(command_table, new_command(command_text, NULL));
		else
		{
			command_split = split_str(command_text, ' ');
			add_command(command_table, new_command(duplicate_str(command_split[0]), join_str(command_split + 1, ' ')));
			free_split_str(command_split);
		}
		return (command_table);
	}
	separation = split_str(line, '|');

	i = -1;
	while(separation[++i])
	{
		command_text = skip_space(separation[i]);
		command_text = ignore_redirection(command_text);
		if (!is_char_in_str(' ', command_text))
			add_command(command_table, new_command(command_text, NULL));
		else
		{
			command_split = split_str(command_text, ' ');
			add_command(command_table, new_command(duplicate_str(command_split[0]), join_str(command_split + 1, ' ')));
			free_split_str(command_split);
		}
	}

	free_split_str(separation);
	return (command_table);
}

void	print_split(char **split_str)
{
	while(*split_str)
		printf("%s, ", *split_str++);
	printf("\n");
}

void	print_parser(t_parser *parser)
{
	printf("{\n");
	printf("\tredirection in  : %s\n", parser->redirection_in);
	printf("\tredirection out : %s\n", parser->redirection_out);
	printf("{\n");
	print_table(parser->command_table);
}

t_minishell	parser(t_minishell minishell, char *line, t_parser *parser)
{
	parser->redirection_in = get_redirection_in(line);
	parser->redirection_out = get_redirection_out(line);
	parser->command_table = get_command_table(line);
	print_parser(parser);
	
	// TODO : FREE
	hub_exec(parser);
	return (minishell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:55:55 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/14 02:55:55 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_minishell	ms_export(t_minishell minishell, char **words)
{
	(void)words;
	return (minishell);
}*/

void	ms_export(t_parser *data, int i, char **str)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (data->index + data->cmd_builtin != data->size_li - 1)
	{
		dup2(data->pipefd[data->index + data->cmd_builtin + 1][1], 1);
		close(data->pipefd[data->index + data->cmd_builtin + 1][1]);
	}
	if (data->command_table->command->arguments == NULL)
		printexport(data, -1, 0);
	else
	{
		str = split_str(data->command_table->command->arguments, ' ');
		while (str[++i] != NULL)
		{
			if (verif_argexport(str[i], -1, data) == 2)
				modif_var(data, str[i], -1);
			if (verif_argexport(str[i], -1, data) == 0) //format var=value 
				setvar(data, str[i]);
		}
		free_split_str(str);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	data->cmd_builtin++;
}

int	verif_argexport(char *arg, int i, t_parser *data)
{
	int	j;

	if (ft_strlen(arg) < 3)
		return (1);
	while (data->envp[++i] != NULL)
	{
		j = -1;
		while (data->envp[i][++j] != '=')
		{
			if (data->envp[i][j] != arg[j])
				break ;
		}
		if (arg[j] == '=' && data->envp[i][j] == '=' && arg[j +1] != '\0')
			return (2);
	}
	i = -1;
	while (arg[++i] != '\0')
	{
		if (arg[i] == '=' && arg[i + 1] != '\0') //numeric seulement ?
			return (0);
	}
	return (1);
}

void	printexport(t_parser *data, int i, int sign)
{
	int		j;
	char	**cpy;
	//implemente le tri en ascii
	cpy = tri_ex(data, -1, 0, NULL);
	while (cpy[++i] != NULL)
	{
		j = -1;
		sign = 0;
		printf("declare -x ");
		while (cpy[i][++j] != '\0')
		{
			if (cpy[i][j] != '=' && sign == 0)
				printf("%c", cpy[i][j]);
			else if (cpy[i][j] == '=' && sign == 0)
			{
				sign++;
				printf("=\"");
			}
			else
				printf("%c", cpy[i][j]);
		}
		printf("\"\n");
	}
	free_split_str(cpy);
}

//fonction censé tri l'env par ordre ascci pour etre print
char	**tri_ex(t_parser *data, int i, int j, char **cpy)
{
	int	size;

	size = 0;
	while (data->envp[size] != NULL)
		size++;
	cpy = (char **)malloc(sizeof (char *) * (size + 1));
	while (++i < size)
	{
		cpy[j] = ft_strdup(data->envp[i]);
		j++;
	}
	cpy[j] = NULL;
	bubble_sort(cpy);
	return (cpy);
}

void	bubble_sort(char **array)
{
	int		i;
	int		j;
	char	*temp;
	int		size;

	size = 0;
	while (array[size] != NULL)
		size++;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (compare(array[j], array[j + 1]) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	i = -1;
}

void	setvar(t_parser *data, char *str)
{
	int		len;
	int		i;
	char	**cpy;

	i = -1;
	len = 0;
	while (data->envp[len] != NULL)
		len++;
	cpy = (char **)malloc(sizeof (char *) * (len + 2));
	while (data->envp[++i] != NULL)
		cpy[i] = ft_strdup(data->envp[i]);
	cpy[i] = ft_strdup(str);
	cpy[i + 1] = NULL;
	free_split_str(data->envp);
	data->envp = cpy;
	//ms_env(data, NULL, );//pour verifie
}

void	modif_var(t_parser *data, char *val, int i)
{
	int	j;

	while (data->envp[++i] != NULL)
	{
		j = -1;
		while (data->envp[i][++j] != '=')
		{
			if (data->envp[i][j] != val[j])
				break ;
		}
		if (val[j] == '=' && data->envp[i][j] == '=' && val[j+1] != '\0')
			break ;
	}
	data->envp = suprlineenvp(data, i);
	setvar(data, val);
}

char	**suprlineenvp(t_parser *data, int i)/* int sign, */
{
	int		len;
	int		tlen;
	int		k;
	char	**cpy;

	tlen = 0;
	while (data->envp[tlen] != NULL)
		tlen++;
	cpy = (char **)malloc(sizeof (char *) * (tlen));
	len = -1;
	k = 0;
	while (data->envp[++len] != NULL)
	{
		//printf("test suprline %s\n", data->envp[len]);
		if (len == i && len != tlen - 1) //ligne a supr
			len++;
		else if (len == tlen - 1)
			break ;
		cpy[k] = ft_strdup(data->envp[len]);
		k++;
	}
	cpy[k] = NULL;
	free_split_str(data->envp);
	return (cpy);
}

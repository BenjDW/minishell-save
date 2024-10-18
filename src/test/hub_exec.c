/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hub_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 09:10:52 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/18 02:35:21 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction central manage les données
void	hub_exec(t_parser *data)
{
	pid_t	*pid;

	printf("start hub\n");
	if (init_ex_utils(data) == 1)
	{
		data->handle_error = 1;
		return ;
	}
	pid = malloc(sizeof (pid_t) * data->size_li);
	exec(data, pid);
	printf("finish total\n");
	//freetab_path(data);
	free_ex(data);
	printf("apres free_ex\n");
	return ;
}

int	init_ex_utils(t_parser *data)
{
	data->handle_error = 0;// dois recup le code erreur
	data->size_li = ft_size(&data->command_table);
	data->index = 0;
	init_pipe(data, 0);
	data->index = 0;
	data->tab_path = NULL;
	data->status = 0;
	data->cmd_builtin = 0;
	data->ipid = 0;
	return (0);
}

void	free_all(t_parser *data)
{
	free_split_str(data->envp);
	//free(data);
	//free_parser(data, -1);
}

void	free_ex(t_parser *data)
{
	int	i;

	i = -1;
	if (data->tab_path != NULL)
		freetab_path(data);
	if (data->pipefd != NULL)
	{
		while (++i != data->size_li)
			free(data->pipefd[i]);
		free(data->pipefd);
		data->pipefd = NULL;
	}
	//probleme de free dans les enfants ?
}

//attention pour les free , condition d'arret
int	init_pipe(t_parser *data, int i)
{
	data->pipefd = (int **)malloc(sizeof(int *) * data->size_li);
	//attention + 1 regle segfault
	if (data->pipefd == NULL)
		printf("erreur malloc\n");
	while (i != data->size_li)
	{
		data->pipefd[i] = (int *)malloc(sizeof(int *) * 2);
		if (data->pipefd[i] == NULL)
			return (printf("erreur malloc I\n"), 1);
		if (pipe(data->pipefd[i]) == -1)
			return (write(1, "error pipe\n", 11), 1);
		i++;
	}
	return (0);
}

void	init_envp(t_parser *data, char **env)
{
	int	i;
	int	len;

	i = -1;
	len = 0;
	while (env[len] != NULL)
		len++;
	data->envp = (char **)malloc(sizeof (char *) * (len + 1));
	while (env[++i] != NULL)
		data->envp[i] = ft_strdup(env[i]);
	data->envp[len] = NULL;
}

int	exec(t_parser *data, pid_t *pid)
{
	while ((data->index + data->cmd_builtin) != data->size_li)
	{
		data->tab_path = finding_path(data);
		if (cmd_builtin(data, &pid[data->index + data->cmd_builtin]) == 1) // si cmd == builtin , exec builtin -> next
			data->command_table = data->command_table->next;
		else
		{
			pid[data->index] = fork();
			if (pid[data->index] == 0)
			{
				if (children(data) == 1)
					return (free_ex(data), set_error(data, 52), exit(0), 0);
				return (set_error(data, 0), 0);
			}
			data->command_table = data->command_table->next;
			data->index++;
		}
		usleep(10);
	}
	close_pipe(data);
	while (data->index-- != 0)
		waitpid(pid[data->index], &data->status, 0);
	return (free(pid), 0);
}

//prendre en compte le cas ou erreur cmd , sa dois effectue les autres cmd mais pas les affiches ou dans des redir si y a

//process pour les exec built-in
//probleme struct en attente de la struc final 
//ATTENTION les built-in qui modifie l'env parent sont a faire dans le parent
//Les built-in qui ne modifie pas doivent etre fait dans un fork pour transmettre via le pipe
int	cmd_builtin(t_parser *data, pid_t *pid)
{
	if (compare(data->command_table->command->command, "env") == 0)
		if (ms_env(data, pid, -1) == 0)
			return (1);
	if (compare(data->command_table->command->command, "pwd") == 0)
		if (ms_pwd(data, pid) == 0)
			return (1);
	if (compare(data->command_table->command->command, "cd") == 0)
		if (ms_cd(data) == 0) /*, pid*/
			return (1);
	if (compare(data->command_table->command->command, "export") == 0)
		return (ms_export(data, -1, NULL), 1);
	if (compare(data->command_table->command->command, "unset") == 0)
		return (ms_unset(data, -1, -1, NULL), 1);
	if (compare(data->command_table->command->command, "exit") == 0)
		return (ms_exit(data, -1), 1);
	if (compare(data->command_table->command->command, "echo") == 0)// complique a finir sans la structure avec le parsing
		return(ms_echo(data), 1);
	return (0);
}

int	children(t_parser *data)
{
	printf("start children %d\n", (data->index + data->cmd_builtin));
	if (pipe_redir(data) == -1) //	need to handle the multi fd
		return (set_error(data, 126), perror("fd"), 1);
	close_pipe_child(data);
	data->validpath = checkaccess(data->command_table->command->command,
			data->tab_path);
	if (data->validpath == NULL)
		return (set_error(data, 127), perror("access"), 1);
	init_cmdtab(data);
	printf("finish children %d\n", (data->index + data-> cmd_builtin));
	execve(data->validpath, data->command_table->command->cmd_ex, data->envp);
	return (set_error(data, 1), 1);
}

int	compare(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	init_cmdtab(t_parser *data)
{
	char	*str;

	str = NULL;
	if (data->command_table->command->arguments != NULL)
	{
		str = joinspace(data->command_table->command->command,
				data->command_table->command->arguments);
		data->command_table->command->cmd_ex = split_str(str, ' ');
	}
	else
		data->command_table->command->cmd_ex
			= split_str(data->command_table->command->command, ' ');
	if (str != NULL)
		free(str);
}

char	*checkaccess(char *cmd, char **split_path)
{
	int		i;
	char	*test;

	i = 0;
	if (cmd[i] == '.' && cmd[i + 1] == '/')
		return (cmd);
	while (split_path[i] != NULL)
	{
		//printf("path : %s\n", split_path[i]);
		test = ft_strjoin(split_path[i], cmd, 1);
		if (access(test, X_OK) == 0)
			return (test);
		free(test);
		i++;
	}
	return (NULL);
}

int	pipe_redir(t_parser *data)
{
	if (data->redirection_in != NULL)
	{
		data->fd_in = open(data->redirection_in, O_RDONLY);
		if (data->fd_in < 0)
			return (-1);//perror() fd introuvable
		dup2(data->fd_in, 0);
		close(data->fd_in);
	}
	else//pas de redirect
	{
		if ((data->index + data->cmd_builtin) == 0)
		{
			dup2(STDIN_FILENO, 0);
		}
		else
			dup2(data->pipefd[data->index + data->cmd_builtin][0], 0);
		close(data->pipefd[data->index + data->cmd_builtin][0]);
	}
	if (redir_out(data) == -1)
		return (-1);
	return (0);
}

int	redir_out(t_parser *data)
{
	if (data->redirection_out != NULL)
	{
		data->fd_out = open(data->redirection_out,
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
		//dans l'autre cas open , ou ont efface tout ou a la suite comme echo
		//gere les differente fleche entre sortie , int ... , gere le heredoc
		if (data->fd_out < 0)
			return (-1);
		dup2(data->fd_out, 1);
		close(data->fd_out);
	}
	else
	{
		if (data->index + data->cmd_builtin + 1 != data->size_li
			&& data->size_li != 1)
		{
			if (dup2(data->pipefd[data->index + data->cmd_builtin + 1][1]
				, 1) == -1)
				perror("dup2 failed for pipefd write end");
		}
		else if (data->index + data->cmd_builtin == data->size_li
			|| data->size_li == 1)
			dup2(STDOUT_FILENO, 1);
		close(data->pipefd[data->index + data->cmd_builtin][1]);
	}
	return (0);
}

/*void close_pipe_child(t_parser *data) 
{
	int i = -1;

	while (++i < data->size_li) 
	{
		if (i != (data->index + data->cmd_builtin)) 
		{
			close(data->pipefd[i][0]);
			close(data->pipefd[i][1]);
			printf("i != index\n");
		}
		else
		{
			if ((data->index + data->cmd_builtin) == 0)
			{
				printf("i == 0\n");
				//close(data->pipefd[i][0]);
				close(data->pipefd[i][1]);
			}
		}
	}
}*/

void	close_pipe_child(t_parser *data)
{
	int	i;

	i = -1;
	while (++i < data->size_li)
	{
		if (i != data->index)
		{
			close(data->pipefd[i][0]);
			close(data->pipefd[i][1]);
			printf("Fermeture des pipes inutilisés pour i = %d\n", i);
		}
		else
		{
			if ((data->index + data->cmd_builtin) == 0)
			{
				printf("Fermeture de la sortie du pipe de la première commande\n");
				close(data->pipefd[i][1]);
			}
			else if ((data->index + data->cmd_builtin) == data->size_li - 1)
			{
				printf("Fermeture de l'entrée du pipe de la dernière commande\n");
				close(data->pipefd[i][0]);
			}
		}
	}
}

void	close_pipe(t_parser *data)
{
	int	i;

	i = 0;
	printf("close pipe\n");
	if (data->redirection_in != NULL)
	{
		close(data->fd_in);
	}
	if (data->redirection_out != NULL)
	{
		close(data->fd_out);
	}
	while (i != data->size_li)
	{
		printf("fermeture pipe parent %d entre - sortie\n", i);
		close(data->pipefd[i][0]);
		close(data->pipefd[i][1]);
		i++;
	}
}

char	**finding_path(t_parser *data)
{
	int		i;
	char	*path;
	char	**tab_path;

	i = 0;
	if (data->tab_path != NULL)
		freetab_path(data);
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], 4) == 1)
			break ;
		i++;
	}
	//TODO : gere le cas ou le path est supr
	path = ft_strdup(data->envp[i] + 5);
	tab_path = split_str(path, ':');
	free(path);
	return (tab_path);
}

void	freetab_path(t_parser *data)
{
	int	i;

	i = 0;
	while (data->tab_path[i] != NULL)
	{
		free(data->tab_path[i]);
		i++;
	}
	free(data->tab_path);
}

char	*joinspace(char *s1, char *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*dup;

	i = 0;
	j = 0;
	dup = (char *)malloc((sizeof (char)) * (ft_strlen(s1) + ft_strlen(s2)) + 2);
	while (s1[i] != '\0')
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = ' ';
	i++;
	while (s2[j] != '\0')
	{
		dup[i] = s2[j];
		i++;
		j++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_strncmp(char *str, int n)
{
	int		i;
	char	*cmp;

	cmp = "PATH";
	i = 0;
	while ((str[i] != '\0' && cmp[i] != '\0') || i < n)
	{
		if (str[i] == cmp[i])
			i++;
		else if (str[i] != cmp[i])
			return (0);
	}
	return (1);
}

int	ft_size(t_command_table **li)
{
	int				i;
	t_command_table	*temp;

	i = 0;
	temp = *li;
	while (temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

void	set_error(t_parser *data, int code)
{
	data->handle_error = code;
	printf("val error return %d\n", data->handle_error);
}

int	ft_atoi(char *ptr)
{
	int			i;
	long long	result;
	int			sign;

	i = 0;
	result = 0;
	sign = 1;
	while ((ptr[i] >= 9 && ptr[i] <= 13) || ptr[i] == 32)
		i++;
	if (ptr[i] == '+' || ptr[i] == '-')
	{
		if (ptr[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (ptr[i] != '\0' && (ptr[i] >= 48 && ptr[i] <= 57))
	{
		result = result * 10 + (ptr[i] - '0');
		i++;
	}
	return (result * sign);
}

//FONCTION DE PARSING POUR ESTEBAN 

//FONCTION POUR LES $VAR
char	*str_var(t_parser *data, char *str, char *cpy, int i)
{
	int	j;

	j = -1;
	cpy = (char *)malloc((sizeof (char)) * (1500));
	while (str[++i] != '\0')
	{
		if (str[i] == '\'')
			replace_cot(str, cpy, &j, &i); //simple '$USERS '
		else if (str[i] == '\"')
		{
			if (double_cot((str + i + 1), cpy, &j, data) == 1) //double "$USERS "
				while (str[++i] != '\"' && str[i] != '\0')
					;
		}
		else if (str[i] == '$' && str[i + 1] != '\0')
		{
			if (replace_var((str + i + 1), cpy, &j, data) == 1) //aucune $USERS 
				while (str[i] != ' ' && str[i] != '\0' && str[i] != '	')
					i++;
			cpy[++j] = str[i];
		}
		else
			cpy[++j] = str[i];
	}
	return (copie_final(str, cpy, &j), str);
}

void	copie_final(char *str, char *cpy, int *j)
{
	cpy[++(*j)] = '\0';
	free(str);
	str = ft_strdup(cpy);
	free(cpy);
}

void	replace_cot(char *str, char *cpy, int *j, int *i)
{
	printf("replace_cot\n");
	cpy[++*j] = str[*i]; //copie la premiere
	while (str[(++*i)] != '\'' && str[(*i)] != '\0') // Copier tout entre les guillemets simples
		cpy[++(*j)] = str[(*i)];
	cpy[++(*j)] = str[(*i)]; // Fermer les guillemets
}

int	double_cot(char *str, char *cpy, int *j, t_parser *data)
{
	int	i;

	i = 0;
	printf("double cot\n");
	cpy[(++*j)] = '\"';
	while (str[i] != '\0' && str[i] != '\"')
	{
		if (str[i] == '$')
		{
			replace_var((str + i + 1), cpy, j, data);
			while (str[i] != ' ' && str[i] != '	' && str[i] != '\0'
				&& str[i] != '\"')
				i++;
		}
		if (str[i] != '$')
			cpy[(++*j)] = str[i];
		i++;
	}
	cpy[(++*j)] = str[i];
	return (1);
}

int	replace_var(char *str, char *cpy, int *j, t_parser *data)
{
	int	i;
	int	l;

	i = 0;
	l = -1;
	while (str[i] != ' ' && str[i] != '\"' && str[i] != '\0')
		i++;
	if (i == 1 && str[i - 1] == '?')
		return (var_error(cpy, j, NULL, data), 1);
	while (data->envp[++l] != NULL)
	{
		if (compare_str(str, data->envp[l], i) == 0)
			break ;
	}
	if (data->envp[l] == NULL)
		return (1);//skip la var
	i = -1;
	if (data->envp[l] != NULL)
		while (data->envp[l][++i] != '\0')
			cpy[++(*j)] = data->envp[l][i];
	return (1);
}

void	var_error(char *cpy, int *j, char *error, t_parser *data)
{
	int	i;

	i = 0;
	error = ft_itoa(data->handle_error);
	while (error[i] != '\0')
	{
		cpy[++(*j)] = error[i];
		i++;
	}
	free(error);
}

int	get_dec(int n)
{
	int	i;

	i = 1;
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n / 10 != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*temp;
	int		len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len = get_dec(n);
	temp = malloc((sizeof (char)) * (len) + 1);
	if (temp == NULL)
		return (NULL);
	temp[len] = '\0';
	len--;
	if (n < 0)
	{
		temp[0] = '-';
		n *= -1;
	}
	if (n == 0)
		return (ft_strdup("0"));
	while (len >= 0 && n > 0)
	{
		temp[len] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	return (temp);
}

//FONCTION POUR LES REDIRECTIONS open etc
/*
void	open_redir()
{
	while()
	{
		data->fd_in = open(okd, O_WRONLY, 0777);
		close(data->fd_in);
	}
	//dernier fd dois etre mis dans data->command->redir_in sans etre ouvert
	while()
	{
		data->fd_out = open(data->redirection_out
			, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		close(data->fd_out)
	}
	//dernier fd dois etre mis dans data->command->redir_out sans etre ouvert
}
*/
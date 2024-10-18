/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcavaill < tcavaill@student.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:05:09 by tcavaill          #+#    #+#             */
/*   Updated: 2024/06/10 12:05:09 by tcavaill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
#include <fcntl.h>

# define FALSE 0
# define TRUE 1

# define MAX_WORD_LENGTH 99

# define NO_ERROR 0
# define ERROR_NULL_LINE 1

typedef struct s_minishell
{
	int	flag_exit;
	int	flag_error;
}	t_minishell;

typedef struct s_command
{
	char	*command;
	char	*arguments;
	char	**cmd_ex;
} t_command;

typedef struct s_command_table
{
	t_command				*command;
	struct	s_command_table	*next;
} t_command_table;

typedef struct s_parser
{
	char			*redirection_in;
	char			*redirection_out;
	char			**envp; //contient l'env parent
	char			**tab_path;//dois contenir les chemins d'access cmd
	char			*validpath;
	int				**pipefd;//entre sortie standard
	int				status;
	int				fd_in;
	int				fd_out;
	int				index;//pos list
	int				size_li;//len de la list cmd
	int				cmd_builtin;//cmd builtin sans fork
	int				handle_error;//printf le return d'erreur en cas de echo $?
	int				ipid;
	//env ici sa peut pose des problemes peu etre dans l'execution ?
	t_command_table	*command_table;
} t_parser;

int			compare_str(const char *s1, const char *s2, int n);

// utils/get_str_len.c
int			get_str_len(char *str);

// utils/duplicate_str.c
char		*duplicate_str(char *s);

// utils/join.c
char		*join_str(char **table, char c);

// utils/split.c
char		**split_str(char const *s, char c);
void		free_split_str(char **words);

/*
t_minishell	ms_cd(t_minishell minishell, char **words);
t_minishell	ms_echo(t_minishell minishell, char **words);
t_minishell	ms_env(t_minishell minishell, char **words);
t_minishell	ms_exit(t_minishell minishell, char **words);
t_minishell	ms_export(t_minishell minishell, char **words);
t_minishell	ms_pwd(t_minishell minishell, char **words);
t_minishell	ms_unset(t_minishell minishell, char **words);
*/

// parser/command.c
t_command		*new_command(char *command, char *arguments);
void			print_command(t_command *command);

// parser/command_table.c
t_command_table	*new_table(void);
void			add_command(t_command_table *command_table, t_command *command);
void			print_table(t_command_table *command_table);


// parser/parser.c
t_minishell		parser(t_minishell minishell, char *line, t_parser *parser);


//exec/hub_exec.c/init
void	hub_exec(t_parser *data);
int		init_ex_utils(t_parser *data);
void	init_envp(t_parser *data, char **env);
int		init_pipe(t_parser *data, int i);

//exec/hub_exec/exec
int		exec(t_parser *data);
int		children(t_parser *data);
void	init_cmdtab(t_parser *data);
char	*joinspace(char *s1, char *s2);
int		pipe_redir(t_parser *data);
void	close_pipe(t_parser *data);
char	**finding_path(t_parser *data);
char	*checkaccess(char *cmd, char **split_path);
void	freetab_path(t_parser *data);
char	*ft_strjoin(char *s1, char *s2, int slash);
char	*ft_strjoinslash(char *s1, char *s2);
int		ft_size(t_command_table **li);
void	set_error(t_parser *data, int code);
void	close_pipe_child(t_parser *data);
int		compare(const char *s1, const char *s2);
int		redir_out(t_parser *data);

//exec//cmd_builtin
int		cmd_builtin(t_parser *data, pid_t *pid);
int		ms_env(t_parser *data, pid_t *pid, int i);
int		ms_pwd(t_parser *data, pid_t *pid);
int		ms_cd(t_parser *data/*, pid_t *pid*/);
void	ms_export(t_parser *data, int i, char **str);
int		verif_argexport(char *arg, int i, t_parser *data);
void	printexport(t_parser *data, int i, int sign);
void	setvar(t_parser *data, char *str);
void	modif_var(t_parser *data, char *val, int i);
char	**suprlineenvp(t_parser *data,/* int sign, */int i);
int		ms_unset(t_parser *data, int i, int j, char ** multi);
int		search_var(char *search, char *senvp);
char	**tri_ex(t_parser *data, int i, int j, char **cpy);
void	bubble_sort(char **array);
int		ms_exit(t_parser *data, int i);
void	free_ex(t_parser *data);
void	free_all(t_parser *data);
void	free_exit(t_parser *data);

char	*str_var(t_parser *data, char *str, char *cpy, int i);
void	replace_cot(char *str, char *cpy, int *j, int *i);
int		double_cot(char *str, char *cpy, int *j, t_parser *data);
int		replace_var(char *str, char *cpy, int *j, t_parser *data);

// free list fonctionne avec erreur
void	free_parser(t_parser *data, int i);
void	free_command_table(t_command_table *table);
void	free_command(t_command *cmd);

//utils/ex
int		ft_strncmp(char *str, int n);
char	*ft_strdup(char *s);
int		ft_strlen(char *str);
void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(char *ptr);

#endif
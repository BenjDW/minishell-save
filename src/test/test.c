/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 06:03:51 by bde-wits          #+#    #+#             */
/*   Updated: 2024/10/11 05:42:01 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_data
{
	char	**envp;
}			t_data;	

char	**suprlineenvp(t_data *data,/* int sign, */int i);

void	freearg(char **tabarg)
{
	int	i;

	i = 0;
	while (tabarg[i] != NULL)
	{
		free(tabarg[i]);
		i++;
	}
	free(tabarg);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while(str[i] != '\0')
		i++;
	return (i);
}

int	compare_str(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup(char *s)
{
	char	*dup;
	int		i;
	int		ls;

	i = 0;
	ls = ft_strlen(s);
	dup = (char *)malloc((sizeof(char)) * (ls + 1));
	if (dup == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*fonctinne mais dois pourvoir gere les redirections
comment on utilsera les pipefd dans le structure ou envoyer directement ?*/
//char *getcwd(char *buf, size_t size);
void	ms_pwd(/*t_struc struc, int *pipefd*/)
{
	char	path[1028];

	/*dup2(struc->sourcefd, 0);// node indépendant ?
	dup2(struc->outpost, 1);
	if (struc->redirections == 1)// node donnée
	{
		//dup(struc->entrée);  si predefinie dans le node
		//dup(struc->sortie redirect fd etc);
		//dup2(pipefd[0], 0);  si manuel
		//dup2(pipefd[1], fd ou struc->redirect);
	}*/
	getcwd(path, 1028);
	printf("%s\n", path);
	//close(struc->sourcefd);
}

/*char	*strcatslash(char *src, char *dest)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = (char*)malloc(sizeof (char) * (ft_strlen(src) + ft_strlen(dest)) + 2);
	while (src[i] != '\0')
	{
		result[i] = src[i];
		i++;
	}
	result[i] = '/';
	i++;
	while (dest[j] != '\0')
	{
		result[i] = dest[j];
		i++;
		j++;
	}
	result[i] = '\0';
	printf("%s\n", result);
	return (result);
}*/

/*void	ms_cd(char **argv)
{
	//char	path[1028];
	//char	*pathcd;

	if (argv[2] == NULL)
	{
		printf("error argument\n");
	}
	//getcwd(path, 1028);
	//pathcd = strcatslash(path, argv[2]);
	if (chdir(argv[2]) != 0)
	{
		//free(pathcd);
		perror("cd\n");
	}
	else
	{
		printf("ok\n");
		//free(pathcd);
	}
}*/

void ms_cd(char **argv)
{
    if (argv[2] == NULL)
	{
        printf("error: no argument\n");
        return;
    }
    // Change directory
    if (chdir(argv[2]) != 0)
        perror("cd");
	else
	{
        printf("Directory changed to %s\n", argv[2]);
		ms_pwd();
    }
}

void	ms_exit()
{
	exit(0);
}

void	init_data(t_data *data, char **env)
{
	int	i;
	int	len;
	//char **copyenv;

	i = -1;
	len = 0;
	while (env[len] != NULL)
		len++;
	data->envp = (char **)malloc(sizeof (char *) * (len + 1));
	while (env[++i] != NULL)
		data->envp[i] = ft_strdup(env[i]);
	data->envp[len] = NULL;
	printf("START DATA ENVP\n");
	i = 0;
	while(data->envp[i] != NULL)
	{
		printf("%s\n", data->envp[i]);
		i++;
	}
	printf("FINISH DATA ENVP %d i %d len\n", i, len);
}

int		ms_env(t_data *data)
{
	int i;

	i = 0;
	if (pipe_redir(data) == -1)
		return (set_error(data, 126), perror("fd"), 1);
	if (data->command_table->command->arguments != NULL)
		return (write(2, "env: invalid option\n", 15), 1);
	while (data->envp[i] != NULL)//struc->envp env modifie  ou egal env
	{
		ft_putstr_fd(data->envp[i], 1);
		//printf("\n");
		i++;
	}
	return (0);
}


//dois gere les redirections aussi
//2 versions je sais pas si on a le droit , l'une avec une variable global actualise environ et l'autre non alloc realloc
/*void	ms_export(char **argv, char **env)
{
	if (putenv(argv[2]) != 0)
        perror("export");
	ms_env(env);
}*/

//EXPORT fonction //
//verif dois verifie si la var existe deja dans l'env
int	verif_argexport(char *arg, int i, t_data *data)
{
	int	j;

	if (ft_strlen(arg) < 3)
		return (1);
	while(data->envp[++i] != NULL)
	{
		j = -1;
		while(data->envp[i][++j] != '=')
		{
			if (data->envp[i][j] != arg[j])
				break;
		}
		if (arg[j] == '=' && data->envp[i][j] == '=' && arg[j+1] != '\0')
			return (2);
	}
	i = -1;
	while(arg[++i] != '\0')
	{
		if (arg[i] == '=' && arg[i + 1] != '\0' )
			return (0);
	}
	return (1);
}
//dois pouvoir redirect
void	printexport(t_data *data, int i, int sign)
{
	int	j;

	while (data->envp[++i] != NULL)
	{
		j = -1;
		sign = 0;
		printf("declare -x ");
		while (data->envp[i][++j] != '\0')
		{
			if (data->envp[i][j] != '=' && sign == 0)
				printf("%c", data->envp[i][j]);
			else if (data->envp[i][j] == '=' && sign == 0)
			{
				sign++;
				printf("=\"");
			}
			else
				printf("%c", data->envp[i][j]);
		}
		printf("\"\n");
	}
	printf("len %d a verif\n", i);
}

//set la variable et actualise tout les envp
void	setvar(t_data *data, char **argv)
{
	int	len;
	int	i;
	char	**cpy;

	i = -1;
	len = 0;
	while(data->envp[len] != NULL)
		len++;
	cpy = (char **)malloc(sizeof (char *) * (len + 2));
	while (data->envp[++i] != NULL)
		cpy[i] = ft_strdup(data->envp[i]);
	cpy[i] = ft_strdup(argv[2]);
	cpy[i + 1] = NULL;
	freearg(data->envp);
	data->envp = cpy;
	ms_env(data);
}

void	modif_var(t_data *data, char **argv, int i)
{
	int	j;
	//int	tempj;
	
	while(data->envp[++i] != NULL)
	{
		j = -1;
		while(data->envp[i][++j] != '=')
		{
			if (data->envp[i][j] != argv[2][j])
				break;
		}
		if (argv[2][j] == '=' && data->envp[i][j] == '=' && argv[2][j+1] != '\0')
			break;
	}
	//tempj = j;
	data->envp = suprlineenvp(data, i);
	setvar(data, argv);
}

void	ms_export(t_data *data, char **argv)
{
//	int	i;

//	i = 0;
	/*dup2(data->source, 0)
	if (data->redirect != NULL)
		dup2(data->redirect, 1);
	else
		dup2(data->sortie, 1) sortie egal sortie ou redirection ou :
	*/
	if (argv[2] == NULL)
	{
		//while (data->envp[i++] != NULL)
		printexport(data, -1, 0);
			//printf("declare -x %s\n", data->envp[i]);//print declare -x var="value" rajoute les " "
	}
	else
	{
		if (verif_argexport(argv[2], -1, data) == 2)
			modif_var(data, argv, -1);
			//printf("sa passe\n");
		if (verif_argexport(argv[2], -1, data) == 0)//format var=value 
			setvar(data, argv);
	}
	//close(data->source, 0); close se qu'on utilsera pas
}

//supprime une variable d'environnement
/*void	ms_unset(char **argv, char **env)
{
	if (unsetenv(argv[2]) != 0)
		perror("unset");
	//ms_env(env); pour test la dif , aucune incidence sur l'env global
}*/

/*int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	unsigned char	ts1;
	unsigned char	ts2;

	i = 0;
	if (n == 0 || !s1 || !s2)
		return (-1);
	while (i < n - 1 && (s1[i] != '\0'  && s2[i] != '\0'))
	{
		ts1 = (unsigned char)s1[i];
		ts2 = (unsigned char)s2[i];

		if (ts1 != ts2)
			return (ts1 - ts2);
		i++;
	}
	ts1 = (unsigned char)s1[i];
	ts2 = (unsigned char)s2[i];
	return (ts1 - ts2);
}*/

// UNSET fonction //
// dois realloc sur tout les nodes ? ou seulement le principal
char	**suprlineenvp(t_data *data,/* int sign, */int i)
{
	int	len;
	int k;
	char **cpy;

	len = 0;
	while (data->envp[len] != NULL)
		len++;
	//if (sign == 1)/*supr la ligne de l'envp*/
	//{
	cpy = (char**)malloc(sizeof (char *) * (len));
	len = -1;
	k = 0;
	while(data->envp[++len] != NULL)
	{
		if (len == i/* && sign == 1*/)//ligne a supr
			len++;
		cpy[k] = ft_strdup(data->envp[len]);
		k++;
	}
	cpy[k] = NULL;
	//}
	freearg(data->envp);
	return (cpy);
}

int	search_var(char *search, char *senvp)
{
	int	i;

	i = 0;
	while (search[i] != '\0')
	{
		if (search[i] != senvp[i])
			return (-1);
		i++;
	}
	if (senvp[i] == '=')
		return (0);
	return (-1);
}

void	ms_unset(char **argv, t_parser *data, int i)
{
	while(data->envp[i] != NULL)
	{
		if (search_var(argv[2], data->envp[i]) == 0)
			break;
		i++;
	}
	if (data->envp[i] != NULL)//1 = supr
		data->envp = suprlineenvp(data,/* 1,*/ i);
	else
		printf("error missing var %s\n", argv[2]);
		//ft_error(msg error)
	ms_env(data);//pour verif
}

// COMMANDE ECHO


/*void	apply_redir(char **argv, t_data *data, int i)
{
	int	fd;
	
	fd = open(argv[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (write(1, "fd introuvable", 14), 0);
	dup2(pipefd[0], 0);
	dup2(fd, 1);
	close(1);
}*/

int	search_redi(char **argv, t_data *data, int i, int j)
{
	if (data != NULL)// unused parametre
		i = i;
	while (argv[++i] != NULL)
	{
		j = -1;
		while (argv[i][++j] != '\0')
		{
			if (argv[i][j] == '>')
				return (i);
		}
	}
	return (-1);// na pas trouve de redirection
}
//dois print la valeur de la variable
void	printval(char **argv, t_data *data, int i)
{
	int j;

	while(data->envp[++i] != NULL)
	{
		j = -1;
		while(data->envp[i][++j] != '=')
		{
			if (data->envp[i][j] != argv[2][j])
				break;
		}
		if (argv[2][j] == '=' && data->envp[i][j] == '=' && argv[2][j+1] != '\0')
		{
			while(data->envp[i][++j] != '\0')
				printf("%c", data->envp[i][j]);
		}
	}
}

/*void	precheck(char **argv, t_data *data)
{
	//int j;
	int	redir;

	redir = search_redi(argv, data, -1, -1);
	if (redir != -1)// il y a une redirection
	{
		if (apply_redir(argv, data, redir) == 0)
			exit (0);
	}
	return (redir + 1);//arguments suivant pour la redir
	//if (search_redi(argv, data, -1, -1) != -1)
}*/

void	ms_echo(char **argv, t_data *data)
{
	int	i;
	int	j;
	//int	redir;

	i = 1;//a.out echo
	//redir = precheck(argv,/* data, */-1);//dois verifie si il y a une redirection '>'
	while (argv[++i] != NULL)
	{
		j = -1;
		while (argv[i][++j] != '\0')
		{
			if (argv[i][j] == '$')
			{
				printval(argv, data, i);
				break;
			}
			printf("%c", argv[i][j]);
		}
		if (argv[i + 1] != NULL)
			printf(" ");
	}
	//if (data->arg == 0)//si il n'y pas de -n
	printf("\n");
	//if (redir =< 0)
	//	close_redir(argv,/* data,*/ redir);
}

/*
void	ms_echo(char *str, int sign)
{
	if (sign == 1)
		printf("%s\n", str);
	else
		printf("%s", str);
}
*/

/*void	close_redir(char **argv, int redir);
{
	close(redir);
	close(pipefd[0]);
}*/

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	init_data(&data, env);
	printf("\n");
	if (argc == 2) // il faudra faire - 1 car il n'y aura pas de a.out
	{
		if (compare_str(argv[1], "env") == 0)
			ms_env(&data);
		if (compare_str(argv[1], "pwd") == 0)
			ms_pwd();
		if (compare_str(argv[1], "export") == 0)
			ms_export(&data, argv);
	}
	if (argc >= 3)//== 3
	{
		if (compare_str(argv[1], "echo") == 0)// complique a finir sans la structure avec le parsing
			ms_echo(argv, &data);
		if (compare_str(argv[1], "cd") == 0)
			ms_cd(argv);
		if (compare_str(argv[1], "unset") == 0)
			ms_unset(argv, &data, 0);
		if (compare_str(argv[1], "export") == 0)
			ms_export(&data, argv);
	}
	freearg(data.envp);
	//ms_exit();
	return (0);
}

/* On peut cree une fonction qui pre-parametra tout les redirections entrée - sortie a appeller
	au debut de la fonction , sa affectera toute les autres interactions(fonction) ensuite
	grace au parsing on recupere les données possiblement comme dans push_swap  */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-wits <bde-wits@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 06:44:40 by bde-wits          #+#    #+#             */
/*   Updated: 2024/07/20 02:05:40 by bde-wits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int	g_sign = 0;
/*The sigaction structure is defined as something like:

           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/

void	sigint(int sig)//ctrl-C
{
	g_sign = 1;
	//printf("\ntest ctrl c\n");
	//printf("g_sign %d\n", g_sign);
	printf("\n");
	rl_on_new_line();  // Réinitialiser la ligne
    rl_replace_line("", 0);  // Effacer la ligne en cours dans `readline`
    rl_redisplay();
}

/*void	sigquit(int sig)//ctrl-/
{
	printf("\ntest ctrl /");
}*/

void	init_sign()
{
	struct sigaction sa;

	sa.sa_handler = sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;//ignore
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);
}

/*int main()
{
	init_sign();
    while (1) 
	{
        printf("shell> ");
        //fflush(stdout);
        
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // Traiter Ctrl-D (EOF)
            //printf("\nExiting shell.\n");
			printf("\n");
            break;
			//exit(0);
        }

        // Ici, on pourrait ajouter le traitement de la commande tapée
        // Pour l'instant, on se contente de l'afficher
        printf("Vous avez tapé: %s", buffer);
    }
	printf("suite des fonction\n");
    return 0;
}*/

/*int main() 
{
    init_sign();
    while (1) {
        printf("shell> ");
        //fflush(stdout); // Assurez-vous que l'invite est affichée immédiatement

        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // Traiter Ctrl-D (EOF)
            // Si le buffer est vide, cela signifie que Ctrl-D a été pressé sur une ligne vide
            if (feof(stdin)) {
                printf("\nExiting shell.\n");
                break; // Sortir de la boucle
            }
            // Sinon, Ctrl-D a été pressé au milieu de la ligne, continuez
            // (En pratique, fgets devrait gérer cela, mais nous le signalons explicitement)
            printf("\n");
            continue; // Continuer la boucle
        }

        // Supprimer le caractère de nouvelle ligne lu par fgets, s'il existe
        buffer[strcspn(buffer, "\n")] = 0;

        // Afficher la commande entrée par l'utilisateur
        printf("\nVous avez tapé: %s\n", buffer);
    }
    printf("suite des fonctions\n");
    return (0);
}*/

int main() 
{
    init_sign();

    while (1) 
	{
		g_sign = 0;
		//printf("valeur global %d\n", g_sign);
        // Lire la ligne d'entrée
        char *input = readline("shell> ");
        if (input == NULL) {
            // Traiter Ctrl-D (EOF)
            printf("\nExit\n");
            //break;
			exit(0);
        }

        // Ajouter l'entrée à l'historique
		else if (g_sign == 0)
        	add_history(input);
		//printf("valeur global fin %d\n", g_sign);
		//else if (g_sign == 1)
		//	rl_on_new_line();
        // Afficher la commande entrée par l'utilisateur
        //printf("Vous avez tapé: %s\n", input);

        // Libérer la mémoire allouée pour l'entrée
        free(input);
    }

    //printf("suite des fonctions\n");
    return (0);
}
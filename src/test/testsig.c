//testsig
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Gestionnaire pour Ctrl-C (SIGINT)
void handle_sigint(int sig) {
    printf("\n");
    printf("shell> ");
    fflush(stdout);
}

// Gestionnaire pour Ctrl-\ (SIGQUIT)
void handle_sigquit(int sig) {
    // Ne fait rien, ignore SIGQUIT
}

int main() {
    struct sigaction sa;

    // Configurer le gestionnaire pour SIGINT (Ctrl-C)
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Configurer le gestionnaire pour SIGQUIT (Ctrl-\) pour qu'il ne fasse rien
    sa.sa_handler = handle_sigquit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);

    // Boucle principale du shell
    while (1) {
        printf("shell> ");
        fflush(stdout);
        
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // Traiter Ctrl-D (EOF)
            printf("\nExiting shell.\n");
            exit(0);
        }

        // Ici, on pourrait ajouter le traitement de la commande tapée
        // Pour l'instant, on se contente de l'afficher
        printf("Vous avez tapé: %s", buffer);
    }

    return 0;
}



/*The sigaction structure is defined as something like:

           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/

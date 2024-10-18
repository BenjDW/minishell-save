#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


void	sigint()//ctrl-C
{
	printf("\n");
	rl_on_new_line();  // Réinitialiser la ligne
    rl_replace_line("", 0);  // Effacer la ligne en cours dans `readline`
    rl_redisplay();
}

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

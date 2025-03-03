#include "minishell.h"

void handle_sigint(int signum)
{
	(void)signum;
	ft_printf("\n");
	// rl_replace_line("", 0);
	rl_on_new_line();
}
void setup_signals(void)
{
	struct sigaction sa_int, sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void handle_heredoc_sigint(int signum)
{
    (void)signum;
    close(STDIN_FILENO);     
    ft_printf("\n");
    g_signal = 130;
}
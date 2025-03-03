#include "minishell.h"
void free_redirections(t_redirections *redirections)
{
    // if (*redirections->argv)
    //     ft_free_split(redirections->argv);
    if (redirections->heredoc_all)
    {
        free(redirections->heredoc_all);
        redirections->heredoc_all = NULL;
    }
    if (redirections->op)
    {
        free(redirections->op);
        redirections->op = NULL;
    }
    if (redirections->target)
    {
        free(redirections->target);
        redirections->target = NULL;
    }
    if (redirections->current_doc)
    {
        free(redirections->current_doc);
        redirections->current_doc = NULL;
    }
    if (redirections->line)
    {
        free(redirections->line);
        redirections->line = NULL;
    }
}
void free_here_doc(t_here_document *here_doc)
{
    if (here_doc->line)
    {
        free(here_doc->line);
        here_doc->line = NULL;
    }
    if (here_doc->current_doc)
    {
        free(here_doc->current_doc);
        here_doc->current_doc = NULL;
    }
    if (here_doc->target)
    {
        free(here_doc->target);
        here_doc->target = NULL;
    }
}

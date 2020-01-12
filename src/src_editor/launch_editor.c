#include "doom.h"

void	clear_n_exit(t_env *w, t_win *win)
{
	free(win->itoastr);
	if (win->paramtxt != NULL)
		free(win->paramtxt);
	if (win->paramvaluetxt != NULL)
		free(win->paramvaluetxt);
	if (win->helptxt != NULL)
		free(win->helptxt);
	free_listlist(win);
	free_triangles(win);
	free_assets(win);
	w->stopread = 1;
}

int		level_editor_start(t_env *w)
{
	t_win	win;

	win.wo = w;
	fit_to_editor(w, &win);
	init2(w, &win);
	loop_play(w, &win);
	w->menu.i = 1;
	return (EXIT_SUCCESS);
}

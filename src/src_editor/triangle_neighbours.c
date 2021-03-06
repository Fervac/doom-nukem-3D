#include "doom.h"

void		triangles_neighbours_helper(t_win *win, int *ts1, int *ts2)
{
	if (win->tni == 0)
	{
		if (ts1[0] == ts2[0] || ts1[0] == ts2[1] || ts1[0] == ts2[2])
			win->neighbour_index++;
		if (ts1[1] == ts2[0] || ts1[1] == ts2[1] || ts1[1] == ts2[2])
			win->neighbour_index++;
	}
	if (win->tni == 1)
	{
		if (ts1[1] == ts2[0] || ts1[1] == ts2[1] || ts1[1] == ts2[2])
			win->neighbour_index++;
		if (ts1[2] == ts2[0] || ts1[2] == ts2[1] || ts1[2] == ts2[2])
			win->neighbour_index++;
	}
	if (win->tni == 2)
	{
		if (ts1[2] == ts2[0] || ts1[2] == ts2[1] || ts1[2] == ts2[2])
			win->neighbour_index++;
		if (ts1[0] == ts2[0] || ts1[0] == ts2[1] || ts1[0] == ts2[2])
			win->neighbour_index++;
	}
}

int			triangles_neighbours_helper2(t_win *win,
	t_lstlst *tmp2, t_lstlst *tmp3, int *tab_sector1)
{
	int			swap;
	int			*tab_sector2;

	tab_sector2 = NULL;
	win->neighbour_index = 0;
	if (tmp3->sector != -1 && tmp3->sector != tmp2->sector)
	{
		tab_sector2 = tab_sector3(win->wo, win, tmp3->sector);
		if (tmp3->clockwise == 2)
		{
			swap = tab_sector2[1];
			tab_sector2[1] = tab_sector2[2];
			tab_sector2[2] = swap;
		}
		triangles_neighbours_helper(win, tab_sector1, tab_sector2);
		if (win->neighbour_index == 2)
		{
			free_dot_tab(tab_sector2);
			return (tmp3->sector);
		}
	}
	free_dot_tab(tab_sector2);
	return (-1);
}

int			triangles_neighbours(t_env *w, t_win *win, t_lstlst *tmp2)
{
	t_lstlst	*tmp3;
	int			*tab_sector1;
	int			swap;
	int			ret;

	tab_sector1 = tab_sector3(w, win, tmp2->sector);
	if (tmp2->clockwise == 2)
	{
		swap = tab_sector1[1];
		tab_sector1[1] = tab_sector1[2];
		tab_sector1[2] = swap;
	}
	tmp3 = win->triangles;
	while (tmp3)
	{
		ret = triangles_neighbours_helper2(win, tmp2, tmp3, tab_sector1);
		if (ret != -1)
		{
			free_dot_tab(tab_sector1);
			return (ret);
		}
		tmp3 = tmp3->next;
	}
	free_dot_tab(tab_sector1);
	return (-1);
}

void		check4(t_win *win)
{
	t_lstlst	*tmp2;
	t_lst		*tmp;

	tmp2 = win->triangles;
	while (tmp2)
	{
		tmp = tmp2->head;
		if (tmp->nb != tmp->next->nb && tmp->next->nb != tmp->next->next->nb
			&& tmp->next->next->nb != tmp->nb)
		{
			tmp2->sector = win->triangle_sector;
			win->triangle_sector += 1;
		}
		else
			tmp2->sector = -1;
		while (tmp)
		{
			tmp->sector = tmp2->sector;
			tmp = tmp->next;
		}
		tmp2 = tmp2->next;
	}
}

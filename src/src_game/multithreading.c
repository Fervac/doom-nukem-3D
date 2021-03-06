/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvienot <nvienot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 12:07:14 by nvienot           #+#    #+#             */
/*   Updated: 2019/07/25 07:07:52 by nvienot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	calcul_render_no_mthrd(t_env *w, t_work *work)
{
	int x;

	x = work->startx;
	while (x <= work->endx)
	{
		calc_z(work, x);
		draw_ceiling_n_floor(work, w, x);
		if (work->network >= 0)
		{
			draw_networks(work, w, x);
			work->ytop[x] = vmid(vmax(work->cya, work->cnya),
				work->ytop[x], HEIGHT - 1);
			work->ybot[x] = vmid(vmin(work->cyb,
				work->cnyb), 0, work->ybot[x]);
		}
		else
			draw_walls(work, w, x);
		x++;
	}
}

void	*thread(t_worker_arg *arg)
{
	t_work		*work;
	int			x;
	int			end_x;

	work = &arg->work;
	x = work->startx + arg->start * ((work->endx - work->startx) / NB_THREAD);
	end_x = work->endx - (NB_THREAD - (arg->start + 1))
		* ((work->endx - work->startx) / NB_THREAD);
	while (x <= end_x)
	{
		calc_z(work, x);
		draw_ceiling_n_floor(work, arg->w, x);
		if (work->network >= 0)
		{
			draw_networks(work, arg->w, x);
			arg->ytop[x] = vmid(vmax(work->cya, work->cnya),
				work->ytop[x], arg->w->res.height - 1);
			arg->ybot[x] = vmid(vmin(work->cyb, work->cnyb), 0, work->ybot[x]);
		}
		else
			draw_walls(work, arg->w, x);
		x++;
	}
	return (NULL);
}

int		calcul_render_mthrd(t_env *w, t_work work, int *ybot, int *ytop)
{
	pthread_t		calc_thread[NB_THREAD];
	t_worker_arg	args[NB_THREAD];
	int				i;
	int				thr[NB_THREAD];

	i = 0;
	while (i < NB_THREAD)
	{
		args[i] = (t_worker_arg)
		{
			.w = w,
			.start = i,
			.work = work,
			.ybot = ybot,
			.ytop = ytop
		};
		thr[i] = pthread_create(&calc_thread[i], NULL, (void*)thread, args + i);
		if (thr[i])
			set_error(w, w->m, 8, strdup_check(w, "thread doesn't exist"));
		i++;
	}
	while (i--)
		pthread_join(calc_thread[i], NULL);
	return (0);
}

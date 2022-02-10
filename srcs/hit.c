#include "../includes/minirt.h"

// 현재 도형에 부딫지는지 검사
int	hit_figures(t_figure *figs, t_ray *r, t_hit_record *rec)
{
	int	hit;

	hit = 0;
	if (figs->flag == SP) //현재도형이 구
		hit = hit_sphere(figs, r, rec);
	else if (figs->flag == PL) //현재도형이 평면
		hit = hit_plane(figs, r, rec);
	else if (figs->flag == CY) //현재도형이 원기둥
		hit = hit_cylinder(figs, r, rec);
	return (hit);
}

//광선이 링크드리스트 도형들에 부딪치는지 검사
int	hit(t_figure *figs, t_ray r, t_hit_record *rec)
{
	int			hit;
	t_hit_record	tmp;

	tmp = *rec;
	hit = 0;
	while (figs)
	{
		if (hit_figures(figs, &r, &tmp))
		{
			hit = 1;
			tmp.t_max = tmp.t;
			*rec = tmp;
		}
		figs = figs->next;
	}
	return (hit);
}

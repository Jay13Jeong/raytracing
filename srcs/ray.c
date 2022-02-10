#include "../includes/minirt.h"

t_ray	ray(t_p3 orig, t_p3 dir)
{
	t_ray ray;

	ray.origin = orig;
	ray.dir = get_unit(dir);
	return (ray);
}

t_p3	ray_at(t_ray *r, double t)
{
	return (vec_plus(vec_X(r->dir, t), r->origin));
}

//광선의 단위벡터를 받아 광선이 도형에 부딪친 색상을 rgb로 반환
t_p3	ray_color(t_rt *rt, t_ray r)
{
	t_hit_record	rec; // 도형과 광선의 교차정보 저장

	rec.t_min = EPS;
	rec.t_max = INFINITY;
	if (hit(rt->figs, r, &rec)) //도형에 광선이 부딪쳤을 때
		return (phong_lighting(rt, &rec));
	return (vdefine(1, 1, 1)); //도형에 광선이 부딪치지 않을 때
}

//인자로 받아온 두개의 백분율 uv값에 대응해서 가장왼쪽아래광선을 기준으로
//다음광선의 벡터를 계산후 그 값을 단위벡터로 반환한다.
t_ray	ray_primary(t_scene *scene, double u, double v)
{
	t_ray	ray;
	 
	ray.origin = scene->origin;
	ray.dir.x = scene->l_l_corner.x + u * scene->horizontal.x
		+ v * scene->vertical.x - scene->origin.x;
	ray.dir.y = scene->l_l_corner.y + u * scene->horizontal.y
		+ v * scene->vertical.y - scene->origin.y;
	ray.dir.z = scene->l_l_corner.z + u * scene->horizontal.z
		+ v * scene->vertical.z - scene->origin.z;
	ray.dir = get_unit(ray.dir);// 다음광선벡터를 단위벡터로 변환
	return (ray);
}

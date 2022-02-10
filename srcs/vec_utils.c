#include "../includes/minirt.h"

double	vec_len(t_p3 a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

t_p3	get_unit(t_p3 a)
{
	return (vec_divide(a, vec_len(a)));
}

t_p3	vdefine(double x, double y, double z)
{
	t_p3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

double	vdot(t_p3 a, t_p3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
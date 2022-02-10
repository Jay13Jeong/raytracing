#include "../includes/minirt.h"

t_p3	vec_minus(t_p3 a, t_p3 b)
{
	t_p3	new;

	new.x = a.x - b.x;
	new.y = a.y - b.y;
	new.z = a.z - b.z;
	return (new);
}

t_p3	vec_plus(t_p3 a, t_p3 b)
{
	t_p3	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return (v);
}

t_p3	vec_X(t_p3 a, double t)
{
	t_p3	v;

	v.x = a.x * t;
	v.y = a.y * t;
	v.z = a.z * t;
	return (v);
}

t_p3	vec_divide(t_p3 a, double b)
{
	t_p3	temp;

	temp.x = a.x / b;
	temp.y = a.y / b;
	temp.z = a.z / b;
	return (temp);
}

t_p3	vec_cross(t_p3 a, t_p3 b)
{
	t_p3	v;

	v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;
	return (v);
}
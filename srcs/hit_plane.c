#include "../includes/minirt.h"

//인자로 받아온 광선이 평면에 부딪치는지 검사
int	hit_plane(t_figure *figs, t_ray *r, t_hit_record *rec)
{
	double	denominator; //분모
	double	nom; //분자
	double	t; //스칼라

	denominator = vdot(r->dir, figs->fig.pl.n); //광선벡터와 평면법선의 내적
	nom = vdot(vec_minus(figs->fig.pl.p, r->origin), figs->fig.pl.n);//카메라까지 거리와 법선의 내적
	if (denominator == 0) //광석벡터와 법선이 직교하면 표시하지 않음
		return (0);
	t = nom / denominator;
	if (t < rec->t_min || t > rec->t_max) //카메라뒤에 있거나 너무 멀리있으면 표시하지않음
		return (0);
	rec->t = t;
	rec->p = ray_at(r, t); //카메라시점 + (광선벡터 * 광선스칼라) = 교차점
	rec->normal = figs->fig.pl.n;
	rec->albedo = figs->albedo;
	set_face_normal(r, rec);
	return (1);
}

/*
평면과 광선의 교차점 P
평면의 중심 P0
카메라 벡터 L
카메라위치 L0
광선스칼라 T
평면의 중심 P0가 법선과 직교한다면 "내적((P - P0), 법선) = 0"
"P = L0 + (L * T)" 이므로
"내적(( (L0 + (L * T)) - P0), 법선) = 0"
T의 값을 알아 내야하므로 이항해서 식을 정리하면
T = [내적((P0 - L0), 법선)] / [내적(L, 법선)]
다시 T를 카메라의 시점부터 광선벡터에 적용하면 교차점 get
*/
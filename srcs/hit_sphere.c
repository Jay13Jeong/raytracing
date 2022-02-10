#include "../includes/minirt.h"

//인자로 받아온 광선이 구에 부딪치는지 근의 공식으로 검사 
int	hit_sphere(t_figure *figs, t_ray *r, t_hit_record *rec)
{
	t_p3			oc;
	t_discriminant	d;

	oc = vec_minus(r->origin, figs->fig.sp.c); //카메라와 구의 거리
	d.a = vdot(r->dir, r->dir); //2차방정식 A부분
	d.b = 2.0 * vdot(r->dir, oc); //2차방정식 B부분
	d.c = vdot(oc, oc) - pow(figs->fig.sp.r, 2); //2차방정식 C부분
	d.discriminant = pow(d.b, 2) - (4.0 * d.a * d.c); //근의 공식 판별식
	if (d.discriminant < 0) //판별식이 음수면 허근이므로 부딪치지 않음
		return (0);
	d.root = (-d.b - sqrt(d.discriminant)) / (2.0 * d.a); //근의 공식으로 근 구하기
	if (d.root < rec->t_min || rec->t_max < d.root) //'-'근이 카메라 뒤에 있거나 너무 멀리있으면
	{
		d.root = (-d.b + sqrt(d.discriminant)) / (2.0 * d.a); //근의 공식으로 '+'근 구하기
		if (d.root < rec->t_min || rec->t_max < d.root)
			return (0);
	}
	rec->t = d.root;
	rec->p = ray_at(r, d.root); //교차점 구하기
	rec->normal = get_unit(vec_minus(rec->p, figs->fig.sp.c)); //구의 법선 구하기
	rec->albedo = figs->albedo; //구의 색상 기록
	set_face_normal(r, rec); //카메라가 구 안에있을 때 법선 처리
	return (1);
}

/*
구의 중심 C
반지름 r
교차할 광선과 구의 표면좌표가 P
카메라위치 O
카메라 벡터 D
카메라 스칼라 T
3차원 구의 방적식 "(C.x)2 + (C.y)2 + (C.z)2 = r2"
"|P - C| = r" 이므로 "(P - C)2 = r2"
"P = O + (D * T)" 이므로 "(O + (D * T) - C)2 = r2"
2차 방정식으로 바꾸기 과정(아래)
> 전부 좌항으로 옮긴다. ((D * T) + O - C)2 - r2 = 0
> T를 중심으로 "Ax2 + Bx + c = 0" 형태로 바꾼다. 
> (D * T)2 + 2(D * T)(O - C) + ((O - C)2 - r2) = 0
> (D2 * T2) + (2D(O - C) * T) + ((O - C)2 - r2) = 0
이제 2차방적식 근의 공식으로 T를 구한다.
*/
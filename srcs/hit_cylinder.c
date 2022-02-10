#include "../includes/minirt.h"

t_p3	get_axis(t_cy cylinder) //바닥센터부터 탑센터까지 벡터 반환
{
	return (vec_X(cylinder.n, cylinder.height));
}

int			hit_cylinder(t_figure *figs, t_ray *r, t_hit_record *rec)
{
	t_p3			n_axis;
	t_p3			w;
	t_discriminant	d;
	double			roots[2];

	n_axis = figs->fig.cy.n;
	w = vec_minus(r->origin, figs->fig.cy.c); //카메라와의 거리
	d.a = vdot(r->dir, r->dir) - pow(vdot(r->dir, n_axis), 2); //2차방정식 A부분
	d.b = 2.0 * (vdot(r->dir, w) - vdot(r->dir, n_axis) * vdot(w, n_axis)); //2차방정식 B부분
	d.c = vdot(w, w) - pow(vdot(w, n_axis), 2) - pow(figs->fig.cy.r, 2); //2차방정식 C부분
	d.discriminant = pow(d.b, 2) - (4.0 * d.a * d.c); //근의 공식 판별식
	if (d.discriminant < 0) //판별식이 음수면 허근이므로 부딪치지 않음
		return (0);
	roots[0] = (-d.b - sqrt(d.discriminant)) / (2.0 * d.a);
	roots[1] = (-d.b + sqrt(d.discriminant)) / (2.0 * d.a); //근의 공식으로 근 구하기
	rec->p = ray_at(r, roots[0]);
	double cal_len = vdot(vec_minus(rec->p, figs->fig.cy.c), n_axis);//광선과 교차점의 직교 높이
	if (cal_len >= 0 && cal_len <= vec_len(get_axis(figs->fig.cy))) // '-'근이 surface인지 확인
		rec->t = roots[0];
	else
	{
		rec->p = ray_at(r, roots[1]);
		cal_len = vdot(vec_minus(rec->p, figs->fig.cy.c), n_axis);
		if (cal_len >= 0 && cal_len <= vec_len(get_axis(figs->fig.cy))) // '+'근이 surface인지 확인
			rec->t = roots[1];
		else
			return (0);
	}
	if (rec->t < rec->t_min || rec->t > rec->t_max) //카메라 뒤에 있거나 너무 멀리있으면 무시
		return (0);
	t_p3 cp = vec_minus(rec->p, figs->fig.cy.c);
	t_p3 qp = vec_minus(cp, vec_X(figs->fig.cy.n, vdot(cp, figs->fig.cy.n)));
	rec->normal = get_unit(qp); //법선 구하기
	rec->albedo = figs->albedo; //색상 기록
	set_face_normal(r, rec); //카메라가 안에있을 때 법선 처리
	return (1);
}

/*
원기둥 바닥의 중심 C
반지름 r
교차할 광선과 원기둥의 표면좌표가 P
축과 P가 직교하는 축의 임의 지점 Q
원기둥 축의 단위벡터 ĥ
카메라위치 O
카메라 시점 단위벡터 v

||CP|| = sqrt(||CQ||^2 + r^2)
||CP||^2 = ||CQ||^2 + r^2
||CQ|| = 내적(CP, ĥ)
||CP||2 - 내적(CP, ĥ)^2 = r^2
P = O + (t * v) 이므로 위 식에 대입하면
||O + (t * v) - C||^2 - 내적(O + (t * v), ĥ)^2 = r^2
위 식을 2차방정식의 형태로 정리하면
‖tv+w‖2−[(tv+w)⋅ĥ ]2=r2 
(tv+w)⋅(tv+w)−[(v⋅ĥ )t+(w⋅ĥ )]2=r2
(v⋅v)t2+2(v⋅w)t+(w⋅w)−(v⋅ĥ )2t2−2(v⋅ĥ )(w⋅ĥ )t−(w⋅ĥ )2=r2
[(v⋅v)−(v⋅ĥ )2]t2+2[(v⋅w)−(v⋅ĥ )(w⋅ĥ )]t+(w⋅w)−(w⋅ĥ )2−r2=0
a=(v⋅v)−(v⋅ĥ )^2
b=2 * [(v⋅w)−(v⋅ĥ )(w⋅ĥ )]
c=(w⋅w)−(w⋅ĥ )2−r2
위 식만으로는 길이가 무한인 원기둥이 나오므로 map에서 가져온 높이로 컷팅한다
P의 원기둥 높이 = 내적(CP, ĥ)
P의 높이가 "0이상", "원기둥 높이 이하"일때에만 hit처리해준다.  
*/

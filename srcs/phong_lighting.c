#include "../includes/minirt.h"

//확산광(diffuse) 추가해주는 함수
t_p3	diffuse(t_p3 light_dir, t_hit_record *rec)
{
	double	radian; //광원과 도형의 법선의 내각

	radian = vdot(rec->normal, get_unit(light_dir)); //내각 구하기
	if (radian > 0.0) 							//법선과 광선벡터가 수직미만이면
		return (vec_X(rec->albedo, radian));	//법선과 광선이 평행에 가까워질수록 밝게 	
	return (vec_X(rec->albedo, 0.0));			//내각이 둔각을 넘어가면 검은색으로 고정
}

//그림자 생기는지 검사하는 함수
int		shadow_check(t_figure *figs, t_ray light_ray, double light_len)
{
	t_hit_record rec; //광선의 벡터정보를 담는 변수

	rec.t_min = 0; //도형에서 광선을 쏠때 상대적 시작지점을 0으로 설정한다
	rec.t_max = light_len; //광선길이는 도형에서 광원까지만 검사예정
	if (hit(figs, light_ray, &rec)) //현재 도형과 광원사이에 또다른 도형이 있다면 1(True)리턴
		return (1); //그림자를 만들어야함
	return (0); //그림자 안생김
}

//도형표면에서 광원방향으로 광선을 쏘는 함수
t_p3	point_light_get(t_rt *rt, t_light *light, t_hit_record *rec)
{
	t_p3	light_dir; //도형의 교차점에서 광원 방향 벡터
	t_ray	light_ray; //도형에서 광원방향 광선

	light_dir = vec_minus(light->position, rec->p); //교차점부터 광원방향 벡터
	light_ray = ray(vec_plus(rec->p, vec_X(rec->normal, EPS)), light_dir);
	if (shadow_check(rt->figs, light_ray, vec_len(light_dir))) //그림자가 생기는지 검사
		return (vdefine(0, 0, 0)); //그림자면 검은색 반환
	return (vec_X(diffuse(light_dir, rec), light->br * LUMEN)); //마지막으로 확산광과 화면밝기(LUMEN)를 입혀서 반환 
}

//도형의 색상에 light을 입혀서 반환하는 함수
t_p3	phong_lighting(t_rt *rt, t_hit_record *rec)
{
	t_light	light; //광원 정보
	t_p3	light_color; //계산할 광원값
	t_p3	result; //반환할값

	light = rt->scene.light;
	light_color = point_light_get(rt, &light, rec); //광원을 추적
	light_color = vec_plus(light_color, rt->scene.amb_color); //주변광을 더한다
	result.x = light_color.x * rec->albedo.x; //도형의 색상에 광원을 입혀준다
	result.y = light_color.y * rec->albedo.y;
	result.z = light_color.z * rec->albedo.z;
	if (result.x > 1.0) //최종 계산된 색상값이 1.0을 넘으면 1.0(rgb255)로 고정
		result.x = 1.0;
	if (result.y > 1.0)
		result.y = 1.0;
	if (result.z > 1.0)
		result.z = 1.0;
	return (result);
}

#include "../includes/minirt.h"

void	scene_init(t_rt *rt)
{
	double	radian_fov;
	double	scope;
	double	window_ratio; // 해상도 비율

	//그래픽에서는 인간의 각도(degree)말고 radian단위를 사용한다.
	radian_fov = (double)(rt->scene.cam.fov) * (M_PI / 180.0);
	scope = tan(radian_fov / 2);// 삼각함수로 시야범위의 가로범위 절반 구하기
	window_ratio = (double)RT_HEIGHT / (double)RT_WIDTH; //세로가로 백분율
	rt->scene.viewport_w = scope * 2.0;
	rt->scene.viewport_h = rt->scene.viewport_w * window_ratio;
	rt->scene.origin = rt->scene.cam.look_from;
}

//카메라의 시점을 초기화하는 함수
void	view_init(t_rt *rt)
{
	t_p3	w;
	t_p3	u;
	t_p3	v;

	w = vec_X(rt->scene.cam.look_at, -1.0);
	if ((w.x == 0 && w.z == 0) && (w.y == -1 || w.y == 1))//상수벡터010과 카메라벡터의 외적이 0이되면 계산이 안되므로  
		w = vec_plus(w, vdefine(EPS, EPS, EPS));//부동소수점의 오차를 더해줘서 0을 초과하는 값으로 보정해준다. 
	u = get_unit(vec_cross(vdefine(0, 1, 0), w));//외적으로 카메라 가로벡터 계산
	v = vec_cross(w, u);//외적으로 카메라 세로벡터 계산
	rt->scene.horizontal = vec_X(u, rt->scene.viewport_w);
	rt->scene.vertical = vec_X(v, rt->scene.viewport_h);
	rt->scene.l_l_corner.x = rt->scene.cam.look_from.x
		- (rt->scene.horizontal.x / 2) - (rt->scene.vertical.x / 2) - w.x;
	rt->scene.l_l_corner.y = rt->scene.cam.look_from.y
		- (rt->scene.horizontal.y / 2) - (rt->scene.vertical.y / 2) - w.y;
	rt->scene.l_l_corner.z = rt->scene.cam.look_from.z
		- (rt->scene.horizontal.z / 2) - (rt->scene.vertical.z / 2) - w.z;
}

//mlx를 초기화 하는 함수
void	init_mlx(t_rt *rt)
{
	rt->mlx.mlx = mlx_init();
	rt->mlx.win = mlx_new_window(rt->mlx.mlx, RT_WIDTH, RT_HEIGHT, "miniRT");
	rt->mlx.img = mlx_new_image(rt->mlx.mlx, RT_WIDTH, RT_HEIGHT);
	rt->mlx.addr = mlx_get_data_addr(rt->mlx.img, &rt->mlx.bpp, 
										&rt->mlx.size_line, &rt->mlx.endian);
}

//map파일 정보를 파싱해서 구조체에 할당해주는 함수
void	rt_init(t_rt *rt, char *rt_file)
{
	char	*line;
	int		map_fd;

	rt->figs = NULL;
	rt->scene.light_unique = 0;
	rt->scene.cam_unique = 0;
	rt->scene.amb_ex = 0;
	map_fd = open(rt_file, O_RDONLY);
	if (map_fd == -1)
		exit(1);
	while (get_next_line(map_fd, &line) == 1) // rt파일 컴파일
	{
		parse(rt, line);
		free(line);
	}
	parse(rt, line);
	free(line);
	if (!rt->scene.amb_ex || !rt->scene.cam_unique || !rt->scene.light_unique)
		alert_and_exit(4, "required A, C, L info"); //주변광, 조명, 캠설정이 없으면 에러
	rt->scene.amb_color = vec_X(rt->scene.amb_color, rt->scene.amb_ratio);
	init_mlx(rt); //mlx 초기화
}
#include "../includes/minirt.h"

//동적할당된 2차원 배열을 해제하는 함수
void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

//아스키를 벡터로 변환하는 함수
t_p3	a2vector(char *str)
{
	char	**rt ;
	t_p3	rgb;
	int		cnt;

	rt  = ft_split(str, ',');
	if (rt  == NULL)
		exit(EXIT_FAILURE);
	cnt = 0;
	while (rt [cnt] != NULL)
		cnt += 1;
	if (cnt != 3)
		exit(EXIT_FAILURE);
	rgb.x = a2f(rt [0]);
	rgb.y = a2f(rt [1]);
	rgb.z = a2f(rt [2]);
	free_arr(rt);
	return (rgb);
}

//아스키를 단위벡터로 변환하는 함수
t_p3	a2normal(char *str)
{
	char	**rt ;
	t_p3	vector;
	int		cnt;

	rt  = ft_split(str, ',');
	if (rt  == NULL)
		exit(EXIT_FAILURE);
	cnt = 0;
	while (rt [cnt] != NULL)
		cnt += 1;
	if (cnt != 3)
		exit(EXIT_FAILURE);
	vector.x = a2f(rt [0]);
	vector.y = a2f(rt [1]);
	vector.z = a2f(rt [2]);
	free_arr(rt);
	return (get_unit(vector));
}

//아스키를 rgb방식으로 변환하는 함수
t_p3	a2rgb(char *str)
{
	char	**rt ;
	t_p3	rgb;
	int		cnt;
	int		temp;

	rt  = ft_split(str, ',');
	if (rt  == NULL)
		exit(EXIT_FAILURE);
	cnt = 0;
	while (rt [cnt] != NULL)
	{
		temp = ft_atoi(rt [cnt]);
		if (temp < 0 || temp > 255)
		{
			printf("ambient rgb error\n");
			exit(EXIT_FAILURE);
		}
		cnt += 1;
	}
	if (cnt != 3)
		exit(EXIT_FAILURE);
	rgb.x = a2f(rt [0]) / 255.0;
	rgb.y = a2f(rt [1]) / 255.0;
	rgb.z = a2f(rt [2]) / 255.0;
	free_arr(rt);
	return (rgb);
}

//파싱된 정보의 갯수를 검사하는 함수
int	check_order(char **order, int need_cnt)
{
	int	order_cnt;

	order_cnt = 0;
	while (order[order_cnt] != NULL)
		order_cnt += 1;
	if (order_cnt != need_cnt)
		return (0);
	return (1);
}

//주변광 정보를 파싱하는 함수
int	parse_ambient(t_scene *scene, char **order)
{
	if (!check_order(order, 3))
		return (-1);
	if (scene->amb_ex == 1)
		alert_and_exit(4, "Double declaration Ambient");
	scene->amb_ex = 1;
	scene->amb_ratio = a2f(order[1]);
	if (scene->amb_ratio < 0 || scene->amb_ratio > 1)
		alert_and_exit(6, "Ambient lightning ratio");
	scene->amb_color = a2rgb(order[2]);
	return (0);
}

//카메라 정보를 파싱하는 함수
int	parse_camera(t_scene *scene, char **order)
{
	if (!check_order(order, 4))
		return (-1);
	if (scene->cam_unique == 1)
		alert_and_exit(4, "Double declaration Camera");
	scene->cam_unique = 1;
	scene->cam.look_from = a2vector(order[1]);
	scene->cam.look_at = a2normal(order[2]);
	scene->cam.fov = ft_atoi(order[3]);
	if (scene->cam.fov < 0 || scene->cam.fov > 180)
		alert_and_exit(6, "FOV");
	return (0);
}

//광원 정보를 파싱하는 함수
int	parse_light(t_scene *scene, char **order)
{

	if (!check_order(order, 4))
		alert_and_exit(3, "");
	if (scene->light_unique == 1)
		alert_and_exit(4, "Double declaration light");
	scene->light_unique = 1;
	scene->light.position = a2vector(order[1]);
	scene->light.br = a2f(order[2]);
	if (scene->light.br < 0 || scene->light.br > 1)
		alert_and_exit(6, "Brightness");
	scene->light.color = a2rgb(order[3]);
	return (0);
}

//인자로 받아온 map파일을 파싱하는 함수
void	parse(t_rt *rt, char *line)
{
	char	**order;

	if (line == NULL || *line == '\0')
		return ;
	order = ft_split(line, ' ');
	if (order == NULL)
		exit(EXIT_FAILURE);
	if (ft_strncmp(order[0], "A", 2) == 0)			//주변광 정보 파싱
		parse_ambient(&rt->scene, order);
	else if (ft_strncmp(order[0], "C", 2) == 0)		//카메라 정보 파싱
		parse_camera(&rt->scene, order);
	else if (ft_strncmp(order[0], "L", 2) == 0)		//광원 정보 파싱
		parse_light(&rt->scene, order);
	else if (ft_strncmp(order[0], "sp", 3) == 0)	//구 정보 파싱
		parse_sphere(&rt->figs, order);
	else if (ft_strncmp(order[0], "pl", 3) == 0)	//평면 정보 파싱
		parse_plane(&rt->figs, order);
	else if (ft_strncmp(order[0], "cy", 3) == 0)	//원기둥정보 파싱
		parse_cylinder(&rt->figs, order);
	free_arr(order);
}

#include "../includes/minirt.h"

int		key_press(int key, void *param)
{
	if (key == 53)
		exit(0);
	param = NULL;
	return (0);
}

void		make_image(t_rt *rt)
{
	int		i;
	int		j;
	double	u;
	double	v;
	t_p3	color;

	scene_init(rt); //보여줄 장면의 범위를 수평시야각을 적용해서 구함.
	view_init(rt); //광선들을 쏠 면적을 구하고 시작광선위치(가장왼쪽아래)를 구함
	i = RT_HEIGHT;
	while (--i >= 0)
	{
		j = -1;
		while (++j < RT_WIDTH)
		{
			u = (double)j / (double)(RT_WIDTH - 1);//총 가로해상도에서 현재 가로위치를 백분율
			v = (double)i / (double)(RT_HEIGHT - 1);//총 세로해상도에서 현재 세로위치를 백분율
			color = ray_color(rt, ray_primary(&rt->scene, u, v));//광선을 쏴서 색을 얻음
			put_pixel(&rt->mlx, j, i, color);//mlx 현재픽셀에 광선으로 구한색상을 적용
		}
	}
}

int		main(int argc, char **argv)
{
	t_rt	rt;
	char	*exe_type;

	if (argc != 2) //인자가 하나가 아니면 모두 에러
		alert_and_exit(1, "");
	exe_type = &argv[1][(ft_strlen(argv[1]) - 3)];
	if (ft_strncmp(exe_type, ".rt", 4)) //인자의 확장자가 .rt로 끝나지 않으면 에러
		alert_and_exit(2, "");
	rt_init(&rt, argv[1]); //맵정보를 구조체에 적용
	make_image(&rt); //레이트레이싱으로 이미지 구현
	mlx_put_image_to_window(rt.mlx.mlx, rt.mlx.win, rt.mlx.img, 0, 0);
	mlx_key_hook(rt.mlx.win, key_press, NULL);
	mlx_loop(rt.mlx.mlx);
	return (0);
}

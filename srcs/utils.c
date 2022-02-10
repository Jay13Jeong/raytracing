#include "../includes/minirt.h"

void	set_face_normal(t_ray *r, t_hit_record *rec)
{
	rec->front_face = vdot(r->dir, rec->normal) < 0;
	rec->normal = (rec->front_face) ? rec->normal : vec_X(rec->normal, -1);
}

void	put_pixel(t_mlx *mlx, int x, int y, t_p3 rgb)
{
	char			*img_addr;
	unsigned int	color_hex;
	int				red;
	int				green;
	int				blue;

	red = (int)(rgb.x * 255);
	green = (int)(rgb.y * 255);
	blue = (int)(rgb.z * 255);
	color_hex = ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
	y = RT_HEIGHT - y;
	img_addr = &mlx->addr[(y * mlx->size_line + x * (mlx->bpp / 8))];
	*(unsigned int *)img_addr = color_hex;
}

void	alert_and_exit(int error_code, char *msg)
{
	printf("Error\n");
	if (error_code == 1)
		printf("invalid argument\n");
	else if (error_code == 2)
		printf("invalid map file\n");
	else if (error_code == 3)
		printf("range error : %s\n", msg);
	else if (error_code == 4)
		printf("check file info : %s\n", msg);
	else if (error_code == 5)
		printf("memory error : %s\n", msg);
	exit(EXIT_FAILURE);
}

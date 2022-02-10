#ifndef _MINIRT_H
# define _MINIRT_H

# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdlib.h>
# include <float.h>
# include "../mlx/mlx.h"
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"

# define EPS		FLT_EPSILON //부동소숫점 오차
# define LUMEN		1			//회면밝기
# define RT_WIDTH	1100 		//해상도 가로길이
# define RT_HEIGHT	619 		//해상도 세로길이
# define ESC_KEY	53 			//애플키보드 esc 키코드

# define SP 		0 //구 타입
# define PL 		1 //평면 타입
# define CY 		2 //원통 타입

typedef struct	s_p3 //3d좌표 또는 rgb 범용
{
	double	x;
	double	y;
	double	z;
}				t_p3;

typedef struct	s_sphere // 구
{
	t_p3	c; //위치좌표
	double	r; //공의 반지름
}				t_sp;

typedef struct	s_plane // 평면
{
	t_p3	p; //위치좌표
	t_p3	n; //법선의 방향 단위벡터 (-1 ~ 1)
}				t_pl;

typedef struct	s_cylinder //원통
{
	t_p3	c; //위치좌표
	t_p3	n; //법선의 방향 단위벡터 (-1 ~ 1)
	double	r; //원통의 반지름
	double	height; //원통 높이
}				t_cy;

union			u_figure //통합 도형 구조체
{
	t_sp	sp;//도형-구
	t_pl	pl;//도형-평면
	t_cy	cy;//도형-원통
};

typedef struct	s_figure //링크드리스트(도형모음)
{
	int				flag; //도형 타입
	union u_figure	fig; //도형 정보
	t_p3			albedo; //도형 색상rgb
	struct s_figure	*next; //다음 도형링크
}				t_figure; 

typedef struct	s_cam //카메라
{
	t_p3			look_from; //카메라위치
	t_p3			look_at; //카메라의 방향 단위벡터 (-1 ~ 1)
	int				fov; //ZOOM, 수평 시야각 (0 ~ 180도)
}				t_cam;

typedef struct	s_hit_record
{
	t_p3	p; //교차점
	t_p3	normal; //법선
	double	t_min; //스칼라 최솟값?
	double	t_max; //스칼라 최댓값?
	double	t; //광선의 스칼라
	int		front_face; //교차점과 광선벡터의 내적 (둔각 1 예각 0) 
	t_p3	albedo; //색상
}				t_hit_record;

typedef struct	s_ray //광선 구조체
{
	t_p3	origin; //광선 발사 시작 위치
	t_p3	dir; //광선벡터
}				t_ray;

typedef struct	s_light //조명
{
	t_p3			position;//조명 위치
	double			br;//조명 밝기
	t_p3			color; //조명 색상 (각각 0 ~ 255)
}				t_light;

typedef struct	s_scene //화면 표현값 구조체
{
	int			cam_unique;//캠 설정여부 (설정됨 1 아니면 0)
	t_cam		cam;//카메라
	double		viewport_h; //시야각 세로범위
	double		viewport_w; //시야각 가로범위
	t_p3		origin; //카메라위치 백업
	t_p3		horizontal; //카메라 수평벡터
	t_p3		vertical; //카메라 수직벡터
	t_p3		l_l_corner; //가장왼쪽아래광선벡터
	double		amb_ratio; //주변광 비율 (0 ~ 1 사이값)
	t_p3		amb_color; //주변광 색상 (각각 0 ~ 255)
	int			amb_ex; //주변광 설정여부 (설정됨 1 아니면 0)
	int			light_unique; //조명 설정여부 (설정됨 1 아니면 0)
	t_light		light; //조명
}				t_scene;

typedef struct	s_mlx
{
	void			*mlx; //mlx pointer
	void			*win; //mlx window pointer
	void			*img; //mlx img pointer
	char			*addr; //img의 첫픽셀 주소
	int				bpp; //bits per pixel (픽셀당 필요한 비트 수)
	int				size_line; //보여줄 img의 한줄 픽셀 갯수
	int				endian; //픽셀 색상 저장방식 (little 0, big 1)
}				t_mlx;

typedef struct	s_rt
{
	t_mlx	mlx; //mlx 설정값
	t_scene	scene; // 화면 설정값
	t_figure	*figs; // 도형들
}				t_rt;

typedef struct	s_discriminant //근의공식
{
	double	a;				//x2 부분
	double	b;				//x 부분
	double	c;				//일반 수 부분
	double	discriminant;	//판별식 값
	double	root;			//근
}				t_discriminant;

int		hit_cylinder(t_figure *lst, t_ray *r, t_hit_record *rec);
int		hit_plane(t_figure *lst, t_ray *r, t_hit_record *rec);
int		hit_sphere(t_figure *lst, t_ray *r, t_hit_record *rec);
int		hit_figures(t_figure *lst, t_ray *r, t_hit_record *rec);
int		hit(t_figure *lst, t_ray r, t_hit_record *rec);

void	rt_init(t_rt *data, char *rt_file);
void	scene_init(t_rt *data);
void	view_init(t_rt *data);

int		key_press (int key, void *param);

void	make_image(t_rt *data);

int		parse_sphere(t_figure **lst, char **order);
int		parse_plane(t_figure **lst, char **order);
int		parse_cylinder(t_figure **lst, char **order);

int		parse_ambient(t_scene *scene, char **order);
int		parse_camera(t_scene *scene, char **order);
int		parse_light(t_scene *scene, char **order);
void	parse(t_rt *data, char *line);

t_p3	phong_lighting(t_rt *data, t_hit_record *rec);

t_ray	ray(t_p3 orig, t_p3 dir);
t_p3	ray_at(t_ray *r, double t);
t_p3	ray_color(t_rt *data, t_ray r);
t_ray	ray_primary(t_scene *scene, double u, double v);

void	set_face_normal(t_ray *r, t_hit_record *rec);

t_p3	vdefine(double x, double y, double z);
double	vdot(t_p3 a, t_p3 b);

void	alert_and_exit(int error_code, char *err_msg);

double  a2f(char *str);
t_p3	a2rgb(char *str);
int		check_order(char **order, int need_cnt);
void	free_arr_all(char **arr);
t_p3	a2normal(char *str);
t_p3	a2vector(char *str);
t_p3	vec_minus(t_p3 a, t_p3 b);
t_p3	vec_cross(t_p3 a, t_p3 b);
t_p3	vec_X(t_p3 a, double t);
t_p3	vec_divide(t_p3 a, double t);
t_p3	vec_plus(t_p3 a, t_p3 b);
double	vec_len(t_p3 a);
t_p3	get_unit(t_p3 a); //벡터를 벡터의 길이만큼 나눈 값
void	put_pixel(t_mlx *mlx, int x, int y, t_p3 rgb);

#endif

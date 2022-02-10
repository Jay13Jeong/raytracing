#include "../includes/minirt.h"

//파싱한 구 정보를 구조체에 할당하는 함수
int	parse_sphere(t_figure **figs, char **order)
{
	t_figure	*new;
	t_figure	*temp;

	if (!check_order(order, 4))
		return (-1);
	new = (t_figure *)malloc(sizeof(t_figure));
	if (new == NULL)
		alert_and_exit(5, "sphere figure");
	new->flag = SP;
	new->fig.sp.c = a2vector(order[1]);
	new->fig.sp.r = a2f(order[2]) / 2.0;
	new->albedo = a2rgb(order[3]);
	new->next = NULL;
	temp = *figs;
	if (*figs == NULL)
		*figs = new;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	return (0);
}

//파싱한 평면 정보를 구조체에 할당하는 함수
int	parse_plane(t_figure **figs, char **order)
{
	t_figure	*new;
	t_figure	*temp;

	if (!check_order(order, 4))
		return (-1);
	new = (t_figure *)malloc(sizeof(t_figure));
	if (new == NULL)
		alert_and_exit(5, "plane figure");
	new->flag = PL;
	new->fig.pl.p = a2vector(order[1]);
	new->fig.pl.n = get_unit(a2normal(order[2]));
	new->albedo = a2rgb(order[3]);
	new->next = NULL;
	temp = *figs;
	if (*figs == NULL)
		*figs = new;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	return (0);
}

//파싱한 원기둥 정보를 구조체에 할당하는 함수
int	parse_cylinder(t_figure **figs, char **order)
{
	t_figure	*new;
	t_figure	*temp;

	if (!check_order(order, 6))
		return (-1);
	new = (t_figure *)malloc(sizeof(t_figure));
	if (new == NULL)
		alert_and_exit(5, "cylinder figure");
	new->flag = CY;
	new->fig.cy.c = a2vector(order[1]);
	new->fig.cy.n = get_unit(a2normal(order[2]));
	new->fig.cy.r = a2f(order[3]) / 2;
	new->fig.cy.height = a2f(order[4]);
	new->albedo = a2rgb(order[5]);
	new->next = NULL;
	temp = *figs;
	if (*figs == NULL)
		*figs = new;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	return (0);
}

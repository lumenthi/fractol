/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 10:57:07 by lumenthi          #+#    #+#             */
/*   Updated: 2018/11/23 16:40:20 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	usage(void)
{
	ft_putstr_fd("fractol: invalid usage\n", 2);
	ft_putstr_fd("usage: fractol [mandelbrot/julia/burningship]\n", 2);
	exit(0);
}

int		new_window(t_mlx *data)
{
	if (!(data->mlx = mlx_init()))
		return (0);
	data->win = mlx_new_window(data->mlx, WIN_X, WIN_Y, WIN_TITLE);
	data->img.ptr = mlx_new_image(data->mlx, WIN_X, WIN_Y);
	data->img.data = mlx_get_data_addr(data->img.ptr, &data->img.bpp,
	&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
	data->prop.click = 0;
	return (1);
}

void	add_pixel(t_mlx d, int x, int y, int color)
{
	if (x < 0 || x >= WIN_X || y < 0 || y >= WIN_Y)
		;
	else
		*(int *)(d.img.data + ((x + y * WIN_X) * d.img.bpp)) = color;
}

void	clear_screen(t_mlx *data)
{
	ft_bzero(data->img.data, WIN_X * WIN_Y * data->img.bpp);
}

int		rgb_to_int(int r, int g, int b)
{
	int c;

	c = (r << 16) + (g << 8) + b;
	return (c);
}

void	draw(t_mlx d)
{
	if (ft_strcmp(d.prop.fractal, "mandelbrot") == 0)
		draw_mandelbrot(d);
	else if (ft_strcmp(d.prop.fractal, "julia") == 0)
		draw_julia(d);
	else if (ft_strcmp(d.prop.fractal, "burningship") == 0)
		draw_burningship(d);
}

int		display(t_mlx *data)
{
	clear_screen(data);
	draw(*data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.ptr, 0, 0);
	return (1);
}

void	get_julia(t_mlx *data)
{
	data->prop.x_min = -1;
	data->prop.x_max = 1.5;
	data->prop.y_min = -1.2;
	data->prop.y_max = 1.2;
	data->prop.zoom = 250;
	data->prop.iter = 150;
	data->prop.julia_r = 0.285;
	data->prop.julia_i = 0.01;
}

void	get_prop(t_mlx *data)
{
	if (ft_strcmp(data->prop.fractal, "mandelbrot") == 0)
	{
		data->prop.x_min = -2.1;
		data->prop.x_max = 1.0;
		data->prop.y_min = -1.6;
		data->prop.y_max = 1.6;
		data->prop.zoom = 200;
		data->prop.iter = 100;
	}
	else if (ft_strcmp(data->prop.fractal, "julia") == 0)
		get_julia(data);
	else if (ft_strcmp(data->prop.fractal, "burningship") == 0)
	{
		data->prop.x_min = -1.9;
		data->prop.x_max = 1.9;
		data->prop.y_min = -2.2;
		data->prop.y_max = 2.2;
		data->prop.zoom = 190;
		data->prop.iter = 100;
	}
}

int		key_hook(int key, t_mlx *data)
{
	if (key == ESC)
	{
		ft_putendl("ESC key pressed, exiting.");
		free(data->prop.fractal);
		exit(1);
	}
	else if (key == RECENTER)
	{
		get_prop(data);
		display(data);
	}
	return (0);
}

double	interpolate(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

void	zoom_up(t_mlx *data, int x, int y)
{
	double	mousere;
	double	mouseim;
	double	interpolation;

	mousere = (double)x /
		(WIN_X / (data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
	mouseim = (double)y /
		(WIN_Y / (data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
	interpolation = 1.0 / ZOOM;
	data->prop.x_min = interpolate(mousere, data->prop.x_min, interpolation);
	data->prop.y_min = interpolate(mouseim, data->prop.y_min, interpolation);
	data->prop.x_max = interpolate(mousere, data->prop.x_max, interpolation);
	data->prop.y_max = interpolate(mouseim, data->prop.y_max, interpolation);
	data->prop.zoom *= ZOOM;
	data->prop.iter++;
	display(data);
}

void	zoom_down(t_mlx *data, int x, int y)
{
	double	zoom;
	double	mousere;
	double	mouseim;
	double	interpolation;

	zoom = 1 / ZOOM;
	mousere = (double)x / (WIN_X
		/ (data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
	mouseim = (double)y / (WIN_Y
		/ (data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
	interpolation = 1.0 / zoom;
	data->prop.x_min = interpolate(mousere, data->prop.x_min, interpolation);
	data->prop.y_min = interpolate(mouseim, data->prop.y_min, interpolation);
	data->prop.x_max = interpolate(mousere, data->prop.x_max, interpolation);
	data->prop.y_max = interpolate(mouseim, data->prop.y_max, interpolation);
	data->prop.zoom *= zoom;
	data->prop.iter--;
	display(data);
}

int		mouse_hook(int button, int x, int y, t_mlx *data)
{
	if (button == ZOOM_UP)
		zoom_up(data, x, y);
	else if (button == ZOOM_DOWN)
		zoom_down(data, x, y);
	else if (button == 1)
	{
		if (data->prop.click == 0)
			data->prop.click = 1;
		else
			data->prop.click = 0;
	}
	return (0);
}

int		motion_hook(int x, int y, t_mlx *data)
{
	if (data->prop.click != 0)
	{
		data->prop.julia_r = (double)x / (WIN_X /
			(data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
		data->prop.julia_i = (double)y / (WIN_Y /
			(data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
		display(data);
	}
	return (0);
}

int		main(int argc, char **argv)
{
	t_mlx	data;

	if (argc == 2 && (ft_strcmp(argv[1], "mandelbrot") == 0 ||
	ft_strcmp(argv[1], "julia") == 0 || ft_strcmp(argv[1], "burningship") == 0))
	{
		data.prop.fractal = ft_strdup(argv[1]);
		get_prop(&data);
		if (!new_window(&data))
			return (0);
		display(&data);
		if (ft_strcmp(argv[1], "julia") == 0)
			mlx_hook(data.win, 6, 0, motion_hook, &data);
		mlx_key_hook(data.win, key_hook, &data);
		mlx_mouse_hook(data.win, mouse_hook, &data);
		mlx_loop(data.mlx);
	}
	else
		usage();
	ft_putstr(data.prop.fractal);
	return (1);
}

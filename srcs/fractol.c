/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 10:57:07 by lumenthi          #+#    #+#             */
/*   Updated: 2018/07/03 19:02:22 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"
#include <stdio.h>

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

int		hex_to_int(char *hexVal)
{
	int	base;
	int	ret;
	int	i;

	ret = 0;
	base = 1;
	i = ft_strlen(hexVal) - 1;
	while (i >= 0)
	{
		if (hexVal[i] >= '0' && hexVal[i] <= '9')
			ret += (hexVal[i] - 48) * base;
		else if (hexVal[i] >= 'a' && hexVal[i] <= 'f')
			ret += (hexVal[i] - 87) * base;
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
			ret += (hexVal[i] - 55) * base;
		base = base * 16;
		i--;
	}
	return (ret);
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

char	*int_to_hex(int nb)
{
	char	*hex;
	int		i;

	i = 0;
	if (nb == 0)
		return (ft_strdup("00"));
	hex = malloc(3);
	while (nb)
	{
		if (nb % 16 < 10)
			hex[i] = nb % 16 + '0';
		else
			hex[i] = nb % 16 + 'A' - 10;
		i++;
		nb /= 16;
	}
	hex[i] = '\0';
	return (hex);
}

int		rgb_to_int(int r, int g, int b)
{
	int c;

	c = (r << 16) + (g << 8) + b;
	return (c);
}

void	draw_mandelbrot(t_mlx d)
{
	t_calc	c;
	c.x = 0;

	while (c.x < WIN_X)
	{
		c.y = 0;
		while (c.y < WIN_Y)
		{
			c.c_r = c.x / d.prop.zoom + d.prop.x_min;
			c.c_i = c.y / d.prop.zoom + d.prop.y_min;
			c.z_r = 0;
			c.z_i = 0;
			c.i = 0;
			while (c.z_r * c.z_r + c.z_i * c.z_i < 4 && c.i < d.prop.iter)
			{
				c.tmp = c.z_r;
				c.z_r = c.z_r * c.z_r - c.z_i * c.z_i + c.c_r;
				c.z_i = 2 * c.z_i * c.tmp + c.c_i;
				c.i = c.i + 1;
			}
			if (c.i != d.prop.iter)
				add_pixel(d, c.x, c.y, rgb_to_int(0, c.i * 350 / d.prop.iter, c.i * 350 / d.prop.iter));
			else
				add_pixel(d, c.x, c.y, rgb_to_int(142, 149, 160));
			c.y++;
		}
		c.x++;
	}
}

void	draw_burningship(t_mlx d)
{
	t_calc	c;
	c.x = 0;

	while (c.x < WIN_X)
	{
		c.y = 0;
		while (c.y < WIN_Y)
		{
			c.c_r = c.x / d.prop.zoom + d.prop.x_min;
			c.c_i = c.y / d.prop.zoom + d.prop.y_min;
			c.z_r = 0;
			c.z_i = 0;
			c.i = 0;
			while (c.z_r * c.z_r + c.z_i * c.z_i < 4 && c.i < d.prop.iter)
			{
				c.tmp = c.z_r * c.z_r - c.z_i * c.z_i + c.c_r;
				c.z_i = fabs(2 * c.z_r * c.z_i) + c.c_i;
				c.z_r = c.tmp;
				c.i = c.i + 1;
			}
			if (c.i != d.prop.iter)
				add_pixel(d, c.x, c.y, rgb_to_int(0, c.i * 350 / d.prop.iter, 0));
			else
				add_pixel(d, c.x, c.y, rgb_to_int(155, 155, 155));
			c.y++;
		}
		c.x++;
	}
}

void	draw_julia(t_mlx d)
{
	t_calc	c;
	c.x = 0;

	while (c.x < WIN_X)
	{
		c.y = 0;
		while (c.y < WIN_Y)
		{
			c.c_r = d.prop.julia_r;
			c.c_i = d.prop.julia_i;
			c.z_r = c.x / d.prop.zoom + d.prop.x_min;
			c.z_i = c.y / d.prop.zoom + d.prop.y_min;
			c.i = 0;
			while (c.z_r * c.z_r + c.z_i * c.z_i < 4 && c.i < d.prop.iter)
			{
				c.tmp = c.z_r;
				c.z_r = c.z_r * c.z_r - c.z_i * c.z_i + c.c_r;
				c.z_i = 2 * c.z_i * c.tmp + c.c_i;
				c.i = c.i + 1;
			}
			if (c.i != d.prop.iter)
				add_pixel(d, c.x, c.y, rgb_to_int(0, c.i * 350 / d.prop.iter, c.i * 350 / d.prop.iter));
			else
				add_pixel(d, c.x, c.y, rgb_to_int(0, 242, 255));
			c.y++;
		}
		c.x++;
	}
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
	{
		data->prop.x_min = -1;
		data->prop.x_max  = 1.5;
		data->prop.y_min = -1.2;
		data->prop.y_max = 1.2;
		data->prop.zoom = 250;
		data->prop.iter = 150;
		data->prop.julia_r = 0.285;
		data->prop.julia_i = 0.01;
	}
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
	printf("key: %d\n", key);
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

int		mouse_hook(int button, int x, int y, t_mlx *data)
{
	printf("button: %d\n", button);
	printf("x: %d\n", x);
	printf("y: %d\n", y);
	if (button == ZOOM_UP)
	{
		double	mouseRe = (double)x / (WIN_X / (data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
		double	mouseIm = (double)y / (WIN_Y / (data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
		double	interpolation = 1.0 / ZOOM;
		data->prop.x_min = interpolate(mouseRe, data->prop.x_min, interpolation);
		data->prop.y_min = interpolate(mouseIm, data->prop.y_min, interpolation);
		data->prop.x_max = interpolate(mouseRe, data->prop.x_max, interpolation);
		data->prop.y_max = interpolate(mouseIm, data->prop.y_max, interpolation);
		data->prop.zoom *= ZOOM;
		data->prop.iter++;
		display(data);
	}
	else if (button == ZOOM_DOWN)
	{
		double	zoom = 1 / ZOOM;
		double	mouseRe = (double)x / (WIN_X / (data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
		double	mouseIm = (double)y / (WIN_Y / (data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
		double	interpolation = 1.0 / zoom;
		data->prop.x_min = interpolate(mouseRe, data->prop.x_min, interpolation);
		data->prop.y_min = interpolate(mouseIm, data->prop.y_min, interpolation);
		data->prop.x_max = interpolate(mouseRe, data->prop.x_max, interpolation);
		data->prop.y_max = interpolate(mouseIm, data->prop.y_max, interpolation);
		data->prop.zoom *= zoom;
		data->prop.iter--;
		display(data);
	}
	else if (button == 1)
	{
		if (data->prop.click == 0)
			data->prop.click = 1;
		else
			data->prop.click = 0;
	}
	printf("x_min: %f\n", data->prop.x_min);
	printf("x_max: %f\n", data->prop.x_max);
	printf("y_min: %f\n", data->prop.y_min);
	printf("y_max: %f\n", data->prop.y_max);
	printf("zoom: %lld\n", data->prop.zoom);
	printf("diff_x: %f\n", data->prop.x_max - data->prop.x_min);
	printf("diff_y: %f\n", data->prop.y_max - data->prop.y_min);
	return (0);
}

int		motion_hook(int x, int y, t_mlx *data)
{
	if (data->prop.click != 0)
	{
		double mouseRe = (double)x / (WIN_X / (data->prop.x_max - data->prop.x_min)) + data->prop.x_min;
		double mouseIm = (double)y / (WIN_Y / (data->prop.y_max - data->prop.y_min)) + data->prop.y_min;
		data->prop.julia_r = mouseRe;
		data->prop.julia_i = mouseIm;
		printf("julia_r: %f\n", data->prop.julia_r);
		printf("julia_i: %f\n", data->prop.julia_i);
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

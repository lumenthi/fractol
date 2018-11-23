/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 16:33:00 by lumenthi          #+#    #+#             */
/*   Updated: 2018/11/23 16:41:19 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static t_calc	init_values(t_calc c, t_mlx d)
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
	return (c);
}

void			draw_mandelbrot(t_mlx d)
{
	t_calc	c;

	c.x = 0;
	while (c.x < WIN_X)
	{
		c.y = 0;
		while (c.y < WIN_Y)
		{
			c = init_values(c, d);
			if (c.i != d.prop.iter)
				add_pixel(d, c.x, c.y, rgb_to_int(0, c.i * 350
					/ d.prop.iter, c.i * 350 / d.prop.iter));
			else
				add_pixel(d, c.x, c.y, rgb_to_int(142, 149, 160));
			c.y++;
		}
		c.x++;
	}
}

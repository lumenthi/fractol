/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 10:48:14 by lumenthi          #+#    #+#             */
/*   Updated: 2018/11/23 16:37:25 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../libft/libft.h"
# include "../minilibx/mlx.h"

# include <unistd.h>
# include <stdlib.h>
# include <math.h>

# define WIN_X 600
# define WIN_Y 600
# define WIN_TITLE "fractol"

# define ESC 53
# define ZOOM_UP 4
# define ZOOM_DOWN 5
# define RECENTER 36

# define ZOOM 1.2

typedef struct		s_image
{
	int				bpp;
	int				size_line;
	int				endian;
	void			*ptr;
	char			*data;
}					t_image;

typedef struct		s_calc
{
	double			x;
	double			y;
	double			c_r;
	double			c_i;
	double			z_r;
	double			z_i;
	double			i;
	double			tmp;
	char			*color;
}					t_calc;

typedef struct		s_prop
{
	char			*fractal;
	int				iter;
	long long		zoom;
	double			x_min;
	double			x_max;
	double			y_min;
	double			y_max;
	double			julia_r;
	double			julia_i;
	int				click;
}					t_prop;

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	t_image			img;
	t_prop			prop;
}					t_mlx;

/*
** fractol.c
*/

void				add_pixel(t_mlx d, int x, int y, int color);
int					rgb_to_int(int r, int g, int b);

/*
** mandelbrot.c
*/

void				draw_mandelbrot(t_mlx d);

/*
** julia.c
*/

void				draw_julia(t_mlx d);

/*
** burningship.c
*/

void				draw_burningship(t_mlx d);

#endif

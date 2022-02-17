#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
}			t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}			t_shape;

int		ft_strlen(char const *str)
{
	int		i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

char	*get_zone(FILE *file, t_zone *zone)
{
	int		i;
	char	*tmp;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return (NULL);
	if ((zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300))
		return (NULL);
	if (!(tmp = (char *)malloc(sizeof(char) * (zone->width *zone->height))))
		return (NULL);
	i = 0;
	while (i < (zone->width * zone->height))
		tmp[i++] = zone->background;
	return (tmp);
}

int		in_rectangle(float x, float y, t_shape *shape)
{
	if (((x < shape->x || (shape->x + shape->width < x))
	|| (y < shape->y)) || (shape->y + shape->height < y))
		return (0);
	if (((x - shape->x < 1.00000000) || ((shape->x + shape->width) - x < 1.00000000)) ||
		((y - shape->y < 1.00000000 || ((shape->y + shape->height) - y < 1.00000000))))
		return (2);
	return (1);
}

void	draw_shape(char *draw, t_shape *shape, t_zone *zone)
{
	int		x;
	int		y;
	int		in;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone ->width)
		{
			in = in_rectangle((float)x, (float)y, shape);
			if ((shape->type == 'r' && in == 2)
				|| (shape->type == 'R' && in))
				draw[(y * zone->width) + x] = shape->color;
			x++;
		}
		y++;
	}
}

int		draw_shapes(FILE *file, char *draw, t_zone *zone)
{
	t_shape	tmp;
	int		ret;

	while((ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if (tmp.width <= 0.00000000 || tmp.height <= 0.00000000
			|| (tmp.type != 'r' && tmp.type != 'R'))
			return (0);
		draw_shape(draw, &tmp, zone);
	} 
	if (ret != -1)
		return (0);
	return (1);
}

void	draw_drawing(char *draw, t_zone *zone)
{
	int		i;

	i = 0;
	while (i < zone->height)
	{
		write(1, draw + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int		str_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}

int		clear_all (FILE *file, char *draw, char const *str)
{
	if (file)
		fclose(file);
	if (draw)
		free (draw);
	if (str)
		str_error(str);
	return (1);
}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_zone	zone;
	char	*draw;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	draw = NULL;
	if (argc != 2)
		return (str_error("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (str_error("Error: Operation file corrupted\n"));
	if (!(draw = get_zone(file, &zone)))
		return (clear_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!draw_shapes(file, draw, &zone))
		return (clear_all(file, draw, "Error: Operation file corrupted\n"));
	draw_drawing(draw, &zone);
	clear_all(file, draw, NULL);
	return (0);
}
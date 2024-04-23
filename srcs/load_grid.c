/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpatsi <hpatsi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:19:17 by hpatsi            #+#    #+#             */
/*   Updated: 2024/04/23 10:59:33 by hpatsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// int init_wall_textures(t_map *map, int x, int y)
// {
// 	t_gridpos gridpos;

// 	gridpos = map->grid[y][x];
// 	gridpos.image_instances.north = mlx_image_to_window(map->mlx, map->images->north, x, y);
// 	gridpos.image_instances.south = mlx_image_to_window(map->mlx, map->images->south, x, y);
// 	gridpos.image_instances.east = mlx_image_to_window(map->mlx, map->images->east, x, y);
// 	gridpos.image_instances.west = mlx_image_to_window(map->mlx, map->images->west, x, y);
// 	return (1);
// }

void	init_player(t_map *map, int x, int y, char gridchar)
{
	map->player.x = x;
	map->player.y = y;
	if (gridchar == 'N')
		map->player.x_rotation = 0;
	if (gridchar == 'S')
		map->player.x_rotation = 180;
	if (gridchar == 'E')
		map->player.x_rotation = 90;
	if (gridchar == 'W')
		map->player.x_rotation = 270;
}

int	init_gridpos(t_map *map, int x, int y, char gridchar)
{
	t_gridpos	*gridpos;

	gridpos = &map->grid[y][x];
	gridpos->x = x;
	gridpos->y = y;
	if (gridchar == '1')
	{
		//init_wall_textures(map, x, y);
		gridpos->type = WALL;
	}
	else
		gridpos->type = EMPTY;
	if (gridchar == 'N' || gridchar == 'S'
		|| gridchar == 'E' || gridchar == 'W')
		init_player(map, x, y, gridchar);
	return (1);
}

int	load_grid_row(t_map *map, int y, char *line)
{
	int	x;

	map->grid[y] = ft_calloc(map->width + 1, sizeof(t_gridpos));
	if (map->grid[y] == 0)
		return (-1);
	x = 0;
	while (line[x] != 0 && line[x] != '\n')
	{
		init_gridpos(map, x, y, line[x]);
		x++;
	}
	while (x < map->width)
	{
		init_gridpos(map, x, y, 0);
		x++;
	}
	return (1);
}

int	load_grid(t_map *map, int map_fd)
{
	int		y;
	char	*line;

	map->grid = ft_calloc(map->height + 1, sizeof(t_gridpos *));
	if (map->grid == 0)
		return (-1);
	y = 0;
	while (y < map->height)
	{
		line = get_next_line(map_fd);
		if (line == 0)
		{
			free(line);
			free_grid(map->grid);
			return_error("Failed to read map");
		}
		if (ft_strcmp(line, "\n") == 0)
		{
			free(line);
			continue ;
		}
		if (load_grid_row(map, y, line) == -1)
		{
			free(line);
			free_grid(map->grid);
			return (-1);
		}
		free(line);
		y++;
	}
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpatsi <hpatsi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:19:17 by hpatsi            #+#    #+#             */
/*   Updated: 2024/05/16 10:55:51 by hpatsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_player(t_map *map, int x, int y, char gridchar)
{
	double	rotation_rad;

	map->player.x = x + 0.5;
	map->player.y = y + 0.5;
	if (gridchar == 'N')
		map->player.x_rotation = 0;
	if (gridchar == 'S')
		map->player.x_rotation = 180;
	if (gridchar == 'E')
		map->player.x_rotation = 90;
	if (gridchar == 'W')
		map->player.x_rotation = 270;
	rotation_rad = (map->player.x_rotation * PI) / 180;
	map->player.dir.x = sin(rotation_rad);
	map->player.dir.y = -cos(rotation_rad);
	map->player.cam_plane.x = cos(rotation_rad);
	map->player.cam_plane.y = sin(rotation_rad);
}

static int	init_gridpos(t_map *map, int x, int y, char gridchar)
{
	t_gridpos	*gridpos;

	gridpos = &map->grid[y][x];
	gridpos->x = x;
	gridpos->y = y;
	if (gridchar == '1')
		gridpos->type = WALL;
	else if (gridchar == '2')
		gridpos->type = CLOSED_DOOR;
	else
		gridpos->type = EMPTY;
	if (gridchar == 'N' || gridchar == 'S'
		|| gridchar == 'E' || gridchar == 'W')
		init_player(map, x, y, gridchar);
	return (1);
}

static int	load_grid_row(t_map *map, int y, char *line)
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

static int	handle_line(t_map *map, char *line, int gnl_error, int y)
{
	if (line == 0 || gnl_error != 0)
	{
		free_grid(map->grid);
		return (gnl_error_return(gnl_error));
	}
	if (ft_strcmp(line, "\n") == 0)
		return (1);
	if (load_grid_row(map, y, line) == -1)
	{
		free_grid(map->grid);
		return (-1);
	}
	return (0);
}

int	load_grid(t_map *map, int map_fd)
{
	int		y;
	char	*line;
	int		gnl_error;
	int		ret;

	map->grid = ft_calloc(map->height + 1, sizeof(t_gridpos *));
	if (map->grid == 0)
		return (-1);
	y = 0;
	while (y < map->height)
	{
		line = get_next_line(map_fd, &gnl_error);
		ret = handle_line(map, line, gnl_error, y);
		free(line);
		if (ret == -1)
			return (-1);
		else if (ret == 1)
			continue ;
		y++;
	}
	return (1);
}

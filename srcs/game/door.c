/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:14:34 by ixu               #+#    #+#             */
/*   Updated: 2024/05/10 16:07:26 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static bool	is_door(t_map *map, int y, int x)
{
	if (map->grid[y][x].type == CLOSED_DOOR)
		return (true);
	if (map->grid[y][x].type == OPEN_DOOR)
		return (true);
	return (false);
}

static void	record_door_pos(t_gridpos *door_gridpos, int y, int x)
{
	door_gridpos->y = y;
	door_gridpos->x = x;
}

bool	next_to_door(t_map *map, t_gridpos *door_gridpos)
{
	double	player_x;
	double	player_y;
	int	x;
	int	y;

	player_x = map->player.x;
	player_y = map->player.y;
	x = (int)player_x;
	y = (int)player_y;
	if (is_door(map, y + 1, x) && player_y + PLAYER_SIZE <= y + 1)
		record_door_pos(door_gridpos, y + 1, x);
	else if (is_door(map, y - 1, x) && player_y - PLAYER_SIZE >= y)
		record_door_pos(door_gridpos, y - 1, x);
	else if (is_door(map, y, x + 1) && player_x + PLAYER_SIZE <= x + 1)
		record_door_pos(door_gridpos, y, x + 1);
	else if (is_door(map, y, x - 1) && player_x - PLAYER_SIZE >= x)
		record_door_pos(door_gridpos, y, x - 1);
	else
		return (false);
	return (true);
}

void	toggle_door_state(t_map *map, int y, int x)
{
	if (map->grid[y][x].type == CLOSED_DOOR)
		map->grid[y][x].type = OPEN_DOOR;
	else if (map->grid[y][x].type == OPEN_DOOR)
		map->grid[y][x].type = CLOSED_DOOR;
}

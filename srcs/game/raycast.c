/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 13:05:12 by hpatsi            #+#    #+#             */
/*   Updated: 2024/05/17 13:40:33 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_raydata(t_raydata *rdata, t_vector origin, t_vector direction)
{
	rdata->grid_x = (int) origin.x;
	rdata->grid_y = (int) origin.y;
	rdata->delta_x = fabs(1 / direction.x);
	rdata->delta_y = fabs(1 / direction.y);
	if (direction.x > 0)
		rdata->dist_to_x = (rdata->grid_x + 1 - origin.x) * rdata->delta_x;
	else
		rdata->dist_to_x = (origin.x - rdata->grid_x) * rdata->delta_x;
	if (direction.y > 0)
		rdata->dist_to_y = (rdata->grid_y + 1 - origin.y) * rdata->delta_y;
	else
		rdata->dist_to_y = (origin.y - rdata->grid_y) * rdata->delta_y;
}

static void	get_next_y_edge(t_hitinfo *hit, t_vector dir, t_raydata *raydata)
{
	if (dir.y > 0)
	{
		raydata->grid_y += 1;
		hit->side = NORTH;
	}
	else
	{
		raydata->grid_y -= 1;
		hit->side = SOUTH;
	}
	hit->distance = raydata->dist_to_y;
	raydata->dist_to_y += raydata->delta_y;
}

static void	get_next_x_edge(t_hitinfo *hit, t_vector dir, t_raydata *raydata)
{
	if (dir.x > 0)
	{
		raydata->grid_x += 1;
		hit->side = WEST;
	}
	else
	{
		raydata->grid_x -= 1;
		hit->side = EAST;
	}
	hit->distance = raydata->dist_to_x;
	raydata->dist_to_x += raydata->delta_x;
}

static int	get_next_edge(t_hitinfo *hit, t_map *map, t_vector dir, \
				t_raydata *raydata)
{
	if (raydata->dist_to_y < raydata->dist_to_x)
		get_next_y_edge(hit, dir, raydata);
	else
		get_next_x_edge(hit, dir, raydata);
	if (raydata->grid_y >= map->height || raydata->grid_x >= map->width
		|| raydata->grid_y < 0 || raydata->grid_x < 0)
		return (0);
	return (1);
}

void	grid_raycast(t_hitinfo *hit, t_map *map, t_vector origin, t_vector dir)
{
	t_raydata	raydata;

	init_raydata(&raydata, origin, dir);
	while (get_next_edge(hit, map, dir, &raydata))
	{
		if (map->grid[raydata.grid_y][raydata.grid_x].type == WALL
			|| map->grid[raydata.grid_y][raydata.grid_x].type == CLOSED_DOOR)
		{
			hit->tile_info = map->grid[raydata.grid_y][raydata.grid_x];
			hit->x = origin.x + hit->distance * dir.x;
			hit->y = origin.y + hit->distance * dir.y;
			if (hit->side == NORTH || hit->side == SOUTH)
				hit->side_ratio = hit->x - floor(hit->x);
			else
				hit->side_ratio = hit->y - floor(hit->y);
			break ;
		}
		else if (map->grid[raydata.grid_y][raydata.grid_x].type == OPEN_DOOR
				&& !hit->hit_open_door)
		{
			hit->hit_open_door = true;
			hit->open_door_info = map->grid[raydata.grid_y][raydata.grid_x];
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:17:11 by hpatsi            #+#    #+#             */
/*   Updated: 2024/05/08 19:15:27 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	load_map(t_map *map, char *map_filename)
{
	int	map_fd;

	map_fd = open(map_filename, O_RDONLY);
	if (map_fd == -1)
		exit_error("Failed to open map");
	if (load_config(map, map_fd) == -1)
	{
		close(map_fd);
		exit(1);
	}
	if (load_grid(map, map_fd) == -1)
	{
		close(map_fd);
		free_textures(map->textures);
		exit(1);
	}
	close(map_fd);
	map->door_open = false;
	return (1);
}

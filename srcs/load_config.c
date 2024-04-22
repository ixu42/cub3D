/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpatsi <hpatsi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:56:33 by hpatsi            #+#    #+#             */
/*   Updated: 2024/04/22 12:49:30 by hpatsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	set_fc_color(t_map *map, char **split_line)
{
	uint32_t	color;
	char		**color_split;

	if (split_line[1] == 0)
		return_error("Failed to read color config");
	split_line[1][ft_strlen(split_line[1]) - 1] = 0;
	color_split = ft_split(split_line[1], ',');
	if (color_split == 0 || color_split[0] == 0 || color_split[1] == 0
		|| color_split[2] == 0)
	{
		free_strs(color_split);
		return_error("Failed to read color config");
	}
	color = atoi(color_split[0]);
	color = color << 8;
	color += atoi(color_split[1]);
	color = color << 8;
	color += atoi(color_split[2]);
	if (ft_strcmp(split_line[0], "F") == 0)
		map->floor_color = color;
	if (ft_strcmp(split_line[0], "C") == 0)
		map->ceiling_color = color;
	return (1);
}

int	load_image_from_config(t_map *map, char	**split_line)
{
	mlx_texture_t	*texture;
	mlx_image_t		*image;

	if (split_line[1] == 0)
		return_error("Failed to read texture config");
	split_line[1][ft_strlen(split_line[1]) - 1] = 0;
	texture = mlx_load_png(split_line[1]);
	if (texture == 0)
		return (-1);
	image = mlx_texture_to_image(map->mlx, texture);
	mlx_delete_texture(texture);
	if (image == 0)
		return (-1);
	if (ft_strcmp(split_line[0], "NO") == 0)
		map->images->north = image;
	if (ft_strcmp(split_line[0], "SO") == 0)
		map->images->south = image;
	if (ft_strcmp(split_line[0], "WE") == 0)
		map->images->west = image;
	if (ft_strcmp(split_line[0], "EA") == 0)
		map->images->east = image;
	return (1);
}

char	**read_split_line(int map_fd)
{
	char	*line;
	char	**split_line;

	line = get_next_line(map_fd);
	if (line == 0)
		return (0);
	split_line = ft_split(line, ' ');
	free(line);
	if (split_line[0] == 0)
	{
		free_strs(split_line);
		return (0);
	}
	return (split_line);
}

int	load_config(t_map *map, int map_fd)
{
	int		config_count;
	char	**split;
	int		ret;

	config_count = 0;
	while (config_count < 6)
	{
		split = read_split_line(map_fd);
		if (split == 0)
			return_error("Failed to read config");
		if (ft_strcmp(split[0], "\n") == 0)
		{
			free_strs(split);
			continue ;
		}
		if (ft_strcmp(split[0], "F") == 0 || ft_strcmp(split[0], "C") == 0)
			ret = set_fc_color(map, split);
		else
			ret = load_image_from_config(map, split);
		free_strs(split);
		if (ret == -1)
			return (-1);
		config_count++;
	}
	return (1);
}

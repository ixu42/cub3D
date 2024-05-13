/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpatsi <hpatsi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:27:31 by ixu               #+#    #+#             */
/*   Updated: 2024/05/13 12:31:36 by hpatsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	validate_file_extension(char *file)
{
	if (ft_strlen(file) < 5)
	{
		ft_putendl_fd("Invalid filename", 2);
		ft_putendl_fd("Usage: ./cub3D path_to_file.cub", 1);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(file + ft_strlen(file) - 4, ".cub") != 0)
	{
		ft_putendl_fd("Invalid filename", 2);
		ft_putendl_fd("Usage: ./cub3D path_to_file.cub", 1);
		exit(EXIT_FAILURE);
	}
}

static int	parse_file(int fd, t_map *map, int *config_flag, bool *map_started)
{
	int		last_line_before_map;
	char	*line;

	last_line_before_map = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!*map_started)
			*map_started = check_if_map_started(*config_flag, line);
		if (!*map_started)
			last_line_before_map++;
		if (*line == '\n' && !*map_started)
		{
			free(line);
			continue ;
		}
		if (!*map_started)
			validate_non_map_elements(line, config_flag);
		else
			get_map_dimensions(line, map);
		free(line);
	}
	return (last_line_before_map + 1);
}

static void	check_missing_content(int map_start_line, int config_flag, \
								bool map_started, t_map *map)
{
	bool	config_missing;

	if (map_start_line == 1)
		put_error_and_exit("Empty file or directory passed as argument\n");
	config_missing = check_if_config_missing(config_flag);
	if (config_missing || !map_started || map->width == 0)
		ft_putstr_fd("Error\n", 2);
	if (config_missing)
	{
		ft_putstr_fd("Missing config:\n", 2);
		print_missing_config(config_flag);
	}
	if (!map_started || map->width == 0)
		ft_putstr_fd("Missing map\n", 2);
	if (config_missing || !map_started || map->width == 0)
		exit(EXIT_FAILURE);
}

/*
	During the first time of reading lines of the file, 
	(1) non-map elements are validated, (2) map dimensions 
	(width and height) are obtained.

	During the second time of reading lines of the file, 
	the map content is saved to a 2D array (grid).
	Then map validation is handled with the grid.
*/

static void	validate_file_content(char *file, t_map *map)
{
	int		fd;
	int		config_flag;
	bool	map_started;
	int		map_start_line;
	char	**grid;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		perror_and_exit("open() error");
	map->width = 0;
	map->height = 0;
	config_flag = 0;
	map_started = false;
	map_start_line = parse_file(fd, map, &config_flag, &map_started);
	if (close(fd) == -1)
		perror_and_exit("close() error");
	check_missing_content(map_start_line, config_flag, map_started, map);
	grid = grid_init(file, map, map_start_line);
	validate_map(grid, map);
	if (DEBUG_MODE)
		print_grid(grid, map);
	free_char_grid(grid);
}

void	validate_input(int argc, char **argv, t_map *map)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Invalid number of arguments\n", 2);
		ft_putstr_fd("Usage: ./cub3D path_to_file.cub\n", 1);
		exit(EXIT_FAILURE);
	}
	validate_file_extension(argv[1]);
	validate_file_content(argv[1], map);
}

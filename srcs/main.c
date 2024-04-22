/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpatsi <hpatsi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 08:54:42 by hpatsi            #+#    #+#             */
/*   Updated: 2024/04/22 13:56:07 by hpatsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void test(t_map map)
{
	mlx_image_to_window(map.mlx, map.images->floor, 0, map.height / 2);
	mlx_image_to_window(map.mlx, map.images->ceiling, 0, 0);

	mlx_image_to_window(map.mlx, map.images->north, 0, 0);
	mlx_image_to_window(map.mlx, map.images->south, 128, 0);
	mlx_image_to_window(map.mlx, map.images->east, 0, 128);
	mlx_image_to_window(map.mlx, map.images->west, 128, 128);
}

int	main(int argc, char **argv)
{
	t_map	map;

	if (argc != 2)
	{
		ft_putstr_fd("Use: ./cub3D path_to_map\n", 2);
		return (1);
	}

	map.width = 512;
	map.height = 512;

	map.mlx = mlx_init(map.width, map.height, "cub3D", false);
	if (map.mlx == 0)
	{
		ft_putstr_fd("Failed to init mlx\n", 2);
		return (1);
	}

	if (load_map(&map, argv[1]) == -1)
		return (1);

	test(map);

	mlx_loop(map.mlx);

	mlx_terminate(map.mlx);

	return (0);
}

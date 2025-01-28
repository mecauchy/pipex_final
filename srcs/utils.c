/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecauchy <mecauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:43:18 by mcauchy-          #+#    #+#             */
/*   Updated: 2025/01/28 18:51:39 by mecauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*ft_is_path(char *path, char *cmd)
{
	char	*try;
	int		i;
	int		j;

	i = 0;
	j = 0;
	try = malloc(sizeof(char *) * (ft_strlen(path) + ft_strlen(cmd) + 2));
	if (!try)
		return (NULL);
	while (path[i])
	{
		try[j] = path[i];
		i++;
		j++;
	}
	//j++;
	try[j] = '/';
	i = 0;
	while (cmd[i])
	{
		try[j] = cmd[i];
		i++;
		j++;
	}
	try[j] = '\0';
	return (try);
}
char	*get_command_path(char *cmd, char **env)
{
	int		i;
	char	*get_path;
	char	*full_path;
	char	*add_slash;
	char	**cut_cmd;
	char	**path;
	i = 0;
	while (1)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			get_path = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!get_path)
	{
		ft_putendl_fd("Error: PATH not found", 2);
		exit(EXIT_FAILURE);
	}
	path = ft_split(get_path, ':');
	if (!path)
		exit(EXIT_FAILURE);
	cut_cmd = ft_split(cmd, ' ');
	if (!cut_cmd)
		return (NULL);
	i = 0;
	while (path[i])
	{
		add_slash = ft_strjoin(path[i], "/");
		if (!add_slash)
			ft_putendl_fd("malloc error", 2);
		full_path = ft_strjoin(add_slash, cut_cmd[0]);
		free(add_slash);
		// printf("path[%d]: %s\n", i, path[i]);
		// printf("*************full_path: %s\n", full_path);
		// if (!full_path)
		// 	ft_putendl_fd("malloc error", 2);
		if (access((full_path), F_OK | X_OK) == 0)
		{
			free_split(path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(path);
	return (NULL);
}

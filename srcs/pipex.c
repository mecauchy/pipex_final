/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcauchy- <mcauchy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:43:13 by mcauchy-          #+#    #+#             */
/*   Updated: 2025/01/29 16:41:15 by mcauchy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec_cmd(char *cmd, char **env)
{
	char	**cmd_args;
	char	*cmd_path;

	if (ft_only_sep(cmd, ' '))
		ft_error("command not found");
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		free_split(cmd_args);
		free(cmd_path);
		ft_putendl_fd("command not found", 2);
		exit(127);
	}
	if (execve(cmd_path, cmd_args, env) == -1)
	{
		free_split(cmd_args);
		free(cmd_path);
		ft_perror("execve cmd error");
	}
}

void	first_cmd_exec(int *fd, char **av, char **env)
{
	int		infile;

	infile = open(av[1], O_RDONLY, 0777);
	if (infile < 0)
	{
		close(fd[0]);
		close(fd[1]);
		ft_perror("open infile error");
	}
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(infile);
	close(fd[0]);
	close(fd[1]);
	exec_cmd(av[2], env);
}

void	second_cmd_exec(int *fd, char **av, char **env)
{
	int	outfile;

	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close(fd[0]);
		close(fd[1]);
		ft_perror("open outfile error");
	}
	dup2(outfile, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(outfile);
	exec_cmd(av[3], env);
}

void	pipex(int ac, char **av, char **env)
{
	int	fd[2];
	int	pid1;

	if (ac < 5)
		ft_error("error : arguments");
	if (pipe(fd) == -1)
		ft_perror("pipe error");
	pid1 = fork();
	if (pid1 == -1)
		ft_perror("pid1 error");
	if (pid1 == 0)
		first_cmd_exec(fd, av, env);
	waitpid(pid1, NULL, 0);
	second_cmd_exec(fd, av, env);
	close(fd[0]);
	close(fd[1]);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 5 || !av[2][0] || !av[3][0])
		ft_error("error : arguments");
	pipex(ac, av, env);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecauchy <mecauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:43:13 by mcauchy-          #+#    #+#             */
/*   Updated: 2025/01/28 21:54:17 by mecauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec_cmd(char *cmd, char **env)
{
	char	**cmd_args;
	char	*cmd_path;

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
void    first_cmd_exec(int *fd, char **av,char **env)
{
	int		infile;
	// char	**cmd_args;
	// char	*cmd_path;

	infile = open(av[1], O_RDONLY, 0777);
	if (infile < 0)
		ft_perror("open infile error");
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	// close(fd[1]);
	// close(infile);
	exec_cmd(av[2], env);
}


void    second_cmd_exec(int *fd, char **av, char **env)
{
	int		outfile;
	// char	**cmd_args;
	// char	*cmd_path;

	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		ft_perror("open outfile error");
	dup2(outfile, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	// dup2(fd[0], STDIN_FILENO);
	// dup2(outfile, STDOUT_FILENO);
	// close(fd[0]);
	close(fd[1]);
	// close(outfile);
	exec_cmd(av[3], env);
}

void pipex(int ac, char **av, char **env)
{
	int fd[2];
	int pid1;
	// int pid2;

	if (ac < 5)
	{
		ft_putendl_fd("error : arguments", 2);
		exit(1);
	}
	if (pipe(fd) == -1)
		ft_perror("pipe error");
	pid1 = fork();
	if (pid1 == -1)
		ft_perror("[PID1] ERROR");
	if (pid1 == 0)
		first_cmd_exec(fd, av, env);
	waitpid(pid1, NULL, 0);
	// pid2 = fork();
	// if (pid2 == -1)
	// 	ft_perror("[PID2] ERROR");
	// if (pid2 == 0)
	second_cmd_exec(fd, av, env);
	// close(fd[0]);
	// close(fd[1]);
	// waitpid(pid2, NULL, 0);
}

int main(int ac, char **av, char **env)
{
	if (ac != 5)
	{
		ft_putendl_fd("error : arguments", 2);
		exit(1);	
	}
	pipex(ac, av, env);
	return (0);
}

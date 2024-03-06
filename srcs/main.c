/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/07 00:23:43 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		d;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(2, ERR_ARG), 1);
	//
	open_files(argv[1], argv[argc - 1], &pipex);
	//
	d = dup2(pipex.fd1, STDIN);
	close(pipex.fd1);
	if (d == -1)
		return (perror("dup2 infile"), 1);
	d = dup2(pipex.fd2, STDOUT);
	close(pipex.fd2);
	if (d == -1)
		return (perror("dup2 outfile"), 1);
	//
	fork_child(pipex, argv, envp, 1);
	exec_arg(pipex, argv, envp, 2);
	return (0);
}

void	open_files(char *file1, char *file2, t_pipex *pipex)
{
	if (access(file1, R_OK) == 0)
	{
		(*pipex).fd1 = open(file1, O_RDONLY);
		if ((*pipex).fd1 < 0)
		{
			perror("open infile");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("access");
		ft_putstr_fd(2, ERR_ACC);
		exit(EXIT_FAILURE);
	}
	(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if ((*pipex).fd2 < 0)
	{
		perror("open outfile");
		close((*pipex).fd1);
		exit(EXIT_FAILURE);
	}
	if (access(file2, W_OK) != 0)
	{
		perror("access");
		ft_putstr_fd(2, ERR_ACC);
		close((*pipex).fd1);
		close((*pipex).fd2);
		exit(EXIT_FAILURE);
	}
}

// waitpid(-1, NULL, WNOHANG); //waitpid(pid, &(pipex.status), 0);
void	fork_child(t_pipex pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	printf("child\n");
	if (pipe(pipex.fd) == -1)
	{
		perror("pipe");
		close_exit(pipex, 1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close_exit(pipex, 1);
	}
	if (pid == 0)
	{
		close(pipex.fd[0]);
		dup2(pipex.fd[1], STDOUT);
		close(pipex.fd[1]);
		exec_arg(pipex, argv, envp, k);
	}
	else
	{
		close(pipex.fd[1]);
		dup2(pipex.fd[0], STDIN);
		close(pipex.fd[0]);
		// waitpid(pid, NULL, 0);
		waitpid(pid, &(pipex.status), 0);
	}
}

// int execve(const char *pathname, char *const argv[], char *const envp[]);
void	exec_cmd(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;
	char	*path_cmd;

	get_paths(envp, &pipex);
	cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
	{
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex, 1);
	}
	path_cmd = check_path(pipex.paths, cmd);
	if (!path_cmd)
	{
		ft_putstr_fd(2, ERR_CMD);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex, 127);
	}
	if (execve(path_cmd, cmd, envp) == -1)
	{
		perror("execve");
		free(path_cmd);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex, 127);
	}
}

void	exec_abs(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;

	cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
	{
		free_cmd(cmd);
		close_exit(pipex, 1);
	}	
	
	if (access(cmd[0], X_OK) != 0)
	{
		ft_putstr_fd(2, ERR_CMD);
		free_cmd(cmd);
		close_exit(pipex, 127);
	}
	if (execve(cmd[0], cmd, envp) == -1)
	{
		perror("execve");
		free_cmd(cmd);
		close_exit(pipex, 127);
	}
}

void	exec_arg(t_pipex pipex, char **argv, char **envp, int k)
{
	if (check_slash(argv[k + 1]) == 1)
	{
		write(2, "ici\n", 4);
		exec_abs(pipex, argv, envp, k);
	}
	else
	{
		write(2, "la\n", 3);
		exec_cmd(pipex, argv, envp, k);
	}
}

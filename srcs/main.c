/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/06 00:48:19 by caguillo         ###   ########.fr       */
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
	open_files(argv[1], argv[argc - 1], &pipex);
	d = dup2(pipex.fd1, STDIN);
	close(pipex.fd1);
	if (d == -1)
		return (perror("dup2 infile"), 1);
	d = dup2(pipex.fd2, STDOUT);
	close(pipex.fd2);
	if (d == -1)
		return (perror("dup2 outfile"), 1);
	fork_child(pipex, argv, envp, 1);
	exec_cmd(pipex, argv, envp, 2);
	// free_paths(&pipex);
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
			// close((*pipex).fd1);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("access");
		ft_putstr_fd(2, ERR_RO);
	 	exit(EXIT_FAILURE);
	}
	(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if ((*pipex).fd2 < 0)
	{
		perror("open outfile");
		close((*pipex).fd1);
		// close((*pipex).fd2);
		exit(EXIT_FAILURE);
	}
}

// waitpid(-1, NULL, WNOHANG);
void	fork_child(t_pipex pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	// int		fd[2];
	if (pipe(pipex.fd) == -1)
	{
		perror("pipe");
		close_exit(pipex);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close_exit(pipex);
	}
	if (pid == 0)
	{
		close(pipex.fd[0]);
		dup2(pipex.fd[1], STDOUT);
		close(pipex.fd[1]);
		exec_cmd(pipex, argv, envp, k);
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
		close_exit(pipex);
	}
	path_cmd = check_path(pipex.paths, cmd);
	if (!path_cmd)
	{
		ft_putstr_fd(2, ERR_CMD);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex);
	}
	if (execve(path_cmd, cmd, envp) == -1)
	{
		perror("execve");
		free(path_cmd);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex);
	}
}

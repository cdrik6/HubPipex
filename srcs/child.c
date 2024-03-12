/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/12 23:51:22 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_in(t_pipex *pipex, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_close_exit("pipex: fork", *pipex, 1);
	if (pid == 0)
	{
		open_infile(argv[1], pipex);
		//
		close((*pipex).fd[0]);
		//
		dup2((*pipex).fd_in, STDIN);
		close((*pipex).fd_in);
		//
		dup2((*pipex).fd[1], STDOUT);
		close((*pipex).fd[1]);
		exec_arg((*pipex), argv, envp, 1);
	}
	// close((*pipex).fd[1]);
}

void	child_out(t_pipex *pipex, char **argv, char **envp, int argc)
{
	(*pipex).pid = fork();
	if ((*pipex).pid == -1)
		perror_close_exit("pipex: fork", (*pipex), 1);
	if ((*pipex).pid == 0)
	{
		open_outfile(argv[argc - 1], pipex);
		//
		close((*pipex).fd[1]);
		//
		dup2((*pipex).fd[0], STDIN);
		close((*pipex).fd[0]);
		//
		dup2((*pipex).fd_out, STDOUT);
		close((*pipex).fd_out);
		exec_arg((*pipex), argv, envp, argc - 3);
	}
	close((*pipex).fd[0]);
	close((*pipex).fd[1]);
}

void	exec_arg(t_pipex pipex, char **argv, char **envp, int k)
{
	if (check_slash(argv[k + 1]) == 1)
		exec_abs(pipex, argv, envp, k);
	else
		exec_cmd(pipex, argv, envp, k);
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
		close_exit(pipex, EXIT_FAILURE);
	}
	path_cmd = check_path(pipex.paths, cmd);
	if (!path_cmd)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(ERR_CMD, 2);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex, EXIT_NOCMD);
	}
	// if (ft_strlen(path_cmd) == 0)
	// {
	// 	ft_putstr_fd(ERR_ACC, 2);		
	// 	free_cmd(cmd);
	// 	free_paths(&pipex);
	// 	close_exit(pipex, EXIT_DENIED);
	// }
	if (execve(path_cmd, cmd, envp) == -1)
	{
		perror("execve");
		free(path_cmd);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(pipex, EXIT_FAILURE);
	}
}

// write(2, cmd[0], ft_strlen(cmd[0]));
void	exec_abs(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;

	cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
	{
		free_cmd(cmd);
		close_exit(pipex, EXIT_FAILURE);
	}
	if (access(cmd[0], X_OK) != 0)
	{
		if (access(cmd[0], F_OK) != 0)
		{
			ft_putstr_fd("pipex: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd(ERR_DIR, 2);
			free_cmd(cmd);
			close_exit(pipex, EXIT_NODIR);
		}
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(ERR_ACX, 2);
		free_cmd(cmd);
		close_exit(pipex, EXIT_DENIED);
	}
	if (execve(cmd[0], cmd, envp) == -1)
	{
		perror("execve");
		free_cmd(cmd);
		close_exit(pipex, EXIT_FAILURE);
	}
}

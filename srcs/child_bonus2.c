/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/17 20:28:27 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	child_in(t_pipex *pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_close_exit("pipex: fork", *pipex, 1);
	if (pid == 0)
	{
		open_infile(argv[1], pipex);
		close((*pipex).fd[0]);
		
		dup2((*pipex).fd_in, STDIN);
		close((*pipex).fd_in);
		
		dup2((*pipex).fd[1], STDOUT);
		close((*pipex).fd[1]);
		
		exec_arg((*pipex), argv, envp, k);
	}	
	dup2((*pipex).fd[0], STDIN);
	close((*pipex).fd[0]);	
	close((*pipex).fd[1]);
}

void	child(t_pipex *pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_close_exit("pipex: fork", *pipex, 1);
	if (pid == 0)
	{
		close((*pipex).fd[0]);
		//
		dup2((*pipex).fd[1], STDOUT);
		close((*pipex).fd[1]);
		//
		exec_arg((*pipex), argv, envp, k);
	}	
	close((*pipex).fd[1]);
	dup2((*pipex).fd[0], STDIN);
	close((*pipex).fd[0]);	
}

void	child_out(t_pipex *pipex, char **argv, char **envp, int argc)
{
	(*pipex).pid = fork();
	//ft_putnbr_fd((*pipex).pid, 2);
	if ((*pipex).pid == -1)
		perror_close_exit("pipex: fork", (*pipex), 1);
	if ((*pipex).pid == 0)
	{
		open_outfile(argv[argc - 1], pipex);
		close((*pipex).fd[1]);
		//close((*pipex).fd[0]);								
		//dup2((*pipex).fd[0], STDIN);		
		close((*pipex).fd[0]);		
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
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 1);
	path_cmd = check_path(pipex.paths, cmd);
	if (!path_cmd)
	{
		putstr_error(cmd[0], ERR_CMD);
		free_close_exit(cmd, &pipex, EXIT_NOCMD, 1);
	}
	if (execve(path_cmd, cmd, envp) == -1)
	{
		perror("execve");
		free(path_cmd);
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 1);
	}
}

void	exec_abs(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;

	cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 0);
	if (access(cmd[0], X_OK) != 0)
	{
		if (access(cmd[0], F_OK) != 0)
		{
			putstr_error(cmd[0], ERR_DIR);
			free_close_exit(cmd, &pipex, EXIT_NODIR, 0);
		}
		putstr_error(cmd[0], ERR_ACX);
		free_close_exit(cmd, &pipex, EXIT_DENIED, 0);
	}
	if (execve(cmd[0], cmd, envp) == -1)
	{
		perror("execve");
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 0);
	}
}

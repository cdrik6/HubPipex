/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:13:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/04 21:18:58 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// int execve(const char *pathname, char *const argv[], char *const envp[]);
void	child(t_pipex pipex, char **argv, char **envp)
{
	dup2(pipex.fd[1], STDOUT);
	close(pipex.fd[0]);
	dup2(pipex.fd1, STDIN);	
	pipex.cmd = ft_split(argv[2], ' ');
	if (!pipex.cmd)
	{
		free_cmd(&pipex);
		free_paths(&pipex);
		close_pipe(pipex);
		close_exit(pipex);
	}
	pipex.path_cmd = check_path(pipex.paths, pipex.cmd);
	// if NULL
	if (execve(pipex.path_cmd, pipex.cmd, envp) == -1)
	{
		perror("execve");
		free_cmd(&pipex);
		free(pipex.path_cmd);
		free_paths(&pipex);
		close_pipe(pipex);
		close_exit(pipex);
	}
	// close(pipex.fd[0]);
	close(pipex.fd[0]);
}

void	child2(t_pipex pipex, char **argv, char **envp)
{
	close(pipex.fd[1]);
	dup2(pipex.fd[0], STDIN);
	dup2(pipex.fd2, STDOUT);
	pipex.cmd = ft_split(argv[3], ' ');
	if (!pipex.cmd)
	{
		free_cmd(&pipex);
		free_paths(&pipex);
		close_pipe(pipex);
		close_exit(pipex);
	}
	pipex.path_cmd = check_path(pipex.paths, pipex.cmd);
	// if NULL
	if (execve(pipex.path_cmd, pipex.cmd, envp) == -1)
	{
		perror("execve");
		free_cmd(&pipex);
		free(pipex.path_cmd);
		free_paths(&pipex);
		close_pipe(pipex);
		close_exit(pipex);
	}
	close(pipex.fd[1]);
	// close(pipex.fd[1]);
}

// Only the last paths[i] is NULL
char	*check_path(char **paths, char **cmd)
{
	int		i;
	char	*tmp;

	if (!paths || !cmd)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], cmd[0]);
		if (!tmp)
			return (NULL);
		if (access(tmp, F_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

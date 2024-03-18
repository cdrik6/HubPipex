/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 01:42:22 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// int execve(const char *pathname, char *const argv[], char *const envp[]);
void	exec_cmd(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;
	char	*path_cmd;

	get_paths(envp, &pipex);
	cmd = ft_split(argv[k], ' ');
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
		perror("pipex: execve");
		free(path_cmd);
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 1);
	}
}

void	exec_abs(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;

	cmd = ft_split(argv[k], ' ');
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
		perror("pipex: execve");
		free_close_exit(cmd, &pipex, EXIT_FAILURE, 0);
	}
}

void	fill_here_doc(t_pipex *pipex)
{
	char	*line;
	char	*limiter;

	limiter = ft_strjoin((*pipex).lim, "\n");
	if (!limiter)
		limiter_err_mal();
	while (1)
	{
		ft_putstr_fd("> ", STDIN);
		line = get_next_line(STDIN);
		if (!line)
		{
			ft_putstr_fd(ERR_GNL, STDERR);
			exit(EXIT_FAILURE);
		}
		if (ft_strcmp(line, limiter) == 0)
			break ;
		else
			ft_putstr_fd(line, (*pipex).docfd[1]);
		free(line);
	}
	free(line);
	free(limiter);
	close((*pipex).docfd[1]);
}

void	limiter_err_mal(void)
{
	ft_putstr_fd(ERR_MAL, STDERR);
	exit(EXIT_FAILURE);
}

void	perror_open(t_pipex pipex, char *filename)
{
	char	*tmp;

	tmp = ft_strjoin("pipex: ", filename);
	perror(tmp);
	free(tmp);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	exit(EXIT_FAILURE);
}

// void	here_doc(t_pipex *pipex, char **argv, char **envp, int k)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 		perror_close_exit("pipex: fork", *pipex, 1);
// 	if (pid == 0)
// 	{
// 		close((*pipex).fd[0]);
// 		dup2((*pipex).docfd[0], STDIN);
// 		close((*pipex).docfd[0]);
// 		dup2((*pipex).fd[1], STDOUT);
// 		close((*pipex).fd[1]);
// 		exec_arg((*pipex), argv, envp, k);
// 	}
// 	close((*pipex).docfd[0]);
// 	close((*pipex).fd[1]);
// 	dup2((*pipex).fd[0], STDIN);
// 	close((*pipex).fd[0]);
// }

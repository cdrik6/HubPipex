/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/17 23:46:42 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

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

void	fill_here_doc(t_pipex *pipex)
{
	char	*line;
	char	*limiter;

	limiter = ft_strjoin((*pipex).lim, "\n");
	if (!limiter)
	{
		ft_putstr_fd(ERR_MAL, STDERR);
		exit(EXIT_FAILURE);
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:42:32 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 01:13:09 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_exit(t_pipex pipex, int k)
{
	close(pipex.fd_in);
	close(pipex.fd_out);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	exit(k);
}

void	perror_close_exit(char *err, t_pipex pipex, int k)
{
	perror(err);
	close_exit(pipex, k);
}

void	free_paths(t_pipex *pipex)
{
	int	i;

	i = 0;
	if ((*pipex).paths)
	{
		while ((*pipex).paths[i])
		{
			free((*pipex).paths[i]);
			i++;
		}
		free((*pipex).paths);
	}
}

void	free_cmd(char **cmd)
{
	int	i;

	i = 0;
	if (cmd)
	{
		while (cmd[i])
		{
			free(cmd[i]);
			i++;
		}
		free(cmd);
	}
}

void	free_close_exit(char **cmd, t_pipex *pipex, int exit_code, int is_paths)
{
	free_cmd(cmd);
	if (is_paths == 1)
		free_paths(pipex);
	close_exit(*pipex, exit_code);
}

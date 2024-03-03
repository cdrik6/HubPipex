/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:42:32 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/03 23:12:50 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_exit(t_pipex pipex)
{
	close(pipex.fd1);
	close(pipex.fd2);
	exit(EXIT_FAILURE);
}

void	close_pipe(t_pipex pipex)
{
	close(pipex.fd[0]);
	close(pipex.fd[1]);
}

void	free_paths(t_pipex *pipex)
{
	int	i;

	i = 0;
	while ((*pipex).paths[i])
	{
		free((*pipex).paths[i]);
		i++;
	}
	free((*pipex).paths);
}

void	free_cmd(t_pipex *pipex)
{
	int	i;

	i = 0;
	while ((*pipex).cmd[i])
	{
		free((*pipex).cmd[i]);
		i++;
	}
	free((*pipex).cmd);
}

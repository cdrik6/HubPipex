/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:42:32 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/08 01:49:00 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	perror_close_exit(char *err, t_pipex pipex, int k)
{
	perror(err);
	close(pipex.fd1);
	close(pipex.fd2);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	exit(k);
}

void	close_exit(t_pipex pipex, int k)
{
	close(pipex.fd1);
	close(pipex.fd2);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	if (WEXITSTATUS(pipex.status) != 0)
	{
		// ft_putstr_fd(2, "exit status\n");
		// ft_putnbr_fd(WEXITSTATUS(pipex.status), 2);
		exit(WEXITSTATUS(pipex.status));
	}
	else if (pipex.err_outf != 0)
		exit(pipex.err_outf);
	else
		exit(k);
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

// void	close_pipe(t_pipex pipex)
// {
// 	close(pipex.fd[0]);
// 	close(pipex.fd[1]);
// }

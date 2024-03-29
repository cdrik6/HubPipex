/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/17 19:10:43 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(ERR_ARG, 2), ft_putstr_fd(USAGE, 2), 1);
	i = 0;
	while (i < argc - 3)
	{
		if (pipe(pipex.fd) == -1)
			perror_close_exit("pipex: pipe", pipex, 1);		
		if (i == 0)
			child_in(&pipex, argv, envp, 1);
		else if (i == argc - 4)
			child_out(&pipex, argv, envp, argc);
		else
			child(&pipex, argv, envp, i + 1);
		i++;
	}
	return (wait_exitcode(pipex));
}

// if exist and readable --> open
// else exit (from child)
void	open_infile(char *infile, t_pipex *pipex)
{
	(*pipex).fd_in = open(infile, O_RDONLY);
	if ((*pipex).fd_in < 0)
		perror_open(*pipex, infile);
}

// if don't exist --> open/create with write
// if exist in write mode --> open/trunc
// if write not possible--> error + exit
void	open_outfile(char *outfile, t_pipex *pipex)
{
	(*pipex).fd_out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if ((*pipex).fd_out < 0)
		perror_open(*pipex, outfile);
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

// while child in progress
// while (waitpid(-1, NULL, 0) != -1)
int	wait_exitcode(t_pipex pipex)
{
	int	exitcode;

	while (errno != ECHILD)
	{
		if (wait(&pipex.status) == pipex.pid)
		{
			if (WIFEXITED(pipex.status))
				exitcode = WEXITSTATUS(pipex.status);
		}
	}
	// if (errno == ECHILD)
	// 	ft_putstr_fd("ici\n", 2);
	return (exitcode);
}

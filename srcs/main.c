/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 01:35:47 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex = (t_pipex){0};
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (argc < 6)
			return (ft_putstr_fd(ERR_ARG, 2), ft_putstr_fd(USAGE_DOC, 2),
				EXIT_ARG);
		pipex.is_heredoc = 1;
		pipex.lim = argv[2];
		if (pipe(pipex.docfd) == -1)
			perror_close_exit("pipex: pipe", pipex, EXIT_FAILURE);
		fill_here_doc(&pipex);
	}
	else
	{
		if (argc < 5)
			return (ft_putstr_fd(ERR_ARG, 2), ft_putstr_fd(USAGE, 2), EXIT_ARG);
	}
	children(&pipex, argv, envp, argc - 3 - pipex.is_heredoc);
	return (wait_exitcode(pipex));
}

void	children(t_pipex *pipex, char **argv, char **envp, int nbr_cmd)
{
	int	i;

	i = 0;
	while (i < nbr_cmd)
	{
		if (pipe((*pipex).fd) == -1)
			perror_close_exit("pipex: pipe", *pipex, EXIT_FAILURE);
		if (i == 0)
			child_in(pipex, argv, envp, i + 2 + (*pipex).is_heredoc);
		else if (i == nbr_cmd - 1)
			child_out(pipex, argv, envp, i + 2 + (*pipex).is_heredoc);
		else
			child(pipex, argv, envp, i + 2 + (*pipex).is_heredoc);
		i++;
	}
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
// 666 rw for all
// >> = append
void	open_outfile(char *outfile, t_pipex *pipex)
{
	if ((*pipex).is_heredoc == 1)
		(*pipex).fd_out = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		(*pipex).fd_out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if ((*pipex).fd_out < 0)
		perror_open(*pipex, outfile);
}

// while child in progress
// while (waitpid(-1, NULL, 0) != -1)
int	wait_exitcode(t_pipex pipex)
{
	int	exitcode;

	exitcode = 0;
	while (errno != ECHILD)
	{
		if (wait(&pipex.status) == pipex.pid)
		{
			if (WIFEXITED(pipex.status))
				exitcode = WEXITSTATUS(pipex.status);
		}
	}
	return (exitcode);
}

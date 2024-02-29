/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/02/29 23:31:01 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv)
{
	if (argc != 5)
		return (error_msg(0), 1);
}

//int execve(const char *pathname, char *const argv[], char *const envp[]);

void	pipex(t_pipex pipex)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		// exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		// exit(EXIT_FAILURE);
	}
    if (pid == 0)
    {
        dup2(pipex.fd_in, STDIN);    

        char* argv[] = { "jim", "jams", NULL };
        char* envp[] = { "some", "environment", NULL };
        if (execve("./sub", argv, envp) == -1)
            perror("execve");
    }
    else
    {
        dup2(pipex.fd_out, STDOUT);    
    }

    
	

    
}

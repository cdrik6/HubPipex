/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:57:35 by caguillo          #+#    #+#             */
/*   Updated: 2024/02/28 00:08:04 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	dup2_test(void);
void	access_test(void);
void	execve_test(void);
void	fork_test(void);

int	main(void)
{
	// dup2_test();
	// access_test();
	// execve_test();
    fork_test();
	return (0);
}

void	dup2_test(void)
{
	int	fd;

	fd = open("dup2.txt", O_WRONLY | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("inside dup2\n");
	printf("toto inside dup2\n");
}

void	access_test(void)
{
	printf("R = %d\n", access("dup2.txt", R_OK));
	printf("W = %d\n", access("dup2.txt", W_OK));
	printf("X = %d\n", access("dup2.txt", X_OK));
	if (access("dup2.txt", R_OK) != -1)
		printf("I have permission\n");
	else
		printf("I don't have permission\n");
}

void	execve_test(void)
{
	char	*args[3];

	args[0] = "ls";
	args[1] = "-l";
	args[2] = NULL;
	execve("/bin/ls", args, NULL);
	printf("This line will not be executed.\n");
}

void	fork_test(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
    
    if (pid == 0)
		printf("This is the child process. (pid: %d)\n", getpid());
	else
		printf("This is the parent process. (pid: %d)\n", getpid());
}
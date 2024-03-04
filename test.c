/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:57:35 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/04 21:35:05 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	dup2_test(void);
void	access_test(void);
void	execve_test(void);
// void	execve_test2(void);
void	fork_test(void);
int		ft_strlen(char *str);
void	pipe_test(void);
void	unlink_test(void);
void	fork_unlink_test(void);
void	wait_test(void);

int	main(void)
{
	// access_test();
	// execve_test();
	// fork_test();
	// dup2_test();
	// printf("toto");
	// pipe_test();
	// unlink_test();
	// fork_unlink_test();
	wait_test();
	return (0);
}

void	dup2_test(void)
{
	int	fd;

	fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("fd %d\n", fd);
	printf("inside dup2\n");
	printf("toto inside dup2\n");
	printf("write %ld\n", write(3, "write sur 3\n",
			ft_strlen("write sur 3\n")));
	write(1, "write sur 1\n", ft_strlen("write sur 1\n"));
}

void	access_test(void)
{
	int	result;

	// printf("R = %d\n", access("dup2.txt", R_OK));
	// printf("W = %d\n", access("dup2.txt", W_OK));
	// printf("X = %d\n", access("dup2.txt", X_OK));
	// if (access("dup2.txt", R_OK) != -1)
	// 	printf("I have permission\n");
	// else
	// 	printf("I don't have permission\n");
	result = access("sample.txt", F_OK);
	if (result == -1)
	{
		printf("Error Number: %d\n", errno);
		perror("Error Description");
	}
	else
	{
		printf("No error\n");
	}
}

void	execve_test(void)
{
	char	*args[3];

	args[0] = "ls";
	args[1] = "-l";
	args[2] = NULL;
	printf("%d\n", execve("/bin/ls", args, NULL));
	printf("This line will not be executed.\n");
}

// void	execve_test2(void)
// {
// 	printf("Main program started\n");
//     char* argv[] = { "jim", "jams", NULL };
//     char* envp[] = { "some", "environment", NULL };
//     if (execve("./sub", argv, envp) == -1)
//         perror("Could not execve");
// }

void	fork_test(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	// printf("ici %d\n", pid);
	if (pid == 0)
	{
		printf("This is the child process. (pid: %d)\n", getpid());
		// printf("%d\n", pid);
	}
	else
	{
		printf("This is the parent process. (pid: %d)\n", getpid());
		// printf("%d\n", pid);
	}
}

void	pipe_test(void)
{
	int		fd[2];
	pid_t	pid;
	char	buffer[14];
	int		i;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	printf("f0 %d\n", fd[0]);
	printf("f1 %d\n", fd[1]);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	printf("ici %d\n", pid);
	if (pid == 0)
	{
		close(fd[0]); // close the read end of the pipe
		write(fd[1], "Hello parent!", 13);
		close(fd[1]); // close the write end of the pipe
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fd[1]); // close the write end of the pipe
		i = 0;
		while (i < 13)
		{
			read(fd[0], &buffer[i], 1);
			i++;
		}
		buffer[13] = 0;
		close(fd[0]); // close the read end of the pipe
		printf("Message from child: '%s'\n", buffer);
		exit(EXIT_SUCCESS);
	}
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	unlink_test(void)
{
	if (unlink("test copy.txt") == 0)
		printf("File successfully deleted");
	else
		printf("Error deleting file");
}

void	fork_unlink_test(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	// printf("ici %d\n", pid);
	if (pid == 0)
	{
		printf("This is the child process. (pid: %d)\n", getpid());
		if (unlink("test copy.txt") == 0)
			printf("child - File successfully deleted\n");
		else
			printf("child - Error deleting file\n");
		// printf("%d\n", pid);
	}
	else
	{
		printf("This is the parent process. (pid: %d)\n", getpid());
		if (unlink("test copy.txt") == 0)
			printf("parent - File successfully deleted\n");
		else
			printf("parent - Error deleting file\n");
		// printf("%d\n", pid);
	}
}

void	wait_test(void)
{
	pid_t	pid;
	int		k;

	pid = fork();
	k = 0;
	while (k < 100)
	{
		printf("%d - %d\n", k, pid);
		k++;
	}
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		printf("I am the child process.\n");
		sleep(2);
		// exit(EXIT_SUCCESS);
	}
	else
	{
		printf("I am the parent process.\n");
		// printf("avant - %d\n", pid);
		wait(NULL);
		printf("apres - %d\n", pid);
		printf("Child process terminated after a 2s delay.\n");
	}
	// return (EXIT_SUCCESS);
}

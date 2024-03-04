/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/04 23:56:39 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(2, ERR_ARG), 1);
	open_files(argv[1], argv[argc - 1], &pipex);
	if (dup2(pipex.fd1, STDIN) == -1)
		return (perror("dup2 infile"), 1);
	if (dup2(pipex.fd2, STDOUT) == -1)
		return (perror("dup2 outfile"), 1);
	fork_child(pipex, argv, envp, 1);
	exec_cmd(pipex, argv, envp, 2);
	close_pipe(pipex);
	// get_paths(envp, &pipex);
}

void	open_files(char *file1, char *file2, t_pipex *pipex)
{
	(*pipex).fd1 = open(file1, O_RDONLY);
	if ((*pipex).fd1 < 0)
	{
		perror("open infile");
		close((*pipex).fd1);
		exit(EXIT_FAILURE);
	}
	(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if ((*pipex).fd2 < 0)
	{
		perror("open outfile");
		close((*pipex).fd1);
		close((*pipex).fd2);
		exit(EXIT_FAILURE);
	}
}

void	fork_child(t_pipex pipex, char **argv, char **envp, int k)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		close_exit(pipex);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close_exit(pipex);
	}
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT);
		exec_cmd(pipex, argv, envp, k);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
		// waitpid(-1, NULL, WNOHANG);
		waitpid(pid, NULL, 0);
	}
}

// int execve(const char *pathname, char *const argv[], char *const envp[]);
void	exec_cmd(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;
	char	*path_cmd;

	get_paths(envp, &pipex);
	cmd = ft_split(argv[k + 1], ' ');
	// if (!cmd)
	// {
	// 	free_cmd(&pipex);
	// 	free_paths(&pipex);
	// 	close_pipe(pipex);
	// 	close_exit(pipex);
	// }
	path_cmd = check_path(pipex.paths, cmd);
	// if NULL
	execve(path_cmd, cmd, envp);
	// if (execve(path_cmd, cmd, envp) == -1)
	// {
	// 	perror("execve");
	// 	free_cmd(&pipex);
	// 	free(pipex.path_cmd);
	// 	free_paths(&pipex);
	// 	close_pipe(pipex);
	// 	close_exit(pipex);
	// }
}

// // checking
// printf("fd1 %d\n", pipex.fd1);
// printf("fd2 %d\n", pipex.fd2);
// // i = 0;
// // while (pipex.paths[i])
// // {
// // 	printf("%s\n", pipex.paths[i]);
// // 	free(pipex.paths[i]);
// // 	i++;
// // }
// free(pipex.paths);
// // free(pipex.cmd1[0]);
// free(pipex.cmd1);
// // free(pipex.cmd2[0]);
// free(pipex.cmd2);
// close(pipex.fd1);
// close(pipex.fd2);

// void	error_msg(int k)
// {
// 	if (k == 0)
// 		ft_putchar_fd(2, ERR_ARG);
// }

// void	get_cmds(char **argv, t_pipex *pipex)
// {
// 	(*pipex).cmd1 = ft_split(argv[2], ' ');
// 	(*pipex).cmd2 = ft_split(argv[3], ' ');
// }

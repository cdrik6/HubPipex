/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/03 22:42:18 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	pid;
	int		k;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(2, ERR_ARG), 1);
	open_files(argv[1], argv[argc - 1], &pipex);
	if (pipe(pipex.fd) == -1)
	{
		perror("pipe");
		close_exit(pipex);
	}
	get_paths(envp, &pipex);
	k = 0;
	while (k < 2)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_paths(pipex.paths);
			close_exit(pipex);
		}
		if (pid == 0)
			child(pipex, argv, envp, k);
	}
}

void	open_files(char *file1, char *file2, t_pipex *pipex)
{
	(*pipex).fd1 = open(file1, O_RDONLY);
	if ((*pipex).fd1 < 0)
	{
		perror("open file1");
		close((*pipex).fd1);
		exit(EXIT_FAILURE);
	}
	(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if ((*pipex).fd2 < 0)
	{
		perror("open file2");
		close_exit(*pipex);
	}
}

void	get_paths(char **envp, t_pipex *pipex)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i] && (check_instr(envp[i], "PATH=") == 0))
		i++;
	path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	if (!path)
		close_exit(*pipex);
	(*pipex).paths = ft_split(path, ':');
	free(path);
	if (!(*pipex).paths)
		close_exit(*pipex);
	slash_paths(pipex);
}

void	slash_paths(t_pipex *pipex)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((*pipex).paths[i])
	{
		tmp = ft_strdup((*pipex).paths[i]);
		free((*pipex).paths[i]);
		(*pipex).paths[i] = ft_strjoin(tmp, "/");
		free(tmp);
		i++;
	}
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

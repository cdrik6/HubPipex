/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/09 01:49:19 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// waitpid(-1, NULL, WNOHANG); // waitpid(pid, NULL, 0);
void	fork_child(t_pipex pipex, char **argv, char **envp, int k)
{
	pid_t	pid[2];

	pid[k - 1] = fork();
	if (pid[k - 1] == -1)
		perror_close_exit("fork", pipex, 1);
	if (pid[k - 1] == 0)
	{
		close(pipex.fd[0]);
		dup2(pipex.fd[1], STDOUT);
		close(pipex.fd[1]);
		exec_arg(pipex, argv, envp, k);
	}
	else
	{
		close(pipex.fd[1]);
		dup2(pipex.fd[0], STDIN);
		close(pipex.fd[0]);
		if (k == 1)
		{
			fork_child(pipex, argv, envp, 2);
			// waitpid(-1, &(pipex.status), 0);
		}
		// // waitpid(pid, &(pipex.status[k - 1]), WNOHANG);
			// no leak but KO sleep
		// // waitpid(-1, &(pipex.status[k - 1]), WNOHANG);
			// no leak but KO sleep
		// waitpid(-1, &(pipex.status[k - 1]), 0);
			//leaks 'head' mais c'est le 'yes' en fait cf valgrind
		// // waitpid(pid, &(pipex.status[k - 1]), 0); //zombie + leaks head
		//
		// if (k == 1)
		// {
		waitpid(-1, &(pipex.status[0]), 0);
		// }
		// if (k == 2)
		// {
		waitpid(-1, &(pipex.status[1]), 0);
		//}
		// waitpid(pid[k - 1], &(pipex.status[k - 1]), 0);
		// ft_putstr_fd(2, "\ncmd1 - ");
		// ft_putnbr_fd(WEXITSTATUS(pipex.status[0]), 2);
		// ft_putstr_fd(2, "\ncmd2 - ");
		// ft_putnbr_fd(WEXITSTATUS(pipex.status[1]), 2);
		if (WEXITSTATUS(pipex.status[0]) != 0)
		{
			// ft_putnbr_fd(k, 2);
			// ft_putstr_fd(2, "exit status non nul\n");
			// ft_putnbr_fd(WEXITSTATUS(pipex.status[k - 1]), 2);
			exit(WEXITSTATUS(pipex.status[0]));
		}
		// if (WIFEXITED(pipex.status))
		// {
		// 	waitpid(pid, &(pipex.status), 0);
		// }
		// else
		// 	waitpid(pid, &(pipex.status), WNOHANG);
	}
}

void	exec_arg(t_pipex pipex, char **argv, char **envp, int k)
{
	if (check_slash(argv[k + 1]) == 1)
		exec_abs(pipex, argv, envp, k);
	else
		exec_cmd(pipex, argv, envp, k);
}

// int execve(const char *pathname, char *const argv[], char *const envp[]);
void	exec_cmd(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;
	char	*path_cmd;

	get_paths(envp, &pipex);
	if (ft_strlen(argv[k + 1]) == 0)
	{
		// ft_putstr_fd(2, "ici\n");
		ft_putstr_fd(2, ERR_ACC);
		free_paths(&pipex);
		close_exit(&pipex, EXIT_DENIED);
	}
	else
		cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
	{
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(&pipex, EXIT_FAILURE);
	}
	path_cmd = check_path(pipex.paths, cmd);
	if (!path_cmd)
	{
		ft_putstr_fd(2, ERR_CMD);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(&pipex, EXIT_NOCMD);
	}
	if (ft_strlen(path_cmd) == 0)
	{
		ft_putstr_fd(2, ERR_ACC);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(&pipex, EXIT_DENIED);
	}
	if (execve(path_cmd, cmd, envp) == -1)
	{
		perror("execve");
		free(path_cmd);
		free_cmd(cmd);
		free_paths(&pipex);
		close_exit(&pipex, EXIT_FAILURE);
	}
}

// write(2, cmd[0], ft_strlen(cmd[0]));
void	exec_abs(t_pipex pipex, char **argv, char **envp, int k)
{
	char	**cmd;

	cmd = ft_split(argv[k + 1], ' ');
	if (!cmd)
	{
		free_cmd(cmd);
		close_exit(&pipex, EXIT_FAILURE);
	}
	if (access(cmd[0], X_OK) != 0)
	{
		if (access(cmd[0], F_OK) != 0)
		{
			perror("access");
			ft_putstr_fd(2, ERR_CMD);
			free_cmd(cmd);
			close_exit(&pipex, EXIT_NOCMD);
		}
		perror("access");
		ft_putstr_fd(2, ERR_ACC);
		free_cmd(cmd);
		close_exit(&pipex, EXIT_DENIED);
	}
	if (execve(cmd[0], cmd, envp) == -1)
	{
		perror("execve");
		free_cmd(cmd);
		close_exit(&pipex, EXIT_FAILURE);
	}
}

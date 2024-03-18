/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:13:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 01:24:24 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	get_paths(char **envp, t_pipex *pipex)
{
	int		i;
	char	*path;

	i = 0;
	if (envp)
	{
		while (envp[i] && (check_in_str(envp[i], "PATH=") == 0))
			i++;
		path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
		if (!path)
		{
			ft_putstr_fd(ERR_CMD, 2);
			close_exit(*pipex, EXIT_NOCMD);
		}
		(*pipex).paths = ft_split(path, ':');
		free(path);
		if (!(*pipex).paths)
		{
			free_paths(pipex);
			close_exit(*pipex, EXIT_FAILURE);
		}
		slash_paths(pipex);
	}
	else
		close_exit(*pipex, EXIT_FAILURE);
}

// Only the last paths[i] is NULL
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
		if (!(*pipex).paths[i])
		{
			free_paths(pipex);
			close_exit(*pipex, 1);
		}
		i++;
	}
}

// Only the last paths[i] is NULL
char	*check_path(char **paths, char **cmd)
{
	int		i;
	char	*tmp;

	if (!paths || !cmd)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if (cmd[0] == NULL)
			return (NULL);
		tmp = ft_strjoin(paths[i], cmd[0]);
		if (!tmp)
			return (NULL);
		if (access(tmp, X_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

int	check_slash(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	putstr_error(char *cmd0, char *err_str)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin("pipex: ", cmd0);
	tmp2 = ft_strjoin(tmp1, err_str);
	ft_putstr_fd(tmp2, STDERR);
	free(tmp1);
	free(tmp2);
}

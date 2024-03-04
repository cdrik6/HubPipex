/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:13:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/04 23:54:26 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
		tmp = ft_strjoin(paths[i], cmd[0]);
		if (!tmp)
			return (NULL);
		if (access(tmp, F_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

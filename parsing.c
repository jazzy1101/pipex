/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:28:29 by dabae             #+#    #+#             */
/*   Updated: 2024/03/14 15:57:14 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* whole_paths : to retrieve all the paths of commands in the environment */

static char	**whole_paths(char **envp)
{
	int		j;
	char	*whole_path;
	char	**path_arr;

	j = 0;
	while (envp[j])
	{
		if (ft_strncmp("PATH=", envp[j], 5) == 0)
		{
			whole_path = ft_strdup(envp[j] + 5);
			path_arr = ft_split(whole_path, ':');
		}
		j++;
	}
	free(whole_path);
	if (!path_arr)
	{
		ft_free_tab(path_arr);
		return (NULL);
	}
	return (path_arr);
}

/* get_cmd_path : to verify if the command is executable and it exists, if so, 
 return the right path for the command */

char	*get_cmd_path(char *cmd_name, char **envp)
{
	int		j;
	char	*cmd_path;
	char	*tmp;
	char	**path_arr;

	path_arr = whole_paths(envp);
	j = 0;
	while (path_arr[j])
	{
		tmp = ft_strjoin(path_arr[j], "/");
		cmd_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
		{
			ft_free_tab(path_arr);
			return (cmd_path);
		}
		free(cmd_path);
		j++;
	}
	ft_free_tab(path_arr);
	return (NULL);
}

void	err_msg_exit(char *msg, char ***arr)
{
	if (arr)
		free_triple_arr(arr);
	errno = 1;
	perror(msg);
	exit(1);
}

void	free_triple_arr(char ***arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		ft_free_tab(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

/* trim_cmds : first, manage the commands line with space, single quote and 
double quote. And then, assign each trimmed command in the array. */

char	***trim_cmds(int ac, char **av)
{
	int		i;
	char	***cmds;

	cmds = malloc(sizeof(char **) * (ac - 2));
	if (!cmds)
	{
		free(cmds);
		return (NULL);
	}
	i = 2;
	while (i < ac - 1)
	{
		cmds[i - 2] = ft_split_special(av[i], " \"'");
		if (!cmds[i - 2])
		{
			free_triple_arr(cmds);
			return (NULL);
		}
		i++;
	}
	cmds[i - 2] = NULL;
	return (cmds);
}

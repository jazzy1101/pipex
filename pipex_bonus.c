/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 10:21:08 by dabae             #+#    #+#             */
/*   Updated: 2024/03/07 14:05:32 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	here_doc_creater(char **av)
{
	char	*line;
	int		fd;

	fd = open("here_doc", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		error_handler();
	while (1)
	{
		write(1, "pipe heredoc>", 13);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strcmp(line, av[2]) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

static int	last_process(char *outfile, bool here)
{
	int	out_fd;

	if (here)
		out_fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0 || access(outfile, W_OK) == -1)
		return (EXIT_FAILURE);
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	if (here)
		unlink("here_doc");
	return (EXIT_SUCCESS);
}

static void	pipe_fork(int i, int num_cmd, char ***cmds, char **envp)
{
	int		end[2];
	pid_t	pid;

	if (pipe(end) == -1)
		error_handler();
	pid = fork();
	if (pid < 0)
		error_handler();
	if (pid == 0)
	{
		close(end[0]);
		dup2(end[1], STDOUT_FILENO);
		close(end[1]);
		execve(get_cmd_path(cmds[i][0], envp), cmds[i], envp);
	}
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	if (i == num_cmd - 2)
		close(end[0]);
}

static int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (EXIT_FAILURE);
	if (ft_strcmp(filename, "here_doc") != 0)
	{
		if (access(filename, R_OK) == -1)
			error_handler();
	}
	return (fd);
}

int	pipex_bonus(int ac, char **av, char ***cmds, char **envp)
{
	int		i;
	int		in_fd;
	int 	num_cmd;

	num_cmd = ac - 3;
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		here_doc_creater(av);
		num_cmd = ac - 4;
	}
	in_fd = open_file(av[1]);
	if (in_fd == EXIT_FAILURE || dup2(in_fd, STDIN_FILENO) < 0)
	{
		close(in_fd);
		return (EXIT_FAILURE);
	}
	close(in_fd);
	i = -1;
	while (++i < num_cmd - 1)
		pipe_fork(i, num_cmd, cmds, envp);
	last_process(av[ac - 1], ft_strcmp(av[1], "here_doc") == 0);
	execve(get_cmd_path(cmds[i][0], envp), cmds[i], envp);
	return (EXIT_SUCCESS);
}

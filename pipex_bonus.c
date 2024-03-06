/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 10:21:08 by dabae             #+#    #+#             */
/*   Updated: 2024/03/06 15:45:43 by dabae            ###   ########.fr       */
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
	if (dup2(out_fd, STDOUT_FILENO) < 0)
		error_handler();
	close(out_fd);
	return (EXIT_SUCCESS);
}

// static int	pipe_fork(int ac, int i, char **cmds, char **envp)
// {
// 	int		end[2];
// 	pid_t	pid;

// 	pid = fork();
// 	if (pipe(end) == -1 || pid < 0)
// 		error_handler();
// 	if (i == ac - 4)
// 	{
// 		if (dup2(end[0], STDIN_FILENO) < 0)
// 			error_handler();
// 		close(end[0]);
// 		close(end[1]);
// 		return (EXIT_SUCCESS);
// 	}
// 	if (pid == 0)
// 	{
// 		if (dup2(end[1], STDOUT_FILENO) < 0)
// 			error_handler();
// 		close(end[1]);
// 		if (!get_cmd_path(cmds[0], envp) || execve(get_cmd_path(cmds[0], envp),
// 				cmds, envp) == -1)
// 			error_handler();
// 	}
// 	if (dup2(end[0], STDIN_FILENO) < 0)
// 		error_handler();
// 	close(end[0]);
// 	return (EXIT_SUCCESS);
// }

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

// int	pipex_bonus(int ac, char **av, char ***cmds, char **envp)
// {
// 	int		i;
// 	int		in_fd;

// 	i = -1;
// 	if (ft_strcmp(av[1], "here_doc") == 0)
// 	{
// 		i = 0;
// 		here_doc_handler(av);
// 	}
// 	in_fd = open_file(av[1]);
// 	if (in_fd == EXIT_FAILURE || dup2(in_fd, STDIN_FILENO) < 0)
// 	{
// 		close(in_fd);
// 		return (EXIT_FAILURE);
// 	}
// 	close(in_fd);
// 	while (++i < ac - 3)
// 	{
// 		if (pipe_fork(ac, i, cmds[i], envp) == EXIT_FAILURE)
// 			return (EXIT_FAILURE);
// 	}
// 	last_process(av[ac - 1], ft_strcmp(av[1], "here_doc"));
// 	if (!get_cmd_path(cmds[i][0], envp) || execve(get_cmd_path(cmds[i][0],
// 			envp), cmds[i], envp) == -1)
// 		error_handler();
// 	return (EXIT_SUCCESS);
// }

static int	fork_loop(int i, int *end, char **cmds, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_handler();
	if (pid == 0)
	{
		if (i == 0)
		{
			close(end[0]);
			dup2(end[1], STDOUT_FILENO);
			close(end[1]);
		}
		else
		{
			close(end[1]);
			dup2(end[0], STDIN_FILENO);
			close(end[0]);
		}
		if (!get_cmd_path(cmds[0], envp) || execve(get_cmd_path(cmds[0], envp),
				cmds, envp) == -1)
			error_handler();
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// int	pipex_bonus(int ac, char **av, char ***cmds, char **envp)
// {
// 	int	end[2];
// 	int	in_fd;
// 	int	i;

// 	i = -1;
// 	if (pipe(end) == -1)
// 		error_handler();
// 	if (ft_strcmp(av[1], "here_doc") == 0)
// 	{
// 		i = 0;
// 		here_doc_creater(av);
// 	}
// 	in_fd = open_file(av[1]);
// 	if (dup2(in_fd, STDIN_FILENO) < 0)
// 	{
// 		close(in_fd);
// 		return (EXIT_FAILURE);
// 	}
// 	close(in_fd);
// 	close(end[0]);
// 	while (++i < ac - 4)
// 		fork_loop(end, cmds[i], envp);
// 	last_process(av[ac - 1], ft_strcmp(av[1], "here_doc") == 0);
// 	if (!get_cmd_path(cmds[i][0], envp) || execve(get_cmd_path(cmds[i][0],
// 			envp), cmds[i], envp) == -1)
// 		error_handler();
// 	close(end[0]);
// 	return (EXIT_SUCCESS);
// }
int	pipex_bonus(int ac, char **av, char ***cmds, char **envp)
{
	int		end[2];
	int		in_fd;
	int		i;

	if (pipe(end) == -1)
		error_handler();
	if (ft_strcmp(av[1], "here_doc") == 0)
		here_doc_creater(av);
	in_fd = open_file(av[1]);
	if (dup2(in_fd, STDIN_FILENO) < 0)
	{
		close(in_fd);
		return (EXIT_FAILURE);
	}
	close(in_fd);
	i = -1;
	while (++i < ac - 4)
		fork_loop(i, end, cmds[i], envp);
	last_process(av[ac - 1], ft_strcmp(av[1], "here_doc") == 0);
	if (!get_cmd_path(cmds[i][0], envp) || execve(get_cmd_path(cmds[i][0], envp), cmds[i], envp) == -1)
		error_handler();
	close(end[0]); // Close read end in parent
	close(end[1]); // Close write end in parent
	return (EXIT_SUCCESS);
}

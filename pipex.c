/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:51:57 by dabae             #+#    #+#             */
/*   Updated: 2024/02/29 09:53:49 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf/ft_printf.h"
#include "ft_printf/libft/libft.h"

static void	init_av(int ac, char **av, t_pipe *args)
{
	int		i;
	char	***cmds;

	/*getting file path*/
	args->infile = ft_strdup(av[1]);
	args->outfile = ft_strdup(av[ac -1]);
	/*getting command lines*/
	cmds = malloc(sizeof(char **) * (ac - 2));
	if (!cmds)
	{
		free(cmds);
		return ;
	}
	i = 2;
	while (i < ac - 1)
	{
		cmds[i - 2] = ft_split(av[i], ' ');
		i++;
	}
	cmds[i]= NULL;
	args->cmd_args = cmds;
	/*bonus part*/
	if (ft_strcmp("here_doc", av[1]) == 0)
		args->here_doc = true;
	args->here_doc = false;
}

static char	*get_cmd_path(t_pipe *args)
{
	int	i;
	int	j;
	char	*cmd_path;

	j = 0;
	while (args->paths[j])
	{
		i = 0;
		while (args->cmd_args[i][1])
		{
			cmd_path = ft_strjoin(args->paths[j], "/");
			cmd_path = ft_strjoin(cmd_path, args->cmd_args[i][1]);
			if (access(cmd_path, F_OK) == 0)
				return cmd_path;
			i++;
		}
		j++;
	}
	perror("Command not exist or unexecutable");
	return (NULL);
}

static void	pipex(t_pipe *args)
{
	int	end[2];
	int	pid1;

	if (pipe(end) == -1)
		perror("pipe error");
	//end[0] = read end, end[1] = write end
	
	pid1 = fork();
	if (pid1 < 0)
		perror("Fork error");
	else if (pid1 == 0)	
	{
		//child process
		args->in_fd = open(args->infile, O_RDONLY);
		if (args->in_fd < 0)
			return (EXIT_FAILURE);
		dup2(args->in_fd, STDIN_FILENO);
		close(end[0]);
		execve(get_cmd_path(args));
	}
	else
	{
		//parent process
		args->out_fd = open(args->outfile, O_WRONLY | O_CREAT, 0777);
		if (args->out_fd < 0)
			return (EXIT_FAILURE);
		dup2(args->out_fd, STDOUT_FILENO);
		close(end[1]);
		execve()
	}

}

int	main(int ac, char **av)
{
	t_pipe	*args;

	if (ac < 5)
		return (EXIT_FAILURE);
	ft_memset(args, 0, sizeof(t_pipe));
	init_av(ac, av, args);
	args->paths = ft_split(getenv("PATH"), ':');
	pipex(args);

	return (0);
}
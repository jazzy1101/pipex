/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:51:52 by dabae             #+#    #+#             */
/*   Updated: 2024/03/04 16:43:34 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include "get_next_line/get_next_line.h"

char	***trim_cmds(int ac, char **av);
char	*get_cmd_path(char *cmd_name, char **envp);
void	free_triple_arr(char ***arr);
int		error_handler(void);
int		pipex_bonus(int ac, char **av, char ***cmds, char **envp);

#endif
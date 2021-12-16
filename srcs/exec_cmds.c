/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 1mthe0wl </var/spool/mail/evil>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 21:41:20 by 1mthe0wl          #+#    #+#             */
/*   Updated: 2021/12/16 09:47:00 by 1mthe0wl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void	child_process(int *fd, char **cmd, char **env)
{
	close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) < 0)
		printf("Error:");
	close(fd[1]);
	if (execve(cmd[0], cmd, env) < 0)
		printf("minishell: %s command execution failed", cmd[0]);
}

void	parent_process(int *fd, char **cmd, char **env)
{
	int	pid;

	close(fd[1]);
	pid = fork();
	if (!pid)
	{
		if (dup2(fd[0], STDIN_FILENO) < 0)
			printf("Error: duplicating filedescriptor 0");
		if (execve(cmd[0], cmd, env) < 0)
			printf("Minishell: %s executing command failed", cmd[0]);
	}
	else if (pid > 0)
		close(fd[0]);
	else
	{
		printf("Minishell: error in parrent process");
	}
}

void	exec_pipe_cmd(t_shell *shell, char **env)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) < 0)
	{
		printf("pipe err");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (!pid)
		child_process(fd, shell->cmds_pipe[0], env);
	else if (pid > 0)
		parent_process(fd, shell->cmds_pipe[1], env);
	else
		printf("Error in forking pipes");
	wait(&status);
	wait(&status);
}

void	exec_path_cmd(char *path, t_shell *shell)
{
	int		status;
	pid_t	pid;
	pid_t	child_pid;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		execv(path, shell->tokens);
	}
	else if (pid > 0)
		child_pid = waitpid(pid, &status, 0);
}

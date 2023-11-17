#include <unistd.h>
#include "shell.h"

/**
 * main - entry point
 * @argc: arg count
 * @argv: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int is_whitespace(char c);

int main(int argc, char *argv[])
{
	char *program_name = argv[0];
	bool interactive_mode;
	size_t len;
	int is_empty_or_whitespace = 1;
	size_t i;
	char *line = NULL;
	size_t bufsize = 0;
	(void) argc;

	interactive_mode = isatty(fileno(stdin));
	while (true)
	{
		if (interactive_mode)
		{
			printf("$ ");
			fflush(stdout);
		}
		if (getline(&line, &bufsize, stdin) == -1)
		{
			break;
		}
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
		{
			line[len - 1] = '\0';
		}
		for (i = 0; i < len; i++)
		{
			if (!is_whitespace(line[i]))
			{
				is_empty_or_whitespace = 0;
				break;
			}
		}
		if (is_empty_or_whitespace)
		{
			free(line);
			continue;
		}
		if (strspn(line, " \t\n\r") == len)
		{
			free(line);
			continue;
		}
		if (strcmp(line, "exit") == 0)
		{
			break;
		}

		execute_command(line, program_name);

	}
	free(line);
	return (0);
}
int is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/**
 * execute_command - Execute a command given by the user.
 * @command: The command to execute.
 * @program_name: The name of the program (argv[0]).
 */
void execute_command(char *command, char *program_name)
{
	pid_t pid;

	if (strspn(command, " \t\n\r") == strlen(command))
                 {
	
                         return;
                 }
	pid = fork();

	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		char **args = NULL;
		char *token;
		int i;
		int x;
		(void) program_name;

		i = 0;
		token = strtok(command, " ");
		while (token != NULL)
		{
			if (strlen(token) > 0)
			{
			args = realloc(args, (i + 1) * sizeof(char *));
			if (args == NULL)
			{
				exit(EXIT_FAILURE);
			}
			args[i] = strdup(token);
			if (args[i] == NULL)
			{
				exit(EXIT_FAILURE);
			}
			i++;
			}
			token = strtok(NULL, " ");
		}
		args = realloc(args, (i + 1) * sizeof(char *));
		if (args == NULL)
		{
			exit(EXIT_FAILURE);
		}
		args[i] = NULL;

		execvp(args[0], args);
		for (x = 0; x < i; x++)
		{
			free(args[x]);
		}
		free(args);
		_exit(EXIT_FAILURE);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
}

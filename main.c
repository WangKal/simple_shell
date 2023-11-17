#include <unistd.h>
#include "shell.h"

/**
 * main - entry point
 * @argc: arg count
 * @argv: arg vector
 *
 * Return: 0 on success, 1 on error
 */
#define BUFFER_SIZE 1024

int is_whitespace(char c);
void print_environment(void);
char *my_getline(void);
int main(int argc, char *argv[])
{
	char *program_name = argv[0];
	bool interactive_mode;
	size_t len;
	int is_empty_or_whitespace = 1;
	size_t i;
	char *line = NULL;
	(void) argc;

	interactive_mode = isatty(fileno(stdin));
	while (true)
	{
		if (interactive_mode)
		{
			printf("$ ");
			fflush(stdout);
		}
		line = my_getline();
		if (line == NULL)
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
		if (strcmp(line, "env") == 0)
		{
			print_environment();
			continue;
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
	int exit_status;
	char *status_str;
	pid_t pid;

	if (strspn(command, " \t\n\r") == strlen(command))
                 {
	
                         return;
                 }
                if (strncmp(command, "exit", 4) == 0)
                {
      			exit_status = 0;
			status_str = strtok(command + 4, " \t\n\r");
			if (status_str != NULL)
			{
				exit_status = atoi(status_str);
			}
			free(command);
			exit(exit_status);
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
		perror("execvp");
		_exit(2);

		for (x = 0; x < i; x++)
		{
			free(args[x]);
		}
		free(args);
		_exit(2);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 127)
			{
				status = 2;
			}
			else
			{
				status = WEXITSTATUS(status);
			}
		}
		else
		{
			status = 1;
		}
	}
}
void print_environment(void)
{
	extern char **environ;
	char **env;
	for (env = environ; *env != NULL; env++)
	{
		printf("%s\n", *env);
	}
}
char *my_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static size_t buffer_index = 0;
	static ssize_t buffer_size = 0;
	char *line = NULL;
	size_t line_size = 0;
	
	while (1)
	{
		if (buffer_index >= (size_t)buffer_size || buffer_size < 0)
		{
			buffer_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			if (buffer_size <= 0)
			{
			       	if (line_size > 0)
				{
					return line;
				}
				return NULL;
			}
			buffer_index = 0;
		}
		if (buffer[buffer_index] == '\n')
		{
			buffer[buffer_index] = '\0';
			buffer_index++;
			break;
		}
		line = realloc(line, line_size + 1 + 1);
		line[line_size] = buffer[buffer_index];
		line_size++;
		buffer_index++;
	}
	line = realloc(line, line_size + 1);
	line[line_size] = '\0';
	return line;
}

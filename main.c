#include <unistd.h>
#include "shell.h"

/**
 * main - entry point
 * @argc: arg count
 * @argv: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char *argv[])
{

	char *program_name = argv[0];
	bool interactive_mode;
	size_t len;
	char *line = NULL;
	size_t bufsize = 0;
	(void) argc;

	interactive_mode = isatty(fileno(stdin));
	while (true)
	{
		if (interactive_mode)
		{
			printf("$ : ");
			fflush(stdout);
		}
		if (getline(&line, &bufsize, stdin) == -1)
		{
			printf("\n");
			break;
		}
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
		{
			line[len - 1] = '\0';
		}
		execute_command(line, program_name);
	}

	return (0);
}

/**
 * execute_command - Execute a command given by the user.
 * @command: The command to execute.
 * @program_name: The name of the program (argv[0]).
 */
void execute_command(char *command, char *program_name)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		char *args[3];
		args[0] = command;
		args[1] = NULL;

		execvp(command, args);
		fprintf(stderr, "%s: No such file or directory %s\n", program_name, command);
		_exit(EXIT_FAILURE);
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}

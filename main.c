#include <unistd.h>
#include "shell.h"

/**
 * main - entry point
 * @arg: arg count
 * @argv: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char *argv[])
{
	char command[MAX_COMMAND_LENGTH];
	char *program_name = argv[0];
	bool interactive_mode;
	size_t len;

	interactive_mode = isatty(fileno(stdin));
	while (true)
	{
		if (interactive_mode)
		{
			printf("$ : ");
			fflush(stdout);
		}
		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			printf("\n");
			break;
		}
		len = strlen(command);
		if (len > 0 && command[len - 1] == '\n')
		{
			command[len - 1] = '\0';
		}
		execute_command(command, program_name);
	}

	return (0);
}


void execute_command(char *command, char *program_name)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		char *args[] = {command, NULL};

		execvp(command, args);
		fprintf(stderr, "%s: command not found: %s\n", program_name, command);
		_exit(EXIT_FAILURE);
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}

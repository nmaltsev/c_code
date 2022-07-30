#include "main.h"
#include "utils.h"

int execute_command(char* cmd_line) {
	int n_tokens = 0;
	char** tokens = parse_command(cmd_line, &n_tokens);
	
	if (tokens == NULL) {
		return -1;
	}
	if (n_tokens > 0 && strcmp(tokens[0], "cd") == 0) {
		if (chdir(tokens[1]) != 0) {
			raise_error("Exception with cd");
		}
	} 
	else if (n_tokens > 1 && strcmp(tokens[n_tokens - 1], "&") == 0) {
		free(tokens[n_tokens - 1]);
		tokens[n_tokens - 1] = NULL;
		run_background_job(tokens);
	}
	else {
		run_foreground_job(tokens);
	}
	cleanup(&tokens, n_tokens);
	return 0;
}

void command_shell() {
	char *str = NULL;
	pid_t pid = getpid();
	char path[PATH_MAX];
	getcwd(path, sizeof(path));
	printf("PID %d\n", pid);

	// The sigaction structure specifies how to handle a signal (specifies how you want to modify the action for the given signal).
	struct sigaction current = {
		.sa_flags = 0,
		// SIG_IGN - Ignore the signal. Setting SIG_IGN for a signal that's pending discards all pending signals, whether it's blocked or not. New signals are discarded. If you set the action for SIGCHLD to SIG_IGN, your process's children won't enter the zombie state, and the process won't be able to use wait() or waitpid() to wait on their deaths. 
		.sa_handler = SIG_IGN
	};
	struct sigaction hup = {
		.sa_flags = 0,
		.sa_handler = &handler_HUP
	};
	setup();

	// here sigaction() is used to examine the action that's associated with a specific signal
	// <) https://man7.org/linux/man-pages/man2/sigaction.2.html
	if (sigaction(SIGTERM, &current, NULL) == -1) {
		raise_error("SIGTERM set");
	}
	if (sigaction(SIGQUIT, &current, NULL) == -1) {
		raise_error("SIGQUIT set");
	}
	if (sigaction(SIGINT, &current, NULL) == -1) {
		raise_error("SIGINT set");
	}
	if (sigaction(SIGHUP, &hup, NULL) == -1) {
		raise_error("SIGHUP set");
	}

	while(1) {
		printf("%s> ", path);
		
		// TODO use fgets to unlimited input
		
		// scanf does not include '\n' in the stored string.
		// The space before the %m matches all preceding whitespaces.
		// The %*c matches a single character, but the asterisk indicates that the character will not be stored anywhere.
		// This has the effect of consuming the newline character generated 
		// by the enter key so that the next time you call scanf() you are starting with an empty input buffer.
		scanf(" %m[^\n]%*c", &str);
		fflush(stdout);

		//printf("'%s' %d \n", str, str == NULL ? 1 : 0);
		
		//// size_t str_len = 0;
		//// ssize_t res = getline(&str, &str_len, stdin);
		//// printf("rez = %zd, n = %zu, '%s'\n", res, str_len, str);


		if (str == NULL) {
			continue;
		}
		
		// Type `exit` to exit from loop
		if (strcmp(str, "exit") == 0) {
			// SIG_DFL - Use the default action for the signal
			current.sa_handler = SIG_DFL;
			if (sigaction(SIGTERM, &current, NULL) == -1) {
				raise_error("SIGTERM set");
			}
			if (sigaction(SIGQUIT, &current, NULL) == -1){
				raise_error("SIGQUIT set");
			}
			// kill(0, SIGTERM);
			break;
		}
		
		if (strcmp(str, "help") == 0) {
			printf("%s", HELP_MESSAGE);
		} else {
			if (execute_command(str) == -1) {
				printf("Can't parse\n");
				continue;
			}
		}
		getcwd(path, sizeof(path));
		str = NULL;
	}
}



int main(int argc, char *argv[]) {
	if (argc == 3 && (strcmp(argv[1], "-c") == 0)) {
		execute_command(argv[2]);
	} 
	else if (argc == 2 && (strcmp(argv[1], "-h") == 0)) {
		printf("%s", HELP_MESSAGE);
	}
	else if (argc == 2 && (strcmp(argv[1], "-t") == 0)) {
		// TODO tests
		char* tests[10] = {
			"echo \"abc 'x'+\\\"y\\\"=z\"\t123 'qw\"e\"rty'\t\t___ ", 
			"echo \"'test'\"", 
			"echo \"\" \"1 2 3\" abc", 
			"echo \"abc",
			"",
			"cd .",
			"a b c &",
			"cd ..",
			"cd \"aaa\"\"bb\"",
			NULL
		};
		
		int n_tokens = 0;
		size_t i;
		char** tokens;
		for (i = 0; *(tests + i); i++) {
			printf("Test #%zu (%zu) `%s`\n", i, strlen(*(tests + i)), *(tests + i));
			tokens = parse_command(*(tests + i), &n_tokens);
			printf("N %d\n", n_tokens);
			print_lines(tokens);

			printf("Last token `%s`\n", n_tokens > 0 ? tokens[n_tokens -1] : "n.e.");
			cleanup(&tokens, n_tokens);
		}
		
	} 
	else {
		command_shell();
	}
	
	return EXIT_SUCCESS;
}

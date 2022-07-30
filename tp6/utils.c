#include "utils.h"


void raise_error(const char* message){
	fprintf(stderr, "%s\n", message);
	perror(message);
	exit(EXIT_FAILURE);
}

void cleanup(char*** lines, size_t len) {
	size_t i;
	for (i = 0; i < len; i++) {
		free(*(*lines + i));
	}
	free(*lines);
}

/*
Signal Name | Signal Value 	| Effect
SIGHUP 		| 1				| Hangup
SIGINT		| 2 			| Interrupt from keyboard
SIGKILL 	| 9 			| Kill signal
SIGTERM 	| 15            | Termination signal
SIGSTOP 	| 17, 19, 23    | Stop the process

to stop: `sudo kill -<signal value> <pid number>`
 */
char* signal2str(int signal){
	char* out;
	switch (signal) {
		case SIGINT: out = "SIGINT"; break;
		case SIGTERM: out = "SIGTERM"; break;
		case SIGCHLD: out = "SIGCHLD"; break;
		case SIGHUP: out = "SIGHUP"; break;
		case SIGKILL: out = "SIGKILL"; break;
		default: out = malloc(10 * sizeof(char)); sprintf(out, "%d", signal); break; 
	}
	return out;
}

// Parse complex expressions like: `echo -e "1\n | \n2" | grep "|"`
// TODO replace quotas!
char** parse_command(char* command_line, int* n_tokens) {
	int line_len = strlen(command_line);
	int i;
	int start_pos = 0;
	// quota counters to respect spaces inside strings
	int n_doubl_quotas = 0;
	int n_singl_quotas = 0;
	int substr_len;
	int count = 0;
	char** collection = (char**)malloc(sizeof(char*));
	int n_skipped_chars = 0;

	for(i = 0; i < line_len; i++) {
		// All tokens must be delimeted by ' ' or '\t' chars
		if (
			(command_line[i] == ' ' || command_line[i] == '\t') &&
			(n_doubl_quotas == 0 && n_singl_quotas == 0)
		) {
			// TODO fix range to not include quotas and replace escaptions!

			// substr_len = i - (i > 0 && (command_line[i-1] == '"' || command_line[i-1] == '\'') ? 1 : 0) - start_pos; 
			substr_len = i - n_skipped_chars - start_pos; 
			
			if (substr_len > 0) {
				// Copy a substring from start_pos to i
				// char* substr = malloc(substr_len);	
				// strncpy(substr, command_line + start_pos, substr_len);
				collection = (char**)realloc(collection, (count + 1) * sizeof(char *));
				collection[count] = malloc((substr_len + 1) * sizeof(char));	
				strncpy(collection[count], command_line + start_pos, substr_len);
				collection[count][substr_len] = '\0';
				count++;
			} 

			start_pos = i + 1;
			n_skipped_chars = 0;			
		}	
		else if (command_line[i] == '\'' && n_doubl_quotas == 0) {
			if (i-1 >= 0 && command_line[i-1] == '\\') {
				continue;
			}
			n_singl_quotas = n_singl_quotas == 0 ? 1 : 0;
			if (n_singl_quotas == 1) start_pos++;
			else n_skipped_chars = 1;
		}
		else if (command_line[i] == '"' && n_singl_quotas == 0) {
			// escape quoatas by leading '\\'
			if (i-1 >= 0 && command_line[i-1] == '\\') {
				continue;
			}
			n_doubl_quotas = n_doubl_quotas == 0 ? 1 : 0;
			if (n_doubl_quotas == 1) start_pos++;
			else n_skipped_chars = 1;
		}
	}

	substr_len = line_len - n_skipped_chars - start_pos;
	// substr_len = line_len - (i > 0 && (command_line[i-1] == '"' || command_line[i-1] == '\'') ? 1 : 0) - start_pos;
	if (substr_len > 0) {
		// Copy a final cut from start_pos to line_len
		// char* substr = malloc(substr_len);	
		// strncpy(substr, command_line + start_pos, substr_len);
		collection = (char**)realloc(collection, (count + 1) * sizeof(char *));
		collection[count] = malloc((substr_len + 1) * sizeof(char));	
		strncpy(collection[count], command_line + start_pos, substr_len);
		collection[count][substr_len] = '\0';
		count++;
	}
	*n_tokens = count;
	collection = (char**)realloc(collection, (count + 1) * sizeof(char *));
	collection[count] = NULL;
	return collection;
}


void print_lines(char** lines) {
	size_t i;
	// %zu - prints as unsigned decimal. Used to print a size_t variable portably.
	for (i = 0; *(lines + i); i++) {
		printf("#%2zu %3zu %s\n", i, strlen(*(lines + i)), *(lines + i));
	}
}





void handler_HUP(int signal) {
	printf("\t[handler HUP] signal: %s\n", signal2str(signal));
	if (signal == SIGHUP) {
		kill(0, SIGHUP);
	}
}


// [Ctrl-C] -> SIGINT
// [Ctrl-Z] -> SIGSTP
// [CTRL-\] -> ​SIGQUIT
void ctrl_c_handler(int signal) {
	printf("[Ctrl C handler] %d %s\n", signal, signal2str(signal));
	// TODO check
	if (signal == SIGINT) {
		// kill(0, SIGTERM);
		kill(0, SIGCHLD);	
		kill(0, SIGINT);
	}
}
void setup_int() {
	struct sigaction ctrl_c = {
		.sa_flags = 0,
		.sa_handler = &ctrl_c_handler
	};
	sigaction(SIGINT, &ctrl_c, NULL);
}

// the last element in argv must be NULL!
// A ​foreground​ job is the group of processes with which the user interacts. 

void run_foreground_job(char** arguments) {
	pid_t fork_pid = fork();
	
	// TODO can't itterupt a foreground job by Ctrl-C
	// Do not use handler for the background task
	//// setup_int();
	if (fork_pid == 0) { // in parent process
		// #include <unistd.h> int execve(const char *filename, char *const argv[], char *const envp[]); 
		execvp(arguments[0], arguments);
	} else { // in child process
		printf("[run_foreground_job] %u\n", fork_pid);
		
		pid_t wait_pid;
		int wait_status;
		do { 
			wait_pid = waitpid(fork_pid, &wait_status, WUNTRACED | WCONTINUED);
			// we take information on the child as long as it is accessible (traceable) and the parent has not received a signal to continue 
			
			if (wait_pid == -1) {
				printf("[Execute_cmd exception %u]\n", wait_pid);
				perror("wait_pid");
			}
			
			if (WIFEXITED(wait_status)) {
				printf("Foreground job %u exited with code=%d\n", wait_pid, WEXITSTATUS(wait_status));
			} 
			else if (WIFSIGNALED(wait_status)) {
				printf("Foreground job %u exited", wait_pid);
			} 
			else if (WIFSTOPPED(wait_status)) {
				printf("Foreground job %u exited, stopped by signal %d\n", wait_pid, WSTOPSIG(wait_status));
			} 
			else if(WIFCONTINUED(wait_status)) {
				printf("Foreground job continued %u\n", wait_pid);
			}
		} while (
			!WIFEXITED(wait_status) && 
			!WIFSIGNALED(wait_status)
		);
	}
}



void chld_handler(int signal){
	int wait_status;
	pid_t wait_pid;

	printf("\t[Handler] signal %s\n", signal2str(signal));
	
	if (signal == SIGCHLD) {
		do {
			// wait_pid = waitpid(0, &wait_status, WNOHANG | WCONTINUED | WUNTRACED);
			wait_pid = waitpid(WAIT_ANY, &wait_status, WNOHANG | WCONTINUED | WUNTRACED);
			printf("\t[Handler] pid:%d signal:%s wstatus %d \n", wait_pid, signal2str(signal), wait_status);
			
			if (wait_pid == -1) {
				//printf("\t[Handler] pid:%d signal:%s \n", w, signal2str(signal));
				perror("waitpid");
				break;
				//exit(EXIT_FAILURE);		
			}
			
			if (WIFEXITED(wait_status)) {
				printf("Background job %u exited with code=%u\n", wait_pid, WEXITSTATUS(wait_status));
				
			} 
			else if (WIFSIGNALED(wait_status)) {
				printf("WIFSIG Background job %u exited\n", wait_pid);
			} 
			else if (WIFSTOPPED(wait_status)) {
				printf("Background job %u exited, stopped by signal %u\n", wait_pid, WSTOPSIG(wait_status));
			} 
			else if (WIFCONTINUED(wait_status)) {
				printf("Background job %u continued\n", wait_pid);
			}
		} 
		while (
			!WIFEXITED(wait_status) && !WIFSIGNALED(wait_status)
			// ! (wait_pid > 0 && (
			// 	WIFSTOPPED (wait_status)
			// 	|| WIFSIGNALED (wait_status)
			// 	|| WIFEXITED(wait_status)
			// ))
		);
	}
}


void setup_chld() {
	struct sigaction act = {
		.sa_flags = 0,
		.sa_handler = &chld_handler
	};
	// if the signal SIGCHLD instead of ignoring as by default, we take into account the sigaction sa:
	sigaction(SIGCHLD, &act, NULL);
}


void setup() {
	printf("[setup]\n");
	setup_chld();
	setup_int();
}

// A ​background​ job is the opposite. It does not take over the shell’s interface, meaning that the shell still receives commands while the background processes execute and does not interrupt the shell. Because background jobs do not take over the shell’s interface, it is possible for a shell to execute many background jobs at once.
pid_t run_background_job(char** argv){
	pid_t pid = fork();
	struct sigaction sa_son;
	sa_son.sa_handler = SIG_DFL;

	// setup_chld();
	if (pid == 0) {
		if (sigaction(SIGTERM, &sa_son, NULL) == -1){
			raise_error("set SIGTERM");
		}
		if (sigaction(SIGQUIT, &sa_son, NULL) == -1) {
			raise_error("set SIGQUIT");
		}
		if (sigaction(SIGINT, &sa_son, NULL) == -1) {
			raise_error("set SIGQUIT");
		}
		// associate between standard with /dev/null which can check if content is well written and is deleted soon after (no conflict with parent shell entry)
		freopen("/dev/null", "w", stdin);
		
		int op_status = execvp(argv[0], argv);

		if (op_status == -1) {	
			fprintf(stderr, "Failed to execute: %s\n", argv[0]);
      		perror("Reason");
		}
	}
	else {
		printf("[run_background_job] %u\n", pid);
	}
	return pid;
}


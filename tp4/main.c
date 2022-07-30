#include "main.h"
#include "utils.h"


void input(int fd) {
	char *str = NULL;
	pid_t pid = getpid();
	while(1) {
		printf("PID=%ld> ", (long)pid);

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

		// Type `q` to exit from loop
		if (strcmp(str, "q") == 0) break;

		if (strcmp(str, "?") == 0) {
			printf("%s\n", HELP_MESSAGE);
		} else {
			// number of lines
			int nlines = 0;

			char** lines = parse_line(str, &nlines);

			if (lines == NULL) {
				printf("Can't parse\n");
				continue;
			}

			//// print_lines(lines);
			//// printf("N %d\n", nlines);

			if (nlines < 4) {
				printf("Not enough arguments\n");
				continue;
			}

			char* cmd = lines[0];
			char* l_type = lines[1];
			//  's' (SEEK_SET, default)
			char* whence = lines[4] != NULL ? lines[4] : "s"; 

			// Starting offset for lock
			off_t start = (off_t)parse_int(lines[2]);

			off_t length = (off_t)parse_int(lines[3]);

			if (start < 0 || length < 0 ) {
				printf("Unvalid start or length values\n");
				continue;
			}

			//printf("[%ld %ld] %s %s %s\n", start, length, cmd, l_type, whence);
			//printf("F_RDLCK %i F_WRLCK %i SUCCESS: %i Fail: %i \n", F_RDLCK, F_WRLCK, EXIT_SUCCESS, EXIT_FAILURE);


			short lock_type_id = -1;
			short whence_id = SEEK_SET;
			short cmd_id = -1;

			// Type of lock
			if (strcmp(l_type, "r") == 0) lock_type_id = F_RDLCK;
			if (strcmp(l_type, "w") == 0) lock_type_id = F_WRLCK;
			if (strcmp(l_type, "u") == 0) lock_type_id = F_UNLCK;

			// How to interpret l_start
			if (strcmp(whence, "c") == 0) whence_id = SEEK_CUR;
			if (strcmp(whence, "e") == 0) whence_id = SEEK_END;

			if (strcmp(cmd, "g") == 0) cmd_id = F_GETLK;
			if (strcmp(cmd, "s") == 0) cmd_id = F_SETLK;
			if (strcmp(cmd, "w") == 0) cmd_id = F_SETLKW;

			if (lock_type_id == -1) {
				printf("Can't parse lock type '%s'\n", l_type);
				continue;
			}
			if (cmd_id == -1) {
				printf("Can't parse cmd '%s'\n", cmd);
				continue;
			}

			struct flock init_flock = {
				.l_type = lock_type_id,
				.l_whence = whence_id,
				.l_start = start,
				.l_len = length,
				.l_pid = pid,
			};

			OperationState op_state = set_ctrl(fd, cmd_id, init_flock);
			print_operation_state(op_state);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) raise_error("Not enough arguments");

	int fd = open(argv[1], O_RDWR);

	if (fd == -1) raise_error("Not a file");

	printf("%s\n", HELP_CMD_MESSAGE);
	input(fd);

	return EXIT_SUCCESS;
}

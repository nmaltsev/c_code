#include "utils.h"


void raise_error(const char* message){
	printf("%s\n", message);
	exit(EXIT_FAILURE);
}

int count_substr(char* str, char* substr) {
	int count = 1;
	char* pos = str;
	
	while ((pos = strstr(pos, substr)) != NULL) {
		count++;
		pos++;
	}
	
	return count;
}

char** parse_line(char* line, int* nlines) {
	if (strlen(line) == 0) return NULL;
	char* delim = " ";
	
	int n = count_substr(line, delim);
    *nlines = n;
	
	char** lines = malloc((sizeof(char *) + 1) * n);

	if (lines == NULL) {
		// Memory allocation failure
		return NULL;
	}
	size_t i = 0;
	// The strsep() function is intended as a replacement for the strtok() function.
	char* subline;
	while ((subline = strsep(&line, delim)) != NULL) {
		if (strlen(subline) == 0) continue;
		*(lines + i++) = strdup(subline);
	}
	*(lines + i) = 0;
	return lines;
}

void print_lines(char** lines) {
	size_t i;
	for (i = 0; *(lines + i); i++) {
		printf("#%d %d %s\n", i, strlen(*(lines + i)), *(lines + i));
		//free(*(lines + i));
	}
}

int parse_int(char* num) {
    if (num == NULL) return -1;

    int n = atoi(num);

    if (n == 0 && strcmp(num, "0") != 0) return -1;

    return n;
}

 


OperationState set_ctrl(int fd, int cmd, struct flock init){
	struct flock copy_flock = {
		.l_type = init.l_type,
		.l_whence = init.l_whence,
		.l_start = init.l_start,
		.l_len = init.l_len,
		.l_pid = init.l_pid
	};
	
	int op_status = fcntl(fd, cmd, &copy_flock);
	
	OperationState out = {
		.init_flock = init,
		.final_flock = copy_flock,
		.status = op_status,
		.error = errno,
		.executed_cmd = cmd,
	};
	return out;
}

// For debugging
void print_state_diff(OperationState op_state) {
	printf("[Init flock] pid:%6d, type:%4d, whence:%4d, start:%3ld, len:%3ld  \n", 
		op_state.init_flock.l_pid,
		op_state.init_flock.l_type,
		op_state.init_flock.l_whence,
		op_state.init_flock.l_start,
		op_state.init_flock.l_len
	);
	printf("[Finl flock] pid:%6d, type:%4d, whence:%4d, start:%3ld, len:%3ld  \n", 
		op_state.final_flock.l_pid,
		op_state.final_flock.l_type,
		op_state.final_flock.l_whence,
		op_state.final_flock.l_start,
		op_state.final_flock.l_len
	);
	
};


char* whence_description(short l_whence) {
	char* value;
	switch(l_whence) {
		case SEEK_SET: value = "The beginning"; break;
		case SEEK_CUR: value = "Where fd points"; break;
		case SEEK_END: value = "The end"; break;
		default: value = "-"; break;
	}
	return value;
}

char* lock_type_description(short l_type) {
	char* value;
	switch(l_type) {
		case F_RDLCK: value = "Read"; break;
		case F_WRLCK: value = "Write"; break;
		case F_UNLCK: value = "No lock"; break;
		default: value = "-"; break;
	}
	return value;
}


void print_operation_state(OperationState op_state) {
	// For debug
	printf("Op status: %d error: %d\n", op_state.status, op_state.error);
	
	if (op_state.executed_cmd == F_SETLK) {
		// F_SETLK Set or clear a file segment lock according to the lock description pointed to by the third argument, arg, taken as a pointer to type struct flock, defined in <fcntl.h>. F_SETLK can establish shared (or read) locks (F_RDLCK) or exclusive (or write) locks (F_WRLCK), as well as to remove either type of lock (F_UNLCK). F_RDLCK, F_WRLCK, and F_UNLCK are defined in <fcntl.h>. If a shared or exclusive lock cannot be set, fcntl() shall return immediately with a return value of -1.
		print_state_diff(op_state);
		
		if (op_state.status != -1) {
			printf(
				op_state.final_flock.l_type == F_UNLCK 
					? "[PID=%d] Got unlock\n"
					: "[PID=%d] Got lock\n", 
				op_state.final_flock.l_pid
			);
		} else {
			if (op_state.error == EACCES || op_state.error == EAGAIN) {
				// [EACCES] or [EAGAIN] mean that the type of lock ( l_type) is a shared (F_RDLCK) or exclusive (F_WRLCK) lock and the segment of a file to be locked is already exclusive-locked by another process, or the type is an exclusive lock and some portion of the segment of a file to be locked is already shared-locked or exclusive-locked by another process.
				printf("Lock is held by another process\n");
				// It is impossible to identify the number of process that holds the lock
			} else {
				printf("Another error %d\n", op_state.error);
			}
		}
	}
	else if (op_state.executed_cmd == F_SETLKW) {
		// F_SETLKW This command shall be equivalent to F_SETLK except that if a shared or exclusive lock is blocked by other locks, the thread shall wait until the request can be satisfied. If a signal that is to be caught is received while fcntl() is waiting for a region, fcntl() shall be interrupted. Upon return from the signal handler, fcntl() shall return -1 with errno set to [EINTR], and the lock operation shall not be done.
		if (op_state.status != -1) {
			print_state_diff(op_state);
			printf("[PID=%d] Got lock\n", op_state.final_flock.l_pid);
		} else {
			if (op_state.error == EINTR) {
				// [EINTR] The cmd argument is F_SETLKW and the function was interrupted by a signal.
				printf("fcntl() function executiond was interrupted by a signal\n");
			} else {
				printf("Another error %d\n", op_state.error);
			}
		}
	}	
	else if (op_state.executed_cmd == F_GETLK) {
		// F_GETLK Get the first lock which blocks the lock description pointed to by the third argument, arg, taken as a pointer to type struct flock, defined in <fcntl.h>. The information retrieved shall overwrite the information passed to fcntl() in the structure flock. If no lock is found that would prevent this lock from being created, then the structure shall be left unchanged except for the lock type which shall be set to F_UNLCK.
		print_state_diff(op_state);
		
		if (op_state.status != 0) {
			printf("IN GETLOCK\n");
			return;
		}
		
		if (op_state.final_flock.l_type == F_UNLCK){
			printf(
				"[PID %d] The lock %s %ld:%ld starting from %s can currently be set without conflict\n",
				op_state.final_flock.l_pid,
				lock_type_description(op_state.init_flock.l_type),
				op_state.final_flock.l_start,
				op_state.final_flock.l_start + op_state.final_flock.l_len,
				whence_description(op_state.init_flock.l_whence)
			);
		} else {
			if (op_state.final_flock.l_pid == -1) {
				printf(
					"Le lock %s ne peut pas être mis en place sur le fichier voulu, car celui-ci est ouvert\n",
					lock_type_description(op_state.final_flock.l_type)
				);
				return;
			}
			printf(
				"[PID=%d] Denied by %s lock on %ld:%ld (held by PID %d)\n",
				op_state.init_flock.l_pid,
				lock_type_description(op_state.final_flock.l_type),
				op_state.final_flock.l_start,
				op_state.final_flock.l_start + op_state.final_flock.l_len,
				op_state.final_flock.l_pid
			);
		}
	}	
}

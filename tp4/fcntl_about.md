[about](https://pubs.opengroup.org/onlinepubs/007904875/functions/fcntl.html)


RETURN VALUE

    Upon successful completion, the value returned shall depend on cmd as follows:

    F_DUPFD
        A new file descriptor.
    F_GETFD
        Value of flags defined in <fcntl.h>. The return value shall not be negative.
    F_SETFD
        Value other than -1.
    F_GETFL
        Value of file status flags and access modes. The return value is not negative.
    F_SETFL
        Value other than -1.
    F_GETLK
        Value other than -1.
    F_SETLK
        Value other than -1.
    F_SETLKW
        Value other than -1.
    F_GETOWN
        Value of the socket owner process or process group; this will not be -1.
    F_SETOWN
        Value other than -1.

    Otherwise, -1 shall be returned and errno set to indicate the error.

ERRORS

    The fcntl() function shall fail if:

    [EACCES] or [EAGAIN]
        The cmd argument is F_SETLK; the type of lock ( l_type) is a shared (F_RDLCK) or exclusive (F_WRLCK) lock and the segment of a file to be locked is already exclusive-locked by another process, or the type is an exclusive lock and some portion of the segment of a file to be locked is already shared-locked or exclusive-locked by another process.
    [EBADF]
        The fildes argument is not a valid open file descriptor, or the argument cmd is F_SETLK or F_SETLKW, the type of lock, l_type, is a shared lock (F_RDLCK), and fildes is not a valid file descriptor open for reading, or the type of lock, l_type, is an exclusive lock (F_WRLCK), and fildes is not a valid file descriptor open for writing.
    [EINTR]
        The cmd argument is F_SETLKW and the function was interrupted by a signal.
    [EINVAL]
        The cmd argument is invalid, or the cmd argument is F_DUPFD and arg is negative or greater than or equal to {OPEN_MAX}, or the cmd argument is F_GETLK, F_SETLK, or F_SETLKW and the data pointed to by arg is not valid, or fildes refers to a file that does not support locking.
    [EMFILE]
        The argument cmd is F_DUPFD and {OPEN_MAX} file descriptors are currently open in the calling process, or no file descriptors greater than or equal to arg are available.
    [ENOLCK]
        The argument cmd is F_SETLK or F_SETLKW and satisfying the lock or unlock request would result in the number of locked regions in the system exceeding a system-imposed limit.
    [EOVERFLOW]
        One of the values to be returned cannot be represented correctly.
    [EOVERFLOW]
        The cmd argument is F_GETLK, F_SETLK, or F_SETLKW and the smallest or, if l_len is non-zero, the largest offset of any byte in the requested segment cannot be represented correctly in an object of type off_t.

    The fcntl() function may fail if:

    [EDEADLK]
        The cmd argument is F_SETLKW, the lock is blocked by a lock from another process, and putting the calling process to sleep to wait for that lock to become free would cause a deadlock.


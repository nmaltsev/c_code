
## Compilation

`make clean && clear && make`

## Examples

To show the attributes of a single file: `./ultra-cp file1`
To list the files in a directory: `./ultra-cp dir`
```
$ ./ultra-cp ./
drwxrwxr-x       4096 Thu Nov  4 14:16:37 2021 ./
-rw-rw-r--      21392 Thu Nov  4 14:16:37 2021 ./utils.o
-rw-r--r--        878 Thu Nov  4 14:10:49 2021 ./readme.md
-rw-r--r--       3872 Thu Nov  4 14:03:27 2021 ./main.c
-rw-r--r--       1159 Thu Nov  4 09:44:45 2021 ./utils.h
-rw-rw-r--      12128 Thu Nov  4 14:16:37 2021 ./main.o
-rw-r--r--         83 Sat Oct 30 00:17:57 2021 ./options.h
-rw-r--r--        642 Sat Oct 30 00:16:26 2021 ./options.c
-rw-r--r--       7601 Thu Nov  4 14:16:36 2021 ./utils.c
-rw-rw-r--       7256 Thu Nov  4 14:16:37 2021 ./options.o
-rwxrwxr-x      34160 Thu Nov  4 14:16:37 2021 ./ultra-cp
-rw-r--r--        701 Thu Nov  4 09:32:25 2021 ./Makefile
-rw-r--r--        120 Thu Nov  4 09:37:34 2021 ./main.h
```

To copy files / directories to a specific directory:: `./ultra-cp file1 link1 dir1 file2 /tmp`
```
$ ./ultra-cp /tmp/test3 /tmp/test6
drwxrwxr-x       4096 Thu Nov  4 14:07:37 2021 /tmp/test6/test3
-rw-rw-r--          4 Thu Nov  4 13:37:41 2021 /tmp/test6/test3/tst.txt
drwxrwxr-x       4096 Thu Nov  4 14:07:37 2021 /tmp/test6/test3/test31
lrwxrwxrwx         18 Thu Nov  4 14:07:37 2021 /tmp/test6/test3/test31/link_test
-rw-rw-r--          4 Thu Nov  4 14:06:12 2021 /tmp/test6/test3/test31/out.txt
-rw-rw-r--          4 Thu Nov  4 13:37:53 2021 /tmp/test6/test3/tst2.txt

```


## Useful commands

To create a soft link: `ln -sf <file> <link>`
To create a hard link: `ln <file> <link>`
To verify soft or hard link in Linux: `ls -l <link>`

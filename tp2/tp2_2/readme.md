## Installation && compilation
```
sudo apt-get install libssl-dev
make

```

## MD5 hash of a string
```
$ echo -n "abc 123" | md5sum
c89cfdb5dd9f56836f59fba6c062dda4  -
$ ./hash -t abc 123
c89cfdb5dd9f56836f59fba6c062dda4
```


## SHA1 hash of a string
```
$ ./hash abc 123
e2d0a343442ba7bd2c0537659a05e61668575f2b
$ echo -n "abc 123" | sha1sum
e2d0a343442ba7bd2c0537659a05e61668575f2b  -
```


## MD5 checksum of files
```
$ md5sum main.c utils.c
7194d15f60fcf281b9cb2f669d6d0ffe  main.c
dcde9799ed6e635d5968275ee2915e90  utils.c
$ ./hash -ft main.c utils.c
7194d15f60fcf281b9cb2f669d6d0ffe main.c
dcde9799ed6e635d5968275ee2915e90 utils.c
```


## SHA1 checksum of files
```
$ sha1sum main.c utils.c
8d5488ee03f35be2cab7ec93e57b870ded2f0a58  main.c
ef44f945457300b133a939b8514decab6e24c680  utils.c
$ ./hash -f main.c utils.c
8d5488ee03f35be2cab7ec93e57b870ded2f0a58 main.c
ef44f945457300b133a939b8514decab6e24c680 utils.c
```

http://www.sha1-online.com/


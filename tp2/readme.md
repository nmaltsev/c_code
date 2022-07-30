## Installation && compilation

```
sudo apt-get install libssl-dev
make
```

## Examples
```
$ ./hash abc 123
e2d0a343442ba7bd2c0537659a05e61668575f2b

$ ./hash -t abc 123
c89cfdb5dd9f5683c6f7a4b50b560000

$ ./hash -f main.c utils.c
2146fcf504e09b26b0c12ccc4371af0031c59d72 main.c
307016edf57c440dcdf891db5aa29925faef48a5 utils.c

$ ./hash -ft main.c utils.c
e1786482d88e71d09ed68be921703159 main.c
7a3fefeeb6551f4adb94e0fd0995b6e6 utils.c
```


http://www.sha1-online.com/

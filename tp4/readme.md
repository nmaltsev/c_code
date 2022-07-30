
## Compilation

`make clean && clear && make`

## Examples

### From Terminal #1
```
$ ./lockcmd bla.txt 
Enter ? for help
PID=22101> s w 0 5
State 0 0
[PID=22101] Got lock
PID=22101> 

```

### From terminal #2
```
$ ./lockcmd bla.txt 
Enter ? for help
PID=22136> g r 0 5
State 0 0
[Init flock] pid: 22136, type:   0, whence:   0, start:  0, len:  5  
[Finl flock] pid: 22101, type:   1, whence:   0, start:  0, len:  5  
[PID=22136] Denied by Write lock on 0:5 (held by PID 22101)
PID=22136> 

```

# MallocRevamped
In this assignment I built my own implementation of malloc and free by implementing a library that interacts with the operating system to perform heap management on behalf of a user process.

## What I Learned
- Memory block splitting and coalescing  
- 4 Heap Management Algorithms: Best Fit, Worst Fit, Next Fit, First Fit  
- Memory Allocation functions: malloc, calloc, realloc and free  
- Heavy Pointer Arithmetic  
- gdb Debugging  

## Usage
To build the code, change to top level directory and type:
```
make
```
The following example shows running the ffnf test using the First Fit shared library:
```
$ env LD_PRELOAD=lib/libmalloc-ff.so tests/ffnf
```

To run the other heap management schemes replace libmalloc-ff.so with the appropriate library:
```
Best-Fit: libmalloc-bf.so
First-Fit: libmalloc-ff.so
Next-Fit: libmalloc-nf.so
Worst-Fit: libmalloc-wf.so
```

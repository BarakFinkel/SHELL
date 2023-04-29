# Operating Systems: Assignment 1 - SHELL

To run any part of the project, make sure to run your terminal from within the project's directory.

## Part A:

In this part, we have implemented copy and compare tools for files.

### Usage:

If this is your first time running the program, make sure to enter "make all" in your terminal before anything.

For your comfort, we already have 3 examples of files added with the project: file1.txt, file2.txt, file3.txt

*Note - file1 and file3 have the same contents when ignoring cases.

#### Copy:

```sh
  ./copy <file1> <file2> -f -v
```
*Note- -f and -v are optional flags, while -f forces the file to copy even if the target file exists, and -v simply prints verbal details.

#### Compare:

```sh
  ./cmp <file1> <file2> -i -v
```
*Note- -i and -v are optional flags, while -i sets the comparator to ignore cases between letter chars, and -v simply prints verbal details.



## Part B:

In this part, we have implemented 2 methods of encoding/decoding a string.
- CodecA: switches between low-upper cases of the *letter* chars in the string.
- CodecB: adds/subtract 3 in ascii from the chars in the string.

### Usage:

If this is your first time running the program, make sure to enter the following in your terminal before anything.

```sh
  export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
  make all
```

#### Encode:

```sh
  ./encode <codecA / codecB> <YourString>
```

#### Decode:
 
```sh
  ./encode <codecA / codecB> <YourString>
```
 
## Part C:
 
In this part, we have implemented a simulation of a bash terminal.
Specifically, the parts of asking our OS to execute commands, the piping of them, and the redirections of I/O from them.
 
### Usage:
 
If this is your first time running the program, make sure to enter "make all" in your terminal before anything.

```sh
  ./stshell
```
Afterwards, you can write naturaly as if you're writing in your normal terminal.

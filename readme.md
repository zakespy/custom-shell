# Custom Shell Program

This is a simple custom shell program written in C. It supports basic shell commands like `ls`, `cd`, `exit`, and includes a `help` command with documentation on how to use them.

## Features

1. **ls**: Lists the contents of the current directory or a specified directory.
2. **cd**: Changes the current directory to the specified one.
3. **exit**: Exits the shell and terminates the program.
4. **help**: Provides a basic description of the available commands (`ls`, `cd`, `exit`).

## Process Tree

The following diagram shows the process tree, which indicate the shell is running and it created child process

## Compilation and Execution

### Using Makefile:
To compile and run the shell, use the following commands:

- **Build the shell:**

  ```bash
  make
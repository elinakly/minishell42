# Minishell to-do list
What we need to do, how, when, why, where, etc? Split up in two parts: parsing and executing!

## Parsing
- Expanding variables into strings
- Reconstructing command string with expanded variables
- Creating token structures
- Creating action structures
- Figuring out how to get file descriptors into actions in a way that makes sense
- Turning tokens into actions in the right order for exec
- Provide linkedlist of actions based on command string given by exec
- Find a good way to "detect" syntax errors, or just in general a way to tell exec a command doesn't make sense

## Executing
- Make builtins
- Make interactive input work (readline)
- Make an execution loop (the thing that keeps minishell running)
- Separate the pipex functionality into different functions
	- Something like:
	- Create pipe
	- Create input redirect
	- Create output redirect
	- Execute command with arguments and custom input/output file descriptors?
	- Restore file descriptors (defaulting back to stdin/stdout I guess)
- Get and store existing environment variables
- Make command history (probably in some .history file)
	- Find last line and compare to current command to avoid double entries in history
- Handle newly created environment variables from `export`
- Make a function that returns the value of an environment variable with a given name
- Find out how non-interactive bash works and make that work (ex.: `bash -c "echo a"`)
- Signal handling (CTRL+C / CTRL+D at least, perhaps other kill signals to clean up before exiting)


## Undecided
- Heredoc handling
- Unclosed string handling


## General stuff
- Decide on a good project structure
- Find out of a structure other than "all files in the root folder" is allowed
- Add documentation to functions
- Endless testing for segfaults and memory leaks and stuff
- We probably need 1 big struct with EVERYTHING as our global variable that we make accessible in all files
- Probably make a cool readme file with all the features we do and don't support and how to use them

## Optional, up for discussion
- Doing the bonus (lol)
- Functions to easily send colored output (for errors, for readline, etc.)
- Making a cool ASCII art logo for on startup
- Give it a name other than Minishell (something more creative)
- Maybe `-h` or `--help` flag support that just prints which features our minishell supports

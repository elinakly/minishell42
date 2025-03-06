# Minishell Parsing
This document will be used to store any information I find and interpret about parsing requirements for minishell.

## Meta Characters
These seem to be characters that make stuff do other stuff. Think pipes, escapes, pattern matching, command substitution, redirects, etc.

### Definitely relevant
- `\` - Escapes other meta characters
- `'` - Ignores any meta characters, so it even ignores escapes. Must be closed.
- `"` - Ignores any meta characters except `$` and `\` in bash, but minishell subject is unclear regarding `\`. Must be closed.
- `>` - Redirects output. Unsure about specifics.
- `<` - Tedirects input. Also unsure about specifics.
- `>>` - Also redirects output, but in append mode. Somehow that makes more sense.
- `<<` - I think this is called a heredoc? Followed by a (sequence of) character(s) as delimiter, after which a newline must follow and input must be continue to be prompted and interpreted as lines until a line with the exact delimiter string appears. Need to do more research on how these work.
- `|` - Pipes. I am still unsure how they even work, haven't done pipex.
- `$` - Used for many things, but definitely relevant for `$EXAMPLE` environment variable expansion. These work directly as well as in double quotes. Not sure about heredoc yet.
- `$?` - Expands to the exit status of the most recently executed command (foreground pipeline specifically, whatever that means).

### Possibly relevant
- `` ` `` - Single backticks can execute the command inside of itself, seems to be outdated but may need to be supported. Must be closed. Example: `` `date` ``
- `$()` - Executes the command inside of the parentheses. Modern alternative of of the single backticks and thus may also need to be supported. example: `$(date)`
- `{}` - Groups multiple commands together. Not specifically mentioned in subject but I personally hope we can include this. Unless you can nest them, that's a bit much. Ok you can nest them but recursion maybe?! Example: `{echo 1; echo 2;} > file`
- `;` - Separates multiple commands. Is mentioned somewhere in the subject but I am unsure what they mean. We may need to support this.
- `:` - Does nothing. I am not sure what that's good for, but it literally does nothing, surely that's easy to implement!
- `#` - It's a comment. In normal, single-line commands, anything after `#` is ignored as it is considered a comment. May or may not have to be supported.
- `${}` - Alternative to just `$VAR` for expanding environment variables. Quite likely to be required in minishell despite no mention of it.

## Storing Parsed Data
Linked list struct with each action? Something like\
```grep "world" < input | cat -n > output```\
Would give the following actions linkedlist:\
Action[type=REDIR_INPUT] -> Action[type=EXEC_COMMAND] -> Action[type=PIPE] -> Action[type=EXEC_COMMAND] -> Action[type=REDIR_OUTPUT]

~~No idea if that makes any logical sense and unsure how to pass results of each action around, but it's something.~~ After having talked to other people this way of action types doesn't really make sense, a pipe doesn't need its own
action, but rather an EXEC_COMMAND action needs to have pipe/redir flags etc probably!

This is currently super outdated and will not see updates until I am further into parsing

## Interpreting each token
I've done some research on how bash interprets tokens and it's a lot to remember so I will store some examples here:

`echo a > test.txt b` here `test.txt` is interpreted as the outfile, and `b` becomes the second argument for echo. Bash first opens the outfile, then echoes `a` and `b` into it.

`cat < test.txt a` here `test.txt` is interpreted as the outfile, and `a` becomes the second argument for echo. Because cat uses arguments as filenames as well, it will take `a` as a second file to display. 

`cat < test.txt > text.txt` opens `test.txt` for cat to read, but then the output redirection also opens and truncates `test.txt` before `cat` can read from it, resulting in an empty text file and no output from `cat`.

`> a < a` is shitty but should work, since there is no command, bash will do `no-op` (fancy word for nothing), and redirect the output of said nothing to the file `a`, but first it will also want to read file `a` and redirect it as input into nothing yet again. Ideally this kind of nonsense gets simplified to just opening `a` for output, truncating it and doing nothing else. Send help. Please.

Now just need to make sure that arguments after infile/outfile are linked back to the right command, but should be doable. 

Worth noting: input and output files can never come before a redirect operator, they only ever come after.

TODO: Find more info about how arguments behave for a `no-op`

### Heredocs stuff for this section
If a heredoc (`<<`) is present in the command anywhere, that takes priority even over input and output redirections. The heredoc must be completed before anything else. It first stores all the text in memory, and upon heredoc completion, writes it all to a temporary file (usually stored in `/tmp/`). That file will then be used as infile like a regular `<` operator. Similarly to those operators, any arguments that come after a heredoc delimiter are then linked back to the command that came before the heredoc / operator.

## Random supposedly useful stuff
- ENV has 'PWD' and 'OLD_PWD' variables that keep track of your working directory, MAY be able to use those?

## Questions to find answers to
- What happens if env var has quote characters in it?
- What characters are *actually* allowed in env vars?
- What's up with the single-digit var names and why does `echo $123` output `23`?
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
Yep that's WIP
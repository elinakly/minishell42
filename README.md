# Minishell

This project is part of the 42 School curriculum. **Minishell** is a simple Unix shell implementation in C that replicates key features of Bash.

---

## 🛠️ Features

✅ Command parsing (with quotes, special characters)
✅ Built-in commands (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`)
✅ Execution of external programs using PATH
✅ Pipes (`|`) and redirections (`>`, `<`, `>>`, `<<`)
✅ Environment variable handling
✅ Custom prompt display
✅ Command history (without using `read_history()`)

---

## ⚙️ How to Build

```bash
git clone https://github.com/elinakly/minishell42.git
cd minishell42
make
```

---

## 🚀 Usage

```bash
./minishell
```

You’ll see a custom prompt where you can run commands like:

* `ls -la | grep minishell`
* `echo $HOME`
* `cat file.txt > out.txt`

---

## 📚 Project Rules

* Only allowed to use specific system calls and standard library functions
* Must handle signals (CTRL+C, CTRL+) properly
* Built-in commands must work **with and without** pipes

---

## 💁 File Structure

```
minishell/
├── src/          # source files
├── include/      # header files
├── lib/          # libraries (if any)
├── Makefile
```

---

## 💡 What I Learned

* Building a shell from scratch
* Process management (fork, exec, wait)
* File descriptors and pipes
* Signal handling
* Memory management and avoiding leaks
* Linked lists for command management

---

## 💊 Author

Made by [Me](https://github.com/elinakly) and [Mika](https://github.com/put) at 42 School.

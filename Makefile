NAME = minishell
CC = cc
SRC_DIR = source
OBJS_DIR  = objects
LIB_DIR = lib
LIBFT_REPO = https://github.com/put/libft
LIBFT_COMMIT = d20ca5b
HEADERS	:= -I ./include -I $(LIB_DIR)/libft
CFLAGS = -g
LDFLAGS = -lreadline

SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/exec/builtins/cd.c \
		$(SRC_DIR)/exec/builtins/clear.c \
		$(SRC_DIR)/exec/builtins/echo.c \
		$(SRC_DIR)/exec/builtins/env.c \
		$(SRC_DIR)/exec/builtins/exit.c \
		$(SRC_DIR)/exec/builtins/export.c \
		$(SRC_DIR)/exec/builtins/is_builtin.c \
		$(SRC_DIR)/exec/builtins/pwd.c \
		$(SRC_DIR)/exec/builtins/unset.c \
		$(SRC_DIR)/exec/execute/venv/venv.c \
		$(SRC_DIR)/exec/execute/venv/venv_helpers.c \
		$(SRC_DIR)/exec/execute/execute.c \
		$(SRC_DIR)/exec/execute/open_redir.c \
		$(SRC_DIR)/exec/execute/pipes.c \
		$(SRC_DIR)/exec/execute/utils.c \
		$(SRC_DIR)/exec/execute/signals/signals.c \
		$(SRC_DIR)/exec/execute/signals/signals2.c \
		$(SRC_DIR)/exec/promt/ft_readline.c \
		$(SRC_DIR)/exec/promt/history.c \
		$(SRC_DIR)/exec/errors.c \
		$(SRC_DIR)/parse/heredoc/heredoc_helpers.c \
		$(SRC_DIR)/parse/heredoc/heredoc.c \
		$(SRC_DIR)/parse/memory/cleanup.c \
		$(SRC_DIR)/parse/memory/cmds_count.c \
		$(SRC_DIR)/parse/memory/free_cmds_redir.c \
		$(SRC_DIR)/parse/memory/free_tokens_var.c \
		$(SRC_DIR)/parse/path/path.c \
		$(SRC_DIR)/parse/structbuild/get_info.c \
		$(SRC_DIR)/parse/structbuild/insert.c \
		$(SRC_DIR)/parse/structbuild/structbuild.c \
		$(SRC_DIR)/parse/structbuild/validate.c \
		$(SRC_DIR)/parse/tokenize/meta_tokenize.c \
		$(SRC_DIR)/parse/tokenize/skipIT.c \
		$(SRC_DIR)/parse/tokenize/tokenize_helpers.c \
		$(SRC_DIR)/parse/tokenize/tokenize_utils.c \
		$(SRC_DIR)/parse/tokenize/tokenize.c \
		$(SRC_DIR)/parse/vars/math.c \
		$(SRC_DIR)/parse/vars/vars_helpers.c \
		$(SRC_DIR)/parse/vars/vars.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(LIB_DIR)/libft/libft.a $(NAME)

$(LIB_DIR)/libft:
	git clone $(LIBFT_REPO) $(LIB_DIR)/libft
	cd $(LIB_DIR)/libft && git checkout $(LIBFT_COMMIT)

$(LIB_DIR)/libft/libft.a: $(LIB_DIR)/libft
	make -C $(LIB_DIR)/libft all

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS) $(LIB_DIR)/libft/libft.a
	@$(CC) $(OBJS) $(LIB_DIR)/libft/libft.a $(HEADERS) $(LDFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	make -C $(LIB_DIR)/libft clean

fclean: clean
	rm -rf $(NAME)
	make -C $(LIB_DIR)/libft fclean
	rm -rf $(LIB_DIR)/libft

re: fclean all

.PHONY: all re fclean clean
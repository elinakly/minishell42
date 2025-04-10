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

## SRCS = $(shell find $(SRC_DIR) -type f -name "*.c" ! -path "$(SRC_DIR)/exec/builtins/*")
#TODO: Probably hardcode all filenames once we're nearly done with project
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(LIB_DIR)/libft/libft.a  minishell_tester_lib $(NAME)

minishell_tester_lib:
	git submodule update --init minishell_tester

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
	git submodule deinit -f --all

re: fclean all

.PHONY: all re fclean clean
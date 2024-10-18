NAME = minishell

CC = clang
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -lreadline

INC_PATH = .
SRC_PATH = ./src/
OBJ_PATH = ./obj/
OBJ_FOLD = utils builtins parser test

SRC_FILES = minishell.c \
	utils/compare_str.c \
	utils/get_str_len.c \
	utils/duplicate_str.c \
	utils/join_str.c \
	utils/split_str.c \
\
	builtins/ms_cd.c \
	builtins/ms_echo.c \
	builtins/ms_env.c \
	builtins/ms_exit.c \
	builtins/ms_export.c \
	builtins/ms_pwd.c \
	builtins/ms_unset.c \
\
	parser/command.c \
	parser/command_table.c \
	parser/parser.c \
\
	test/hub_exec.c \
	test/str_dup.c \
	test/str_len.c \
	test/ft_strjoin.c \

LIBS = $(addprefix $(LIB_PATH),$(LIB_NAME))
SRCS = $(addprefix $(SRC_PATH),$(SRC_FILES))
OBJS = $(addprefix $(OBJ_PATH),$(SRC_FILES:.c=.o))
OBJS_FOLD = $(addprefix $(OBJ_PATH),$(OBJ_FOLD))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(OBJS) -I $(INC_PATH)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INC_PATH)
$(OBJ_PATH) : | $(OBJS_FOLD)
	mkdir -p $(OBJ_PATH)
$(OBJS_FOLD) :
	mkdir -p $(OBJS_FOLD)

fclean : clean
	rm -rf $(NAME)
clean :
	rm -rf $(OBJ_PATH)


re : fclean all

.PHONY : all bonus fclean clean re

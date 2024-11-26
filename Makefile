NAME		:= so_long

SRCS_DIR	:= ./src
SRCS		:= $(wildcard $(SRCS_DIR)/*.c)
OBJS_DIR	:= ./obj


OBJS		:= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
CC			:= gcc
INCLUDES	:= -Ilibft -Iincludes

LIB_FT		:= -Llibft -lft
LIBRARY		:= $(LIB_FT)
INCLUDES	:= -Ilibft -Iincludes -Iminilibx -Lminilibx -lmlx -L/usr/lib -lXext -lX11 -lm -lz


all: $(NAME)

bonus: $(OBJS)
	@echo "Compiling Bonus $(NAME)..."
	@make bonus -C ./libft
	$(CC) $(OBJS) $(FLAGS) $(LIBRARY) $(INCLUDES) -o so_long_bonus
	@echo "$(NAME) compiled successfully."

$(NAME): $(OBJS)
	@echo "Compiling $(NAME)..."
	@make -C ./libft
	$(CC) $(OBJS) $(FLAGS) $(LIBRARY) $(INCLUDES) -fsanitize=address -g3 -o $@
	@echo "$(NAME) compiled successfully."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C ./libft
	@echo "Object files have been removed."

fclean: clean
	@rm -rf $(NAME)
	@rm -rf so_long_bonus
	@make fclean -C ./libft
	@echo "Object files and $(NAME) have been removed."

re: fclean all

.PHONY: all clean fclean re
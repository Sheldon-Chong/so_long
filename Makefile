
$(all): *.c
	gcc *.c ./libft/*.c -lmlx -framework OpenGL -framework AppKit
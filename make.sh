
#gcc *.c ./libft/*.c -lmlx -framework OpenGL -framework AppKit -fsanitize=address -g3 && ./a.out
gcc *.c ./libft/*.c -lmlx -framework OpenGL -framework AppKit && Leaks -atExit -- ./a.out
#
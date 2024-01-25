
gcc *.c ./libft/*.c -lmlx -framework OpenGL -framework AppKit -fsanitize=address -g3 -Wall -Werror -Wextra && ./a.out map.ber
#gcc *.c ./libft/*.c -lmlx -framework OpenGL -framework AppKit && Leaks -atExit -- ./a.out map.ber
#
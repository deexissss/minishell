NAME		=	minishell

CC			=	gcc 
FLAG		=	-Wall -Wextra -Werror -lreadline

LIBFT_FILE	=	libft/libft.a
LIBFT		=	$(LIBFT_FILE)

C_FILE		=	minishell.c \

SRC			=	$(C_FILE)
OBJ			=	$(SRC:.c=.o)

BLUE := \033[0;34m
PINK := \033[1;35m
BLUE2 := \033[0;36m

.c.o:
	$(CC) $(FLAG) -c $< -o $@

all: $(NAME)

lib:
	@cd libft && make

$(NAME): lib $(OBJ)
	$(CC) $(OBJ) -o $(NAME)
	@echo "$(BLUE) MINISHELL COMPILATION DONE"

clean:
	@cd libft && make clean
	rm -f $(OBJ)
	@echo "$(BLUE2)CLEAN ALL THE MINISHELL OBJ"
	
fclean:
	@cd libft && make fclean
	rm -f $(NAME)
	@echo "$(PINK)CLEAN EVERYTHING IN MINISHELL"

re: fclean all

.PHONY: all clean fclean re lib
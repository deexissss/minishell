NAME		=	minishell

CC			=	gcc 
FLAG		=	-Wall -Wextra -Werror
R_FLAG		=	-lreadline
LIBFT_FILE	=	libft/libft.a
LIBFT		=	$(LIBFT_FILE)

SRC			=	functions/cd.c functions/echo.c functions/echo_utils.c functions/env.c \
				functions/exit.c functions/export.c functions/export_handle_new_var.c \
				functions/export_setenv.c functions/export_utils.c functions/export_utils2.c \
				functions/pwd.c functions/unset.c execute_commands.c handle_command.c \
				redirections_heredoc.c handle_command2.c handle_command3.c init_shell.c \
				minishell.c pipe.c pipe_utils.c process_command.c redirection_handlers.c \
				redirections.c tokenizer.c utils.c utils2.c utils3.c utils4.c

OBJ			=	$(SRC:.c=.o)

BLUE := \033[0;34m
PINK := \033[1;35m
BLUE2 := \033[0;36m

.c.o:
	@$(CC) $(FLAG) -c $< -o $@

all: $(NAME)

lib:
	@cd libft && make

$(NAME): lib $(OBJ)
	@$(CC) $(OBJ) $(LIBFT) $(R_FLAG) -o $(NAME)
	@echo "$(BLUE)MINISHELL COMPILATION DONE"

clean:
	@cd libft && make clean
	@rm -f $(OBJ)
	@echo "$(BLUE2)CLEAN ALL THE MINISHELL OBJ"
	
fclean: clean
	@cd libft && make fclean
	@rm -f $(NAME)
	@echo "$(PINK)CLEAN EVERYTHING IN MINISHELL"

re: fclean all

.PHONY: all clean fclean re lib
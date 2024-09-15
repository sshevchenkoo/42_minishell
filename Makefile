NAME = minishell.a
PRINTFNAME = libftprintf.a
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
PRINTFDIR = ./ft_lib

SRCS = 	main.c src/check_syntax.c src/check_syntax2.c \
		src/tokenize.c src/tokenize2.c src/parse.c \
		src/parse2.c src/init_shell.c src/update_shell_env.c \
		src/update_shell_env2.c src/shell_env_utils.c \
		src/builtin_cmd.c src/builtin_cmd2.c src/builtin_cmd3.c \
		src/builtin_cmd4.c src/exec/exec1.c src/utils/utils1.c src/utils/utils2.c \
		src/exec/exec2.c src/exec/exec3.c\
		src/exec/quote.c src/exec/quote2.c src/signals.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)
	$(CC) $(CFLAGS) main.c $(NAME) -o minishell -lreadline -lncurses

makelibft:
	@make -C $(PRINTFDIR)
	@cp $(PRINTFDIR)/$(PRINTFNAME) .
	@mv $(PRINTFNAME) $(NAME)

$(NAME): makelibft $(OBJS)
	@ar -r $(NAME) $(OBJS)


clean:
	@rm -f $(OBJS)
	@cd $(PRINTFDIR) && make clean
	
fclean: clean
	@rm -f $(NAME) minishell
	@cd $(PRINTFDIR) && make fclean
	
re: fclean all

.PHONY: all re fclean clean

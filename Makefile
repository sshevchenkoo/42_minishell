NAME = minishell.a
PRINTFNAME = libftprintf.a
CC = cc
CFLAGS = -g 
PRINTFDIR = ./ft_lib

SRCS = 	main.c src/check_syntax.c src/check_syntax2.c src/tokenize.c src/tokenize2.c src/parse.c src/parse2.c

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

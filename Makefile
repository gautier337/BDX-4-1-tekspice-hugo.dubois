##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Nanotekspice
##

NAME =	nanotekspice

SRC = 	Src/AComponent.cpp			\
		Src/main.cpp				\
		Src/Circuit.cpp				\
		Src/FileParser.cpp

OBJ	=	$(SRC:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	g++ -o $(NAME) $(OBJ) -Wall -Wextra -Werror -std=c++2a -pthread

clean:
	rm -f $(OBJ)
	rm -f *.o
	rm -f *~

f:
	make fclean
	make -j

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
NAME = webserv

CXX := c++
CXXFLAGS = #-Wall -Wextra -Werror # -std=c++98 -g

SRCS = main.cpp Server.cpp RequestParser.cpp \
	 MetaVars.cpp Socket.cpp
OBJS := $(SRCS:%.cpp=%.o)
SRCDIR = ./srcs
INCDIR = ./includes

vpath %.cpp $(SRCDIR)

all : $(NAME)

$(NAME) : $(OBJS)
	@printf "\e[33;1m> $(SRCS) [$(CXXFLAGS)] \n> $(NAME) compiled\e[0m\n"
	@$(CXX) $(CXXFLAGS) -I $(INCDIR) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -I $(INCDIR) $< -o $@

clean:
	@printf "\e[31;1m--- $(OBJS)\e[0m\n"
	@rm -rf $(OBJS)

fclean: clean
	@printf "\e[31;1m--- $(NAME)\e[0m\n"
	@rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean

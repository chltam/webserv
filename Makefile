NAME = webserv

CXX := c++
CXXFLAGS = -g #-std=c++98 -g #-Wall -Wextra -Werror  

SRCS = main.cpp Server.cpp RequestParser.cpp ResponseBuilder.cpp\
	AResponse.cpp GetResponse.cpp PostResponse.cpp DeleteResponse.cpp\
	ErrorResponse.cpp MetaVars.cpp Socket.cpp
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

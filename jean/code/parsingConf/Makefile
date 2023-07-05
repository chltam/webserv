NAME = configuration

CXX := c++
CXXFLAGS = #-Wall -Wextra -Werror -std=c++98

SRCS = main.cpp Config.cpp
OBJS := $(SRCS:%.cpp=%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@printf "\e[33;1m> $(SRCS) [$(CXXFLAGS)] \n> $(NAME) compiled\e[0m\n"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@printf "\e[31;1m--- $(OBJS)\e[0m\n"
	@rm -rf $(OBJS)

fclean: clean
	@printf "\e[31;1m--- $(NAME)\e[0m\n"
	@rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean

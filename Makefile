NAME	=	webserv
CC		=	c++
FLAG	=	-Wall -Wextra -Werror -std=c++98 -g3

SRC_DIR	=	src/
OBJ_DIR	=	obj/

#Source
FILES	=	main \
			ConfigFile Location Server \
			CGI exec_rq rq_dir rq_html \
			Request RequestLine URL \
			Poll server \
			ParserUtils Response utils \

SRCS	=	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(FILES)))
OBJS	=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

INCLUDE	=	-I ./include
HNAME	=	CGI ConfigFile Exception Location ParserUtils Poll Request RequestLine Response Server URL
HEADER	=	$(addsuffix .hpp, $(HNAME))

OBJF	=	.cache_exits

$(OBJF) :
	@mkdir -p $(OBJ_DIR)

vpath %.cpp $(SRC_DIR)config $(SRC_DIR)exec $(SRC_DIR)request $(SRC_DIR)server
vpath %.hpp include/

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAG) $(OBJS) $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)Compiled!$(DEF_COLOR)"
	@echo "Launch the program with $(RED)./${NAME} file.conf"

$(OBJ_DIR)%.o : %.cpp $(HEADER) Makefile | $(OBJF)
	$(CC) $(FLAG) $(INCLUDE) -c $< -o $@


clean :
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJF)

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re

# Color
DEF_COLOR	= \033[0;39m
RED			= \033[0;91m
GREEN		= \033[0;92m
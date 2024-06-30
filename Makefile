NAME		=	webserv
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3

SRC_DIR	=	./
OBJ_DIR	=	obj/

#Source
FILES	=	main ConfigFile Location Server ParserUtils Request RequestLine URL CGI exec_rq Poll Response rq_dir rq_html server utils RequestChecking
SRCS	=	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(FILES)))
OBJS	=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

INCLUDE	=	-I ./include
HNAME	=	ConfigFile Location Server \
			CGI Exception Poll Response server \
			ParserUtils \
			Request RequestLine URL RequestChecking
HEADER	=	$(addsuffix .hpp, $(HNAME))

OBJF	=	.cache_exits

$(OBJF) :
	@mkdir -p $(OBJ_DIR)

vpath %.cpp ConfigFile/ ParserUtils/ RequestParsing/ server/ RequestChecking/
vpath %.hpp include/ ConfigFile/ ParserUtils/ RequestParsing/ RequestChecking/

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAG) $(OBJS) $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)Compiled!$(DEF_COLOR)"
	@echo "Launch the program with $(RED)./${NAME}"

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

# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++17 -pthread -lrt

LDFLAGS := -lGL -lGLEW -lglfw -lm

# VPath

FILE_PATH = $(shell pwd)/config.json

RM			= rm -f
AR			= ar rcs

# Colors
DEF_COLOR = \033[0;39m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# Directories
SRC_DIR := ./src
OBJ_DIR	:= ./obj
LIBS_DIR := libs
VPATH =			./enemies ./enemy ./include ./libs ./map ./player ./projectile ./src ./textures ./tile ./utils 
SRCS=			$(shell find  ./enemies -name "*.cpp")\
			$(shell find  ./enemy -name "*.cpp")\
			$(shell find  ./include -name "*.cpp")\
			$(shell find  ./libs -name "*.cpp")\
			$(shell find  ./map -name "*.cpp")\
			$(shell find  ./player -name "*.cpp")\
			$(shell find  ./projectile -name "*.cpp")\
			$(shell find  ./src -name "*.cpp")\
			$(shell find  ./textures -name "*.cpp")\
			$(shell find  ./tile -name "*.cpp")\
			$(shell find  ./utils -name "*.cpp")\
			
INCLUDE =		 -I ./libs -I ./enemies -I ./enemy -I ./include -I ./libs -I ./map -I ./player -I ./projectile -I ./src -I ./textures -I ./tile -I ./utils
OBJS	= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
NAME := game


all:
	@${MAKE} $(NAME) #-j

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $@ $(LDFLAGS)
	@echo "$(YELLOW)$(NAME)$(DEF_COLOR) $(CYAN)done.$(DEF_COLOR)"

data_race: $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $@  -fsanitize=thread

$(OBJ_DIR)/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

show:
	@echo $(OBJS)

clean:
			@$(RM) -rdf $(OBJ_DIR)
			@echo "$(YELLOW)$(NAME)$(DEF_COLOR) $(CYAN)successfully cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@echo "$(YELLOW)All$(DEF_COLOR) $(CYAN)objects successfully cleaned!$(DEF_COLOR)"

re:			fclean all

.PHONY: all clean fclean re

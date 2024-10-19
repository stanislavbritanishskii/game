#!/bin/bash


cat << EOF

# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++17 -pthread -lrt

LDFLAGS := -lGL -lGLEW -lglfw -lm

# VPath

FILE_PATH = \$(shell pwd)/config.json

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
EOF


echo -en VPATH ="\t\t\t"

for name in *; do
    if [ "$name" != 'tests' ] && [ "$name" != 'obj' ]&& [ "$name" != 'Commander' ] && [ -d $name ]; then
        # Print the name if the conditions are met
        echo -n ./"$name "
    fi
done
echo


echo -en "SRCS=\t\t\t"

for name in *; do
    if [ "$name" != 'tests' ] && [ "$name" != 'obj' ]&& [ "$name" != 'Commander' ] && [ -d $name ]; then
        # Print the name if the conditions are met
        echo -ne '$(shell find ' ./"$name -name \"*.cpp\")"'\\'"\n\t\t\t"
    fi
done
echo

echo -en INCLUDE ="\t\t -I ./libs "

for name in *; do
    if [ "$name" != 'tests' ] && [ "$name" != 'obj' ]&& [ "$name" != 'Commander' ] && [ -d $name ]; then
        # Print the name if the conditions are met
        echo -n "-I ./$name "
    fi
done
echo
cat << EOF
OBJS	= \$(addprefix \$(OBJ_DIR)/, \$(notdir \$(SRCS:.cpp=.o)))
EOF

echo NAME := $(basename $(pwd))

cat << EOF


all:
	@\${MAKE} \$(NAME) #-j

\$(NAME): \$(OBJ_DIR) \$(OBJS)
	@\$(CXX) \$(CXXFLAGS) \$(INCLUDE) \$(OBJS) -o \$@ \$(LDFLAGS)
	@echo "\$(YELLOW)\$(NAME)\$(DEF_COLOR) \$(CYAN)done.\$(DEF_COLOR)"

data_race: \$(OBJ_DIR) \$(OBJS)
	\$(CXX) \$(CXXFLAGS) \$(INCLUDE) \$(OBJS) -o \$@  -fsanitize=thread

\$(OBJ_DIR)/%.o: %.cpp
	@\$(CXX) \$(CXXFLAGS) \$(INCLUDE) -c \$< -o \$@

\$(OBJ_DIR):
	@mkdir -p \$(OBJ_DIR)

show:
	@echo \$(OBJS)

clean:
			@\$(RM) -rdf \$(OBJ_DIR)
			@echo "\$(YELLOW)\$(NAME)\$(DEF_COLOR) \$(CYAN)successfully cleaned!\$(DEF_COLOR)"

fclean:		clean
			@\$(RM) -f \$(NAME)
			@echo "\$(YELLOW)All\$(DEF_COLOR) \$(CYAN)objects successfully cleaned!\$(DEF_COLOR)"

re:			fclean all

.PHONY: all clean fclean re
EOF
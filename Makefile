# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/16 20:52:32 by psuanpro          #+#    #+#              #
#    Updated: 2023/10/24 17:26:40 by rchiewli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

MAIN		= main.cpp
MAIN_DIR	= ./

CC			= g++
CPPFLAGS	= -g -Wall -Werror -Wextra


SRC_DIR		= ./src/

MODULE1		= Parsing.cpp \
				Conf.cpp

MODULE2		= Server.cpp \
				Response.cpp \
				Store.cpp \
				FileHandle.cpp \
				Request.cpp
 #HttpRequestHandle.cpp

MODULE3		=

MODULE4		= Console.cpp \
				Uti.cpp


MODULE1_DIR	= ./src/parsing/

MODULE2_DIR = ./src/server/
MODULE3_DIR = ./src/cgihandler/
MODULE4_DIR = ./src/utils/

# LIB_DIR		= ./lib/
# MLX = mlx/libmlx.a
# INCLUDE_MLX = -framework OpenGL -framework AppKit
# LIBFT = libft/libft.a
# LIB =	${addprefix $(LIB_DIR),$(LIBFT)} \
# 		${addprefix $(LIB_DIR),$(MLX)}


OBJ_DIR		= ./obj/

OBJS		= ${addprefix $(OBJ_DIR),$(MODULE1:.cpp=.o)} \
			  ${addprefix $(OBJ_DIR),$(MODULE2:.cpp=.o)} \
			  ${addprefix $(OBJ_DIR),$(MODULE4:.cpp=.o)} \
			  ${addprefix $(OBJ_DIR),$(MAIN:.cpp=.o)}

RM = rm -rf


BRED =\033[1;31m
BGRN =\033[1;32m
BYEL =\033[1;33m
RES = \033[0m
BLU = \033[0;34m

COMPILE = echo "$(BGRN)$(NAME) compiled....$(RES)"
CLEAN = echo "$(BYEL)$(NAME) clean....$(RES)"
FCLEAN = echo "$(BRED)$(NAME) fclean....$(RES)"

# LOAD = bash ./src/script/animation.sh $(MODULE1) $(MODULE2) $(MODULE3) $(MODULE4) $(MODULE5);
# CUBE =  echo '$(BLU)________$(RES)/\\\\\\\\\\\\\\\\\\$(BLU)________________$(RES)/\\\\\\$(BLU)____________$(RES)/\\\\\\\\\\\\\\\\\\\\$(BLU)___$(RES)/\\\\\\\\\\\\\\\\\\\\\\\\$(BLU)____$(RES)         '; \
# 		echo ' $(BLU)_____$(RES)/\\\\\\////////$(BLU)________________$(RES)\\/\\\\\\$(BLU)__________$(RES)/\\\\\\///////\\\\\\$(BLU)_$(RES)\\/\\\\\\////////\\\\\\$(BLU)__$(RES)        '; \
# 		echo '  $(BLU)___$(RES)/\\\\\\/$(BLU)_________________________$(RES)\\/\\\\\\$(BLU)_________$(RES)\\///$(BLU)______$(RES)/\\\\\\$(BLU)__$(RES)\\/\\\\\\$(BLU)______$(RES)\\//\\\\\\$(BLU)_$(RES)       '; \
# 		echo '   $(BLU)__$(RES)/\\\\\\$(BLU)______________$(RES)/\\\\\\$(BLU)____$(RES)/\\\\\\$(BLU)_$(RES)\\/\\\\\\$(BLU)________________$(RES)/\\\\\\//$(BLU)___$(RES)\\/\\\\\\$(BLU)_______$(RES)\\/\\\\\\$(BLU)_$(RES)      '; \
# 		echo '    $(BLU)_$(RES)\\/\\\\\\$(BLU)_____________$(RES)\\/\\\\\\$(BLU)___$(RES)\\/\\\\\\$(BLU)_$(RES)\\/\\\\\\\\\\\\\\\\\\$(BLU)_________$(RES)\\////\\\\\\$(BLU)__$(RES)\\/\\\\\\$(BLU)_______$(RES)\\/\\\\\\$(BLU)_$(RES)     '; \
# 		echo '     $(BLU)_$(RES)\\//\\\\\\$(BLU)____________$(RES)\\/\\\\\\$(BLU)___$(RES)\\/\\\\\\$(BLU)_$(RES)\\/\\\\\\////\\\\\\$(BLU)___________$(RES)\\//\\\\\\$(BLU)_$(RES)\\/\\\\\\$(BLU)_______$(RES)\\/\\\\\\$(BLU)_$(RES)    '; \
# 		echo '      $(BLU)__$(RES)\\///\\\\\\$(BLU)__________$(RES)\\/\\\\\\$(BLU)___$(RES)\\/\\\\\\$(BLU)_$(RES)\\/\\\\\\$(BLU)__$(RES)\\/\\\\\\$(BLU)__$(RES)/\\\\\\$(BLU)______$(RES)/\\\\\\$(BLU)__$(RES)\\/\\\\\\$(BLU)_______$(RES)/\\\\\\$(BLU)__$(RES)   '; \
# 		echo '       $(BLU)____$(RES)\\////\\\\\\\\\\\\\\\\\\_$(RES)\\//\\\\\\\\\\\\\\\\\\$(BLU)__$(RES)\\/\\\\\\\\\\\\\\\\\\$(BLU)__$(RES)\\///\\\\\\\\\\\\\\\\\\/$(BLU)___$(RES)\\/\\\\\\\\\\\\\\\\\\\\\\\\/$(BLU)___$(RES)  '; \
# 		echo '        $(BLU)_______$(RES)\\/////////$(BLU)___$(RES)\\/////////$(BLU)___$(RES)\\/////////$(BLU)_____$(RES)\\/////////$(BLU)_____$(RES)\\////////////$(BLU)_____$(RES) ';

#parsing
${OBJ_DIR}%.o: ${MODULE1_DIR}%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

#server
${OBJ_DIR}%.o: ${MODULE2_DIR}%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

#cgihandler
${OBJ_DIR}%.o: ${MODULE3_DIR}%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

#utils
${OBJ_DIR}%.o: ${MODULE4_DIR}%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

# #free
# ${OBJ_DIR}%.o: ${MODULE3_DIR}%.c
# 	$(CC) -g $(CFLAGS) -c -o $@ $^

# #init
# ${OBJ_DIR}%.o: ${MODULE4_DIR}%.c
# 	$(CC) -g $(CFLAGS) -c -o $@ $^

# ${OBJ_DIR}%.o: ${MODULE5_DIR}%.c
# 	$(CC) -g $(CFLAGS) -c -o $@ $^

all: ${OBJ_DIR} ${NAME}



${OBJ_DIR}:
	@mkdir -p $(OBJ_DIR)

${OBJ_DIR}%.o: ${MAIN_DIR}%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

${NAME}: ${OBJS}
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME) $(LIB)

clean:
	@$(RM) $(OBJ_DIR)
	@$(RM) *.dSYM
	@$(CLEAN)

fclean: clean
	@$(RM) $(NAME) $(OBJ_DIR)
	@$(FCLEAN)


re: fclean all

run: re
	./webserv config.conf

.PHONY: all clean fclean re run

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/24 19:29:43 by mcourtoi          #+#    #+#              #
#    Updated: 2023/11/24 19:29:45 by mcourtoi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######################################
#              COMMANDS              #
######################################
CXX         =   ${shell which c++}
LINK        =   ${shell which clang++}
MKDIR       =   ${shell which mkdir} -p
RM          =   ${shell which rm} -rf
CMAKE		=	${shell which cmake}

######################################
#             EXECUTABLE             #
######################################
NAME		=	ft_irc

#######################################
#             DIRECTORIES             #
#######################################
SRC_DIR     =   src
OBJ_DIR     =   obj
PRV_DIR     =   private
LIB_DIR		=	lib

#######################################
#              LIBRARIES              #
#######################################
FT_IRC_A				=	libirc.a


######################################
#            SOURCE FILES            #
######################################
SRC         =                           \
				${addprefix server/,	\
					${addprefix class/,	\
						Server.cpp		\
					}					\
					server.cpp			\
				}						\
				${addprefix utils/,		\
					trim.cpp			\
				}						\
                main.cpp

LIB_SRC		= 	\
				${addprefix ${LIB_DIR}/, \
					${addprefix regex/, \
						regex.cpp \
					} \
					hello.cpp \
				}

######################################
#            OBJECT FILES            #
######################################
OBJ         =   ${SRC:.cpp=.o}
OBJ         :=  ${addprefix ${OBJ_DIR}/, ${OBJ}}

LIB_OBJ		=	${LIB_SRC:.cpp=.o}
LIB_OBJ		:=	${addprefix ${OBJ_DIR}/, ${LIB_OBJ}}

DEP         =   ${OBJ:.o=.d}
LIB_DEP		=	${LIB_OBJ:.o=.d}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS    =   -c
CXXFLAGS    +=  -Wall -Wextra -Werror
CXXFLAGS    +=  -MMD -MP
CXXFLAGS    +=  -Wshadow
CXXFLAGS 	+= -std=c++98
CXXFLAGS    +=  -I${PRV_DIR}

ifeq (${DEBUG}, 1)
    CXXFLAGS    +=  -g
    CXXFLAGS    +=  -DDEBUG=1
endif

#######################################
#                RULES                #
#######################################
${NAME}: ${OBJ} ${FT_IRC_A}
	${LINK} $^ ${OUTPUT_OPTION}

all: ${NAME}

-include ${DEP} ${LIB_DEP}

${OBJ_DIR}/%.o: 
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

${FT_IRC_A}: ${LIB_OBJ}
	ar rcs ${FT_IRC_A} ${LIB_OBJ}

clean:
	${RM} ${OBJ_DIR} ${NAME} ${FT_IRC_A} vgcore.*

fclean:
	${RM} ${OBJ_DIR} ${NAME} ${FT_IRC_A} vgcore.*

re: clean all

fre: fclean all

.PHONY:	all clean fclean re fre test

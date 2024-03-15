# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/14 23:15:50 by mcourtoi          #+#    #+#              #
#    Updated: 2024/03/15 03:18:56 by mcourtoi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######################################
#              COMMANDS              #
######################################
CXX         :=   ${shell which c++}
LINK        :=   ${shell which clang++}
MKDIR       :=   ${shell which mkdir} -p
RM          :=   ${shell which rm} -rf

######################################
#             EXECUTABLE             #
######################################
NAME := bot

#######################################
#             DIRECTORIES             #
#######################################
OBJ_DIR     :=  obj
SRC_DIR		:=	src
PRIV_DIR	:=	../private/
INC_DIR		:=	../include/

#######################################
#              LIBRARIES              #
#######################################

LIB := ../libircserv.a

######################################
#            SOURCE FILES            #
######################################
SRC := \
	${addsuffix .cpp, \
		main \
	${addprefix class/, \
		${addprefix exceptions/, \
			${addprefix ProblemWithConnect/, \
				core \
				methods \
			} \
			${addprefix ProblemWithSelect/, \
				core \
				methods \
			} \
		} \
		${addprefix Bot/, \
			core \
			methods \
		} \
	} \
}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS    = \
	-c \
	-Wall \
	-Wextra \
	-Werror \
	-Wshadow \
	-pedantic \
	-std=c++98 \
	-I${INC_DIR}

ifeq (${DEBUG}, 1)
	CXXFLAGS += -gdwarf-4 -DDEBUG=1
endif

######################################
#            OBJECT FILES            #
######################################
OBJ := ${SRC:.cpp=.o}
OBJ := ${addprefix ${OBJ_DIR}/, ${OBJ}}
DEP := ${addprefix ${OBJ_DIR}/, ${OBJ:.o=.d}}

#####################################
#           	RULES        		#
#####################################
.PHONY:	all clean fclean re fre

all: ${NAME}

${NAME}: ${OBJ} ${LIB}
	${LINK} $^ ${OUTPUT_OPTION}

${LIB}:
	$(MAKE) -C ${@D} ${@F}

-include ${DEP}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

clean:
	${RM} ${NAME} ${OBJ_DIR} vgcore.*

fclean:
	${RM} ${NAME} ${OBJ_DIR} ${LIB} vgcore.*

re: clean all

fre: fclean all


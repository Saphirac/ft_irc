# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/24 19:29:43 by mcourtoi          #+#    #+#              #
#    Updated: 2024/02/15 14:39:44 by mcourtoi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######################################
#              COMMANDS              #
######################################
   AR = ${shell which ar} rcs
  CXX = ${shell which c++}
 LINK = ${shell which clang++}
MKDIR =	${shell which mkdir} -p
   RM = ${shell which rm} -rf

######################################
#             EXECUTABLE             #
######################################
NAME = ft_irc

#######################################
#               LIBRARY               #
#######################################
LIB = lib${NAME}.a

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
SRC			=							\
				${addprefix server/,	\
					${addprefix class/,	\
						Server.cpp		\
						Client.cpp		\
						Command.cpp		\
						Channel.cpp		\
						IrcMessage.cpp	\
					}					\
				}						\
				${addprefix utils/,		\
					trim.cpp			\
				}						\
				main.cpp
SRC = \
	${addsuffix .cpp, \
		main \
	}

LIB_SRC = \
	${addsuffix .cpp, \
		${addprefix ${LIB_DIR}/, \
			${addprefix class/, \
				${addprefix Server/, \
					${addprefix command/, \
						away \
						die \
						error \
						info \
						invite \
						ison \
						join \
						kick \
						kill \
						list \
						mode \
						motd \
						names \
						nick \
						notice \
						oper \
						part \
						pass \
						ping \
						pong \
						privmsg \
						quit \
						restart \
						time \
						topic \
						user \
						version \
						wallops \
						whois \
					} \
					core \
				} \
				Client \
			} \
			format_reply \
		} \
	}

LIB_SRC		= 	\
				${addprefix ${LIB_DIR}/, \
					${addprefix regex/, \
						regex.cpp \
					} \
					parse_irc_message.cpp \
					hello.cpp \
				}

######################################
#            OBJECT FILES            #
######################################
    OBJ = ${addprefix ${OBJ_DIR}/,${SRC:.cpp=.o}}
    DEP = ${OBJ:.o=.d}
LIB_OBJ = ${addprefix ${OBJ_DIR}/,${LIB_SRC:.cpp=.o}}
LIB_DEP = ${LIB_OBJ:.o=.d}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS = \
	-c \
	-Wall -Wextra -Werror \
	-MMD -MP \
	-Wshadow \
	-std=c++98 \
	-ferror-limit=1 \
	-I${PRV_DIR} \
	-I${INC_DIR}

ifeq (${DEBUG}, 1)
	CXXFLAGS += -g -DDEBUG=1
endif

#######################################
#                RULES                #
#######################################
.PHONY:	all clean fclean re fre

${NAME}: ${LIB} ${OBJ}
	${LINK} $^ ${OUTPUT_OPTION}

${LIB}: ${LIB_OBJ}
	${AR} $@ $^

all: ${LIB} ${NAME}

-include ${DEP} ${LIB_DEP}

${OBJ_DIR}/%.o: 
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

clean:
	${RM} ${NAME} ${LIB} ${OBJ_DIR} vgcore.*

fclean: clean

re: clean all

fre: fclean all

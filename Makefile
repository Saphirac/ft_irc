# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/24 19:29:43 by mcourtoi          #+#    #+#              #
#    Updated: 2024/02/23 12:07:06 by mcourtoi         ###   ########.fr        #
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
SRC_DIR	=	src
OBJ_DIR	=	obj
PRV_DIR	=	private
LIB_DIR	=	lib
INC_DIR =	include

######################################
#            SOURCE FILES            #
######################################
SRC		=	\
	${addsuffix .cpp, \
		${addprefix server/,	\
			${addprefix class/,	\
				Server		\
				Client		\
				Channel		\
				IrcMessage	\
			}	\
		}	\
		${addprefix utils/,	\
			trim			\
		}	\
	main \
	}

LIB_SRC = \
	${addsuffix .cpp, \
		${addprefix ${LIB_DIR}/, \
			${addprefix class/, \
				${addprefix Client/, \
					Client \
					core \
				} \
				${addprefix Channel/, \
					Channel \
				} \
				${addprefix IrcMessage/, \
					IrcMessage \
				} \
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
						version \
						wallops \
						whois \
						cap \
						init_map_cmd \
					} \
					Server \
					core \
					epoll \
					handle_clients \
				} \
			} \
			${addprefix regex/, \
						regex \
				Client \
				Hostname \
				Nickname \
				Realname \
				UserModeMask \
				UserModes \
				Username \
			} \
			format_reply \
			parse_irc_message \
			send_message \
		} \
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
	-Wall \
	-Wextra \
	-Werror \
	-Wshadow \
	-Wno-c99-designator \
	-pedantic \
	-ferror-limit=1 \
	-std=c++98 \
	-MMD \
	-MP \
	-I${PRV_DIR} \
	-I${INC_DIR}


ifeq (${DEBUG}, 1)
	CXXFLAGS += -g -DDEBUG=1
endif

#######################################
#                RULES                #
#######################################
.PHONY:	all clean fclean re fre

${NAME}: ${OBJ} ${LIB}
	${LINK} $^ ${OUTPUT_OPTION}

${LIB}: ${LIB_OBJ}
	${AR} $@ $^

all: ${LIB} ${NAME}

-include ${DEP}
-include ${LIB_DEP}

${OBJ_DIR}/%.o: 
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

clean:
	${RM} ${NAME} ${LIB} ${OBJ_DIR} vgcore.*

fclean: clean

re: clean all

fre: fclean all

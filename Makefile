# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/24 19:29:43 by mcourtoi          #+#    #+#              #
#    Updated: 2024/03/15 07:26:01 by jodufour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######################################
#              COMMANDS              #
######################################
   AR := ${shell which ar} rcs
  CXX := ${shell which c++}
 LINK := ${shell which clang++}
MKDIR := ${shell which mkdir} -p
   RM := ${shell which rm} -rf

######################################
#             EXECUTABLE             #
######################################
NAME := ircserv

#######################################
#               LIBRARY               #
#######################################
LIB := lib${NAME}.a

#######################################
#             DIRECTORIES             #
#######################################
SRC_DIR := src
OBJ_DIR := obj
PRV_DIR := private
LIB_DIR := lib
INC_DIR := include

######################################
#            SOURCE FILES            #
######################################
SRC := \
	${addsuffix .cpp, \
		main \
	}

LIB_SRC := \
	${addsuffix .cpp, \
		${addprefix ${LIB_DIR}/, \
			${addprefix class/, \
				${addprefix Channel/, \
					core \
					methods \
				} \
				${addprefix Client/, \
					core \
					methods \
				} \
				${addprefix exception/, \
					${addprefix InvalidConversion/, \
						core \
						methods \
					} \
					${addprefix NotAFlag/, \
						core \
						methods \
					} \
					${addprefix ProblemWithAccept/, \
						core \
						methods \
					} \
					${addprefix ProblemWithBind/, \
						core \
						methods \
					} \
					${addprefix ProblemWithClose/, \
						core \
						methods \
					} \
					${addprefix ProblemWithEpollCreate1/, \
						core \
						methods \
					} \
					${addprefix ProblemWithEpollCtl/, \
						core \
						methods \
					} \
					${addprefix ProblemWithEpollWait/, \
						core \
						methods \
					} \
					${addprefix ProblemWithFcntl/, \
						core \
						methods \
					} \
					${addprefix ProblemWithListen/, \
						core \
						methods \
					} \
					${addprefix ProblemWithRecv/, \
						core \
						methods \
					} \
					${addprefix ProblemWithSend/, \
						core \
						methods \
					} \
					${addprefix ProblemWithSetSockOpt/, \
						core \
						methods \
					} \
					${addprefix ProblemWithSocket/, \
						core \
						methods \
					} \
					${addprefix ProblemWithStrftime/, \
						core \
						methods \
					} \
					${addprefix ProblemWithTime/, \
						core \
						methods \
					} \
					${addprefix UnknownReply/, \
						core \
						methods \
					} \
					${addprefix UserNotOnChannel/, \
						core \
						methods \
					} \
				} \
				${addprefix Message/, \
					core \
				} \
				${addprefix Server/, \
					${addprefix command/, \
						away \
						cap \
						invite \
						join \
						kick \
						list \
						mode \
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
						topic \
						user \
					} \
					core \
					methods \
				} \
				${addprefix specialized_string/, \
					${addprefix ChannelName/, \
						core \
						methods \
					} \
					${addprefix HostName/, \
						core \
						methods \
					} \
					${addprefix Key/, \
						core \
						methods \
					} \
					${addprefix NickName/, \
						core \
						methods \
					} \
					${addprefix Password/, \
						core \
						methods \
					} \
					${addprefix Prefix/, \
						core \
						methods \
					} \
					${addprefix RealName/, \
						core \
						methods \
					} \
					${addprefix Topic/, \
						core \
						methods \
					} \
					${addprefix UserModeMask/, \
						core \
						methods \
					} \
					${addprefix UserName/, \
						core \
						methods \
					} \
				} \
			} \
			check_main_arguments \
		} \
	}

######################################
#            OBJECT FILES            #
######################################
    OBJ := ${addprefix ${OBJ_DIR}/,${SRC:.cpp=.o}}
    DEP := ${OBJ:.o=.d}
LIB_OBJ := ${addprefix ${OBJ_DIR}/,${LIB_SRC:.cpp=.o}}
LIB_DEP := ${LIB_OBJ:.o=.d}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS = \
	-c \
	-Wall \
	-Wextra \
	-Werror \
	-Wshadow \
	-pedantic \
	-std=c++98 \
	-ferror-limit=1 \
	-MMD \
	-MP \
	-I${PRV_DIR} \
	-I${INC_DIR}

ifeq (${DEBUG}, 1)
	CXXFLAGS += -gdwarf-4 -DDEBUG
endif

#######################################
#                RULES                #
#######################################
.PHONY:	all clean fclean re fre

all: ${LIB} ${NAME}

${NAME}: ${OBJ} ${LIB}
	${LINK} $^ ${OUTPUT_OPTION}

${LIB}: ${LIB_OBJ}
	${AR} $@ $^

-include ${DEP}
-include ${LIB_DEP}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

clean:
	${RM} ${NAME} ${LIB} ${OBJ_DIR} vgcore.*

fclean: clean

re: clean all

fre: fclean all

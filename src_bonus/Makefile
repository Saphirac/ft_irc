######################################
#              COMMANDS              #
######################################
CXX         :=   ${shell which c++}
LINK        :=   ${shell which clang++}
MKDIR       :=   ${shell which mkdir} -p
RM          :=   ${shell which rm} -rf
CMAKE		:=	${shell which cmake}

######################################
#             EXECUTABLE             #
######################################
NAME := bot

#######################################
#             DIRECTORIES             #
#######################################
OBJ_DIR     :=   obj
SRC_DIR		:=	.
PRIV_DIR	:=	../private/
INC_DIR		:=	../include/

#######################################
#              LIBRARIES              #
#######################################

IRC_A := ../libircserv.a

######################################
#            SOURCE FILES            #
######################################
SRC := \
	${addsuffix .cpp, \
	 \
		${addprefix Bot/, \
			core \
			methods \
		} \
		main \
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
	-std=c++14 \
	-I${PRIV_DIR} \
	-I${INC_DIR}

ifeq (${DEBUG}, 1)
	CXXFLAGS += -gdwarf-4 -DDEBUG=1
endif

######################################
#            OBJECT FILES            #
######################################
OBJ := ${SRC:.cpp=.o}
OBJ := ${addprefix ${OBJ_DIR}/, ${OBJ}}

#####################################
#           	RULES        		#
#####################################
all: ${NAME}

${NAME}: ${OBJ} ${IRC_A}
	${LINK} $^ ${LFLAGS} ${OUTPUT_OPTION}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp ${IRC_A}
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

${IRC_A} :
	${MAKE} -C ${@D} ${@F}

clean:
	${RM} ${NAME} ${IRC_A} vgcore.*

fclean:
	${RM} ${OBJ_DIR} ${NAME} ${IRC_A} vgcore.*

re: clean all

fre: fclean all

.PHONY:	all clean fclean re fre

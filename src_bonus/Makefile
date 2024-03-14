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
NAME = bot

#######################################
#             DIRECTORIES             #
#######################################
OBJ_DIR     =   obj
SRC_DIR		=	.
PRIV_DIR	=	../private/
INC_DIR		=	../include/

#######################################
#              LIBRARIES              #
#######################################

IRC_A = ../libircserv.a

######################################
#            SOURCE FILES            #
######################################
SRC = \
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
CXXFLAGS    =   -c
CXXFLAGS    +=  -Wall -Wextra -Werror
CXXFLAGS    +=	-MMD -MP
CXXFLAGS    +=	-Wshadow
CXXFLAGS 	+=	-std=c++14
CXXFLAGS	+=	-I${PRIV_DIR}
CXXFLAGS	+=	-I${INC_DIR}

LFLAGS 		+=	-lpthread

ifeq (${DEBUG}, 1)
    CXXFLAGS    +=  -g
    CXXFLAGS    +=  -DDEBUG=1
endif

######################################
#            OBJECT FILES            #
######################################
OBJ = ${SRC:.cpp=.o}
OBJ := ${addprefix ${OBJ_DIR}/, ${OBJ}}

TEST_DEP = ${OBJ:.o=.d}

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

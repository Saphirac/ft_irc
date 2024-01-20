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
NAME        =   ft_irc

#######################################
#             DIRECTORIES             #
#######################################
SRC_DIR     =   src
OBJ_DIR     =   obj
PRV_DIR     =   private

######################################
#            SOURCE FILES            #
######################################
SRC         =                           \
                main.cpp

######################################
#            OBJECT FILES            #
######################################
OBJ         =   ${SRC:.cpp=.o}
OBJ         :=  ${addprefix ${OBJ_DIR}/, ${OBJ}}

DEP         =   ${OBJ:.o=.d}

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
${NAME}: ${OBJ}
	${LINK} $^ ${OUTPUT_OPTION}

all: ${NAME}

-include ${DEP}

${OBJ_DIR}/%.o: 
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} ${OUTPUT_OPTION}

clean:
	${RM} ${OBJ_DIR} ${NAME} vgcore.*

fclean:
	${RM} ${OBJ_DIR} ${NAME} vgcore.*

re: clean all

fre: fclean all

.PHONY:	all clean fclean re fre test

######################################
#              COMMANDS              #
######################################
CXX         =   c++
LINK        =   clang++
MKDIR       =   mkdir -p
RM          =   rm -rf

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
GTEST_DIR   =   googletest/googletest

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
CXXFLAGS    +=  -std=c++14
CXXFLAGS    +=  -I${PRV_DIR}
CXXFLAGS    +=  -I${GTEST_DIR}/include  # Ajout du chemin d'inclusion pour Google Test

ifeq (${DEBUG}, 1)
    CXXFLAGS    +=  -g
    CXXFLAGS    +=  -DDEBUG=1
endif

#######################################
#           GOOGLE TEST SETUP         #
#######################################
# Drapeaux et inclusions pour Google Test
GTEST_LIB = -Lgoogletest/build/lib -lgtest -lgtest_main -lpthread

# Fichiers de test source
TEST_SRC = tests/test1.cpp tests/test2.cpp # Ajoutez vos fichiers de test ici

# Fichiers objet pour les tests
TEST_OBJ = ${TEST_SRC:.cpp=.o}
TEST_OBJ := ${addprefix ${OBJ_DIR}/, ${TEST_OBJ}}

# Exécutable de test
TEST_NAME = unit_tests

# Règle pour compiler les tests
$(TEST_NAME): $(TEST_OBJ)
	$(LINK) $^ $(GTEST_LIB) -o $@ 

# Dépendances pour les tests
TEST_DEP = ${TEST_OBJ:.o=.d}

# Règle pour les fichiers objet de test
${OBJ_DIR}/%.o: %.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} -o $@

# Inclure les dépendances
-include $(TEST_DEP)

# Règle pour exécuter les tests
test: $(TEST_NAME)
	./$(TEST_NAME)

#######################################
#                RULES                #
#######################################
${NAME}: ${OBJ}
	${LINK} $^ -o $@

all: ${NAME}

-include ${DEP}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@${MKDIR} ${@D}
	${CXX} $< ${CXXFLAGS} -o $@

clean:
	${RM} ${OBJ_DIR} ${NAME} vgcore.*

fclean:
	${RM} ${OBJ_DIR} ${NAME} vgcore.*

re: clean all

fre: fclean all

.PHONY:	all clean fclean re fre test

#include <gtest/gtest.h>


int mainProgram(); // Prototype modifié pour que mainProgram ne retourne rien

TEST(MainProgramTest, OutputsHelloWorld) {
    // Sauvegarder l'ancien buffer
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();

    // Créer un stringstream pour capturer la sortie
    std::ostringstream strCout;
    std::cout.rdbuf(strCout.rdbuf());

    // Appeler la fonction qui imprime sur la sortie standard
    mainProgram();

    // Restaurer l'ancien buffer
    std::cout.rdbuf(oldCoutStreamBuf);

    // Vérifier la sortie capturée
    EXPECT_EQ(strCout.str(), "hello world\n");
}

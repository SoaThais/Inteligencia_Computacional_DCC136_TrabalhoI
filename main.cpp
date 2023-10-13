#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <chrono>

#include "Grafo.hpp"

using relogio_t = std::chrono::high_resolution_clock;
using tempo_t = relogio_t::time_point;

#define EXPECTED_ARGC(argc) ((argc) > 4 || (argc) < 3)

#define ERR_INVALID_ARGC        1
#define ERR_COULD_NOT_OPEN_FILE 2
#define ERR_READ_FAIL           3

void cleanName(std::string& instanceName) {
    size_t it = instanceName.find("Instancias");
    instanceName.erase(0, it + 11);
    it = instanceName.find(".ophs");
    instanceName.erase(it, 5);
}

int main(int argc, char **argv)
{
    if (!EXPECTED_ARGC(argc)) {
        std::cerr << "Uso: " << argv[0]
            << " ARQUIVO_ENTRADA <SEED>\n";
        return ERR_INVALID_ARGC;
    }

    std::ifstream arqEntrada;
    std::ofstream arqSaida;

    arqEntrada.open(argv[1]);
    //arqSaida.open(argv[2]);

    if (!arqEntrada.is_open()) {
        std::cerr << argv[0]
            << ": não foi possível abrir o arquivo `"
            << argv[1] << "`\n";
        return ERR_COULD_NOT_OPEN_FILE;
    }

    std::string instanceName = argv[1];
    cleanName(instanceName);

    Grafo g = Grafo::lerArquivo(arqEntrada, instanceName);

    arqEntrada.close();

    long seed = time(NULL)*time(NULL)/rand();

    if (argc == 3) {
        seed = std::stol(argv[2]);
    }

    //g.imprimeGrafo();

    srand(seed);

    tempo_t start = relogio_t::now();

    g.geraSolucao();

    tempo_t end = relogio_t::now();

    //printa custo u.cost;
    double time = std::chrono::duration_cast<std::chrono::microseconds>((end)-(start)).count();
    std::cout << "Tempo Gasto: " << time << std::endl;

}

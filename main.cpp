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

#define EXPECTED_ARGC(argc) ((argc) == 5 || (argc) == 6)

#define ERR_INVALID_ARGC        1
#define ERR_COULD_NOT_OPEN_FILE 2
#define ERR_READ_FAIL           3

struct InfoSolucao {
    double execucao;
    double custo;
    double score;
    double qualidade;
    double tempoGasto;
};


void cleanName(std::string& instanceName) {
    size_t it = instanceName.find("Instancias");
    instanceName.erase(0, it + 11);
    it = instanceName.find(".ophs");
    instanceName.erase(it, 5);
}

void printTitle(const std::string& customText) {
    std::cout << std::endl;
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << customText << std::endl;
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char **argv)
{

    /*--------------------------------*/
    /*------LEITURA-DOS-ARQUIVOS------*/
    /*--------------------------------*/

    if (!EXPECTED_ARGC(argc)) {
        std::cerr << "Uso: " << argv[0]
            << " ARQUIVO_ENTRADA ALGORITMO MAX_IT MAX_EXEC <SEED>\n";
        return ERR_INVALID_ARGC;
    }

    std::ifstream arqEntrada;
    std::ofstream arqSaida;

    arqEntrada.open(argv[1]);
    if (!arqEntrada.is_open()) {
        std::cerr << argv[0]
            << ": não foi possível abrir o arquivo `"
            << argv[1] << "`\n";
        return ERR_COULD_NOT_OPEN_FILE;
    }
    std::string instanceName = argv[1];
    cleanName(instanceName);

    /*--------------------------------*/
    /*-----------CRIA GRAFO-----------*/
    /*--------------------------------*/

    Grafo g = Grafo::lerArquivo(arqEntrada, instanceName);
    arqEntrada.close();

    std::vector<InfoSolucao> solucoesDeG;

    int algorithmID = std::stol(argv[2]);
    size_t max_it = std::stol(argv[3]);
    size_t max_exec = std::stol(argv[4]);

    for(size_t numExec = 1; numExec <= max_exec; numExec++){

        /*--------------------------------*/
        /*--------SEED-DE-RANDOM----------*/
        /*--------------------------------*/

        long seed = time(NULL)*time(NULL)/rand();
        if (argc == 5) {
            seed = std::stol(argv[4]);
        }
        srand(seed);

        /*--------------------------------*/
        /*--------GERANDO-SOLUÇÃO---------*/
        /*--------------------------------*/

        tempo_t start = relogio_t::now();
        g.geraSolucao(algorithmID, max_it);
        tempo_t end = relogio_t::now();

        std::cout << "Custo da Solução: " << g.getCustoSol() << std::endl;
        std::cout << "Score da Solução: " << g.getScoreSol() << std::endl;
        double time = std::chrono::duration_cast<std::chrono::microseconds>((end)-(start)).count();
        std::cout << "Tempo Gasto: " << time << std::endl;


        InfoSolucao info;
        info.execucao = numExec;
        info.custo = g.getCustoSol();
        info.score = g.getScoreSol();
        info.qualidade = info.score - info.custo;
        info.tempoGasto = time;
        solucoesDeG.push_back(info);
    }

    printTitle("RESULTADOS DAS EXECUÇÕES");
    for (const InfoSolucao& solucao : solucoesDeG) {
        std::cout << "Número da Execução: " << solucao.execucao << std::endl;
        std::cout << "Custo da Solução: " << solucao.custo << std::endl;
        std::cout << "Score da Solução: " << solucao.score << std::endl;
        std::cout << "Qualidade da Solução: " << solucao.qualidade << std::endl;
        std::cout << "Tempo Gasto: " << solucao.tempoGasto << std::endl;
    }
}
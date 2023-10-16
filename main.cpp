#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstdio>

#include "Grafo.hpp"

using relogio_t = std::chrono::high_resolution_clock;
using tempo_t = relogio_t::time_point;

#define EXPECTED_ARGC(argc) ((argc) == 6 || (argc) == 7)

#define ERR_INVALID_ARGC        1
#define ERR_COULD_NOT_OPEN_FILE 2
#define ERR_READ_FAIL           3

struct InfoSolucao {
    double execucao;
    double custo;
    double score;
    double qualidade;
    double tempoGasto;
    listavertices_t vertices;
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
            << " ARQUIVO_ENTRADA ARQUIVO_SAIDA ALGORITMO MAX_IT MAX_EXEC <SEED>\n";
        return ERR_INVALID_ARGC;
    }

    std::ifstream arqEntrada;

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

    int algorithmID = std::stol(argv[3]);
    size_t max_it = std::stol(argv[4]);
    size_t max_exec = std::stol(argv[5]);

    for(size_t numExec = 1; numExec <= max_exec; numExec++){

        /*--------------------------------*/
        /*--------SEED-DE-RANDOM----------*/
        /*--------------------------------*/

        long seed = time(NULL)*time(NULL)/rand();
        if (argc == 5) {
            seed = std::stol(argv[6]);
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
        listavertices_t listaSolucoes = g.getListaSol();
        for (const Vertice& vertex : listaSolucoes) {
            info.vertices.push_back(vertex);
        }
        solucoesDeG.push_back(info);
    }

    std::ofstream arqSaida;

    arqSaida.open(argv[2]);
    if (!arqSaida.is_open()) {
        std::cerr << argv[0]
            << ": não foi possível abrir o arquivo `"
            << argv[2] << "`\n";
        return ERR_COULD_NOT_OPEN_FILE;
    }

    printTitle("RESULTADOS DAS EXECUÇÕES");
    for (const InfoSolucao& solucao : solucoesDeG) {
        arqSaida << "Número da Execução: " << solucao.execucao << std::endl;
        arqSaida << "Custo da Solução: " << solucao.custo << std::endl;
        arqSaida << "Score da Solução: " << solucao.score << std::endl;
        arqSaida << "Qualidade da Solução: " << solucao.qualidade << std::endl;
        arqSaida << "Tempo Gasto: " << solucao.tempoGasto << std::endl;
        arqSaida << "Vertices da Solução: ";
        for (const Vertice& vertex : solucao.vertices) {
            if(vertex.id()!=0){arqSaida << "-";}
            if(vertex.isHotel()){ arqSaida << "H"; }
            arqSaida << vertex.id();
        }
        arqSaida << std::endl;
        arqSaida << "Coordenadas dos Vertices da Solução: ";
        for (const Vertice& vertex : solucao.vertices) {
            arqSaida << "(" << vertex.x() << "," << vertex.y() << ")";
        }
        arqSaida << std::endl;
        arqSaida << "__________________________________" << std::endl;

    }
    arqSaida.close();

    std::string oldName = argv[2]; // nome antigo do arquivo
    std::string newName = instanceName + "-output.txt";
    if (std::rename(oldName.c_str(), newName.c_str()) == 0) { // renomeia o arquivo
        std::cout << "Arquivo renomeado com sucesso." << std::endl;
    }

    arqSaida.close();
}
#include "Grafo.hpp"
#include <iomanip>

Grafo::Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax):
    _graphName(graphName),
    _numeroHoteis(numeroDeHoteis),
    _numeroTrips(numeroDeTrips),
    _tMax(tMax)
{
    this->listaVertices.reserve(numeroDeVertices);
    this->listaTamanhoTrips.reserve(numeroDeTrips);

}

void Grafo::setaTamTrips(std::vector <float> tamTrips) {
    this->listaTamanhoTrips = tamTrips;
}

void Grafo::setaMatrizDists() {
    this->matrizDist = calculaMatrizDistancias();
}

Vertice& Grafo::getVerticeById(size_t id) {
    return this->listaVertices.at(id - 1);
}

double Grafo::distanciaEuclidiana(Vertice a, Vertice b)
{
    return sqrt(pow((a.x() - b.x()), 2) + pow((a.y() - b.y()), 2));
}

double** Grafo::calculaMatrizDistancias() {
    double **matrizDistancias = new double *[numeroDeVertices()];
    for (size_t i = 0; i < numeroDeVertices(); i++) {
        matrizDistancias[i] = new double[numeroDeVertices()];
        for (size_t j = 0; j < numeroDeVertices(); j++) {
            matrizDistancias[i][j] = distanciaEuclidiana(this->getVerticeById(i + 1), this->getVerticeById(j + 1));
            //std::cerr << std::setprecision(2) << matrizDistancias[i][j] << " | ";
        }
        //std::cerr << std::endl;
    }
    return matrizDistancias;
}

std::vector <float> separaLinha(std::string linha){
    std::istringstream stream(linha);
    std::string palavra;
    std::vector <float> numeros;

    while (stream >> palavra) {
        char* end;
        float numero = strtof(palavra.c_str(), &end);

        if (*end == '\0') {
            numeros.push_back(numero);
        } else {
            std::cerr << "Erro ao converter a string em float: " << palavra << std::endl;
        }
    }

    return numeros;
}

Grafo Grafo::lerArquivo(std::istream& arqEntrada, std::string nomeArquivo)
{
    std::string graphName = nomeArquivo;
    size_t nVertices;
    size_t nHoteis;
    size_t nTrips;
    size_t tMax;
    std::vector <float> tamTrips;

    std::string linha;
    std::vector <float> numeros;
    int i = -1;

    do {
        std::getline(arqEntrada, linha);
        if (linha == "---------------------------------------------------") {
            continue;
        }

        i += 1;
        numeros = separaLinha(linha);

        if (i == 0){
            nVertices = numeros.at(0);
            nHoteis = numeros.at(1);
            nTrips = numeros.at(2);
            continue;
        }
        if (i == 1){
            tMax = numeros.at(0);
            continue;
        }
        if (i == 2){
            tamTrips = numeros;
            continue;
        }
        if (i == 3){
            break;
        }
    } while (true);

    Grafo g(graphName, nVertices, nHoteis, nTrips, tMax);

    g.setaTamTrips(tamTrips);

    for (size_t i = 0; i < nVertices; i++) {
        std::string str_linha;

        //id = 0, Hotel Inicial
        //id = 1, Hotel Final

        size_t id = i;
        double x, y, score;
        bool isHotel = false;

        getline(arqEntrada, str_linha);
        sscanf(str_linha.c_str(), "%lf%lf%lf", &x, &y, &score);

        if(i < 2+nHoteis){
           isHotel = true;
        }
        g.listaVertices.push_back(Vertice(id, x, y, score, isHotel));
    }

    g.setaMatrizDists();

    return g;
}


listavertices_t Grafo::selecionaHoteisCandidatos(std::vector<Vertice> hoteis, int nTrips) {

    listavertices_t resultado;

    // Adiciona H_i
    resultado.push_back(hoteis[0]);

    // Seleciona nTrips-1 hotéis aleatórios entre H_i e H_f
    for (int i = 0; i < nTrips-1; i++) {

        //Pode repetir os hoteis
        int j = 1 + rand() % (hoteis.size() - 1);
        do
        {
            j = 1 + rand() % (hoteis.size() - 1);
        } while (j == 1);
        resultado.push_back(hoteis[j]);
    }

    // Adiciona H_f
    resultado.push_back(hoteis[1]);

    return resultado;
}

listavertices_t Grafo::guloso(){

    //FASE CONSTRUTIVA

    //gera lista de candidatos com todos os hoteis
    listavertices_t todosHoteisCandidatos(this->listaVertices.begin(), this->listaVertices.begin() + 2 + numeroDeHoteis());
    imprimeListaVertices(todosHoteisCandidatos);

    //seleciona [ntrips-1] candidatos entre h0 e hf para a lista
    listavertices_t listaCandidatos = selecionaHoteisCandidatos(todosHoteisCandidatos, numeroDeTrips());

    std::cout << "Lista de candidatos:" << std::endl;
    imprimeListaVertices(listaCandidatos);
    //resultado deve ser (h0 - hx - hx - hf)

    return listaCandidatos;
    //FASE INSERÇÃO

}

void Grafo::geraSolucao(){
    listavertices_t solucao = guloso();
}

void Grafo::imprimeListaVertices(listavertices_t verticesToPrint){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < verticesToPrint.size(); i++) {
        if(verticesToPrint[i].isHotel())
            { std::cout << "H" << i << " | ";}
        else{ std::cout << "V" << i-numeroDeHoteis()-2 << " | ";}

        std::cout << verticesToPrint[i].toString() << std::endl;
    }
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

void Grafo::imprimeGrafoHelper(){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << "Lista de Vertices: " << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < numeroDeVertices(); i++) {
        if(i == 0){ std::cout << "H_i" << " | ";}
        else if(i == 1){ std::cout << "H_f" << " | ";}
        else if(i < 2 + numeroDeHoteis())
            { std::cout << "H" << i - 2 << " | ";}
        else{ std::cout << "V" << i - 2 - numeroDeHoteis() << " | ";}

        std::cout << this->listaVertices[i].toString() << std::endl;
    }
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

void Grafo::imprimeGrafo(){

    std::cout << std::endl;
    std::cout << "Instancia: " << graphName() << std::endl;
    std::cout << std::endl;
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << "N: " << numeroDeVertices() << std::endl;
    std::cout << "H: " << numeroDeHoteis() << std::endl;
    std::cout << "D: " << numeroDeTrips() << std::endl;
    std::cout << "T_Max: " << this->_tMax << std::endl;

    std::cout << "T_d: ";

    for (size_t i = 0; i < numeroDeTrips(); i++) {
        std::cout << this->listaTamanhoTrips[i] << " | ";
    }
    std::cout << std::endl;

    imprimeGrafoHelper();

}

struct InfoSolucao {
    double distanciaTotal;
    double tempoViajando;
};
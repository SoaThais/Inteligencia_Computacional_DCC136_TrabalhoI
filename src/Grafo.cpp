#include "Grafo.hpp"
#include <iomanip>
#include <algorithm>


#define ERR_EMPTY_CANDIDATES             1
#define ERR_VERTEX_NON_EXIST             2
#define ERR_BIG_DISTANCE                 3
#define ERR_FULL_TRIPS                   4

Grafo::Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax):
    _graphName(graphName),
    _numeroHoteis(numeroDeHoteis),
    _numeroTrips(numeroDeTrips),
    _tMax(tMax)
{
    this->listaVertices.reserve(numeroDeVertices);
    this->listaVariancias.reserve(numeroDeVertices);
    this->listaTamanhoTrips.reserve(numeroDeTrips);
    this->listaCheckTrips.reserve(numeroDeTrips);
    this->listaTamanhoMaxTrips.reserve(numeroDeTrips);
}

/*--------------------------------*/
/*-----------Sets/Gets-----------*/
/*--------------------------------*/

void Grafo::setaTamMaxTrips(listatour_t tamTrips) {
    this->listaTamanhoMaxTrips = tamTrips;
}
void Grafo::setaTamTrips(listatour_t tamTrips) {
    this->listaTamanhoTrips = tamTrips;
}
void Grafo::setaCheckTrips(listatour_t tamTrips) {
    this->listaCheckTrips = tamTrips;
}

void Grafo::setaVariancias(listavariancias_t variancias) {
    this->listaVariancias = variancias;
}

void Grafo::setaMatrizDists() {
    this->matrizDist = calculaMatrizDistancias();
}

Vertice& Grafo::getVerticeById(size_t id) {
    return this->listaVertices.at(id);
}

int Grafo::getVerticeIndex(listavertices_t verticeVector, Vertice v)
{
    int index = -1;
    auto it = find(verticeVector.begin(), verticeVector.end(), v);
    if (it != verticeVector.end())
    {
        index = std::distance(verticeVector.begin(), it);
        std::cerr << "O elemento " << v.id() << " foi encontrado no index " << index << std::endl;

    }
    else {
        std::cerr << "O elemento " << v.id() << " não foi encontrado " << std::endl;
    }
    return index;
}

void Grafo::atualizaTamanhoTripT(double novoCusto, size_t t){
    this->listaTamanhoTrips[t-1] += novoCusto;
}

double Grafo::refazCalculoAresta(listaids_t verticesRefatorados){
    //0 - entre0 | 1 - entre1 | 2 - verticeNovo
    double refatorado = 0;
    double custoOriginalAresta = matrizDist[verticesRefatorados[0]][verticesRefatorados[1]];
    double custoArestasNovas = matrizDist[verticesRefatorados[0]][verticesRefatorados[2]] + matrizDist[verticesRefatorados[2]][verticesRefatorados[1]];
    std::cout << "CustoArestasNovas: " << matrizDist[verticesRefatorados[0]][verticesRefatorados[2]] << " | " << matrizDist[verticesRefatorados[2]][verticesRefatorados[1]]<< std::endl;
    std::cout << "custoOriginalAresta: " << custoOriginalAresta << std::endl;
    refatorado = custoArestasNovas - custoOriginalAresta;
    return refatorado;
}

bool Grafo::insercaoViavel(size_t i, size_t j, size_t t){
    return this->listaTamanhoTrips[t - 1] + matrizDist[i][j] <= this->listaTamanhoMaxTrips[t - 1];
}


/*--------------------------------*/
/*-----------Auxiliares-----------*/
/*--------------------------------*/


double Grafo::distanciaEuclidiana(Vertice a, Vertice b)
{
    return sqrt(pow((a.x() - b.x()), 2) + pow((a.y() - b.y()), 2));
}

double Grafo::calculaVariancia(double media, double *distancias)
{
    double variancia = 0;
    for(size_t k = 0; k < numeroDeVertices(); k++){
        variancia += pow((distancias[k]-media), 2);
    }
    variancia = variancia/numeroDeVertices();
    return variancia;
}

double** Grafo::calculaMatrizDistancias() {

    double **matrizDistancias = new double *[numeroDeVertices()];
    listavariancias_t variancias;

    for (size_t i = 0; i < numeroDeVertices(); i++) {
        matrizDistancias[i] = new double[numeroDeVertices()];
        double mediaAritmetica = 0;

        for (size_t j = 0; j < numeroDeVertices(); j++) {
            matrizDistancias[i][j] = distanciaEuclidiana(this->getVerticeById(i), this->getVerticeById(j));
            mediaAritmetica += matrizDistancias[i][j];
        }
        mediaAritmetica = mediaAritmetica/numeroDeVertices();
        variancias.push_back(calculaVariancia(mediaAritmetica, matrizDistancias[i]));
    }
    setaVariancias(variancias);
    return matrizDistancias;
}

Vertice Grafo::maiorScore(listavertices_t vertices) {
  Vertice maior = vertices[0];
  for (size_t i = 1; i < vertices.size(); i++) {
    if (vertices[i].score() > maior.score()) {
      maior = vertices[i];
    }
  }
  return maior;
}

size_t Grafo::particionamento(listavertices_t& listaOrdenada, size_t p, size_t q, size_t idOrigem)
{
    size_t i = p - 1, j = q;
    double v = matrizDist[idOrigem][listaOrdenada[q].id()];

    while(1)
    {
        while (++i < listaOrdenada.size() && matrizDist[idOrigem][listaOrdenada[i].id()] < v)
        {
            if (idOrigem == listaOrdenada[i].id())
                continue;
        }
        while (v < matrizDist[idOrigem][listaOrdenada[--j].id()])
        {
            if (j == p)
                break;
        }
        if (i >= j)
            break;
        std::swap(listaOrdenada[i], listaOrdenada[j]);
    }
    std::swap(listaOrdenada[i], listaOrdenada[q]);
    return i;
}
void Grafo::auxQuickSort(listavertices_t& listaOrdenada, size_t p, size_t q, size_t idOrigem)
{
    if (p < q)
    {
        size_t j = particionamento(listaOrdenada, p, q, idOrigem);
        if(j != 0)
            auxQuickSort(listaOrdenada, p, j - 1, idOrigem);
        auxQuickSort(listaOrdenada, j + 1, q, idOrigem);
    }
}
listavertices_t Grafo::quickSort(size_t idOrigem, listavertices_t clientesCandidatos)
{
    listavertices_t listaOrdenada = clientesCandidatos;
    auxQuickSort(listaOrdenada, 0, clientesCandidatos.size()-1, idOrigem);

    return listaOrdenada;
}


/*--------------------------------*/
/*-----------Construtivo----------*/
/*--------------------------------*/


listavertices_t Grafo::selecionaHoteisCandidatos(listavertices_t hoteis, int nTrips) {

    listavertices_t resultado;
    listatour_t distanciasResultado;

    resultado.push_back(hoteis[0]);

    for (int i = 0; i < nTrips-1; i++) {
        int j = 1 + rand() % (hoteis.size() - 1);
        do
        {
            j = 1 + rand() % (hoteis.size() - 1);
        } while (j == 1);
        resultado.push_back(hoteis[j]);
    }
    resultado.push_back(hoteis[1]);

    for (int k = 0; k <= nTrips-1; k++){
        distanciasResultado.push_back(matrizDist[resultado[k].id()][resultado[k+1].id()]);
        std::cout << distanciasResultado[k] << std::endl;
    }
    setaTamTrips(distanciasResultado);

    return resultado;
}

Vertice Grafo::selecionaClienteIdeal(listaids_t insereEntre, listavertices_t clientesCandidatos, size_t t){

    if (clientesCandidatos.size() == 1){
        if (insercaoViavel(clientesCandidatos[0].id(), insereEntre[0], t))
            return clientesCandidatos[0];
    }

    Vertice v(0, 0, 0, -1, false);

    listavertices_t possiveis0 = quickSort(insereEntre[0], clientesCandidatos);
    listavertices_t possiveis1 = quickSort(insereEntre[1], clientesCandidatos);
    double menorDist0 = matrizDist[insereEntre[0]][possiveis0[0].id()];
    double menorDist1 = matrizDist[insereEntre[1]][possiveis1[0].id()];
    double minimo = 1;
    size_t limitador = 5;

    /*--------------------------------*/
    /*-------CRITÉRIO SIMPLES---------*/
    /*--------------------------------*/

    if(menorDist0 <= minimo){
        if (insercaoViavel(possiveis0[0].id(), insereEntre[0], t))
            v = possiveis0[0];
    }
    else if(menorDist1 <= minimo){
        if (insercaoViavel(possiveis1[0].id(), insereEntre[1], t))
            v = possiveis1[0];
    }
    else{

        /*--------------------------------*/
        /*------RANK POR DISTÂNCIA--------*/
        /*--------------------------------*/

        size_t i = 0;
        listavertices_t tolerancia0;
        listavariancias_t distancias0;
        do
        {
            if (!insercaoViavel(insereEntre[0], possiveis0[i].id(), t))
                break;

            distancias0.push_back(matrizDist[insereEntre[0]][possiveis0[i].id()]);
            tolerancia0.push_back(possiveis0[i]);
            i = i + 1;

        } while (i < possiveis0.size() && i <= limitador && distancias0[i] < menorDist0 + listaVariancias[insereEntre[0]]);

        size_t j = 0;
        listavertices_t tolerancia1;
        listavariancias_t distancias1;
        do
        {
            if (!insercaoViavel(insereEntre[1], possiveis1[j].id(), t))
                break;

            distancias1.push_back(matrizDist[insereEntre[1]][possiveis1[j].id()]);
            tolerancia1.push_back(possiveis1[j]);
            j = j + 1;
        } while (j < possiveis1.size()  && j <= limitador && distancias1[j] < menorDist1 + listaVariancias[insereEntre[1]]);


        /*--------------------------------*/
        /*---------RANK POR SCORE---------*/
        /*--------------------------------*/

        listavertices_t toleranciaCombo;
        for(size_t k = 0; k < limitador; k++){
            if(k < i && k < j){
                if(distancias0[k] <= distancias1[k])
                    toleranciaCombo.push_back(tolerancia0[k]);
                else
                    toleranciaCombo.push_back(tolerancia1[k]);
            }
            else if(k < i && k >= j){
                toleranciaCombo.push_back(tolerancia0[k]);
            }
            else if(k >= i && k < j){
                toleranciaCombo.push_back(tolerancia1[k]);
            }
        }

        if (!toleranciaCombo.empty())
            v = maiorScore(toleranciaCombo);
    }

    return v;
}

listavertices_t Grafo::insereClientes(listavertices_t listaCandidatos, listavertices_t clientesCandidatos) {

    int breakFlag = 0;
    int k = 0;
    do
    {
        size_t tripAtual = numeroDeTrips();
        listavertices_t verticesTrip;
        for (int i = listaCandidatos.size() - 1; i > 0; i--) {

            bool insercaoProibida = false;

            if(clientesCandidatos.empty()){
                breakFlag = ERR_EMPTY_CANDIDATES;
                break;
            }

            size_t cont = 0;
            for(size_t l = 0; l < this->listaCheckTrips.size(); l++){
                 if (this->listaCheckTrips[l] == 1){
                    cont += 1;
                }
            }

            if (cont == numeroDeTrips()){
                breakFlag = ERR_FULL_TRIPS;
                break;
            }

            /*--------------------------------*/
            /*-------SELEÇÃO VÉRTICE V--------*/
            /*--------------------------------*/

            listaids_t insereEntre;
            insereEntre.push_back(listaCandidatos.at(i).id());
            insereEntre.push_back(listaCandidatos.at(i-1).id());
            Vertice v = selecionaClienteIdeal(insereEntre, clientesCandidatos, tripAtual);

            if(v.id() > numeroDeVertices()){
                breakFlag = ERR_VERTEX_NON_EXIST;
                break;
            }

            std::cout << std::endl;
            std::cout << "______________PREPARA_PARA_INSERIR_______________" << std::endl;
            std::cout << std::endl;
            std::cout << "Vai inserir entre: " << insereEntre[0] << " e " << insereEntre[1] << " o vertice " << v.id() << std::endl;


            /*--------------------------------*/
            /*------TRATAMENTO INSERÇÃO-------*/
            /*--------------------------------*/

            if (v.id() == 0 && !v.isHotel())
                insercaoProibida = true;

            if (getVerticeById(insereEntre[0]).isHotel()){
                if(!verticesTrip.empty()) {
                    std::cout << std::endl;
                    std::cout << "______________VERTICES_DE_UMA_TRIP_" << tripAtual << "_______________" << std::endl;
                    std::cout << std::endl;
                    imprimeListaVertices(verticesTrip);
                    if (insercaoProibida)
                        this->listaCheckTrips[tripAtual-1] = 1;
                    tripAtual--;
                    verticesTrip.clear();
                }
                verticesTrip.push_back(getVerticeById(insereEntre[0]));
            }

            if(!insercaoProibida){
                verticesTrip.push_back(v);
                listaids_t verticesRefatorados;
                verticesRefatorados.push_back(insereEntre[0]);
                verticesRefatorados.push_back(insereEntre[1]);
                verticesRefatorados.push_back(v.id());
                double novoCusto = refazCalculoAresta(verticesRefatorados);
                atualizaTamanhoTripT(novoCusto, tripAtual);
                std::cout << "NovoCusto: " << novoCusto << std::endl;
                std::cout << "TamTrip: " << this->listaTamanhoTrips[tripAtual - 1] << std::endl;
            }
            verticesTrip.push_back(getVerticeById(insereEntre[1]));
            imprimeListaTripTour();

            if(!insercaoProibida){
                listaCandidatos.insert(listaCandidatos.begin() + i, v);
                int posicao = getVerticeIndex(clientesCandidatos, v);
                clientesCandidatos.erase(clientesCandidatos.begin() + posicao);
            }

            //Depois tratar se totalTour > Tmax

        }
        k++;
        std::cout << "Lista de candidatos na " << k << "º insercao " << std::endl;
        imprimeListaVertices(listaCandidatos);

    } while (breakFlag == 0);

    std::cout << "Parou por FLAG: " << breakFlag << std::endl;
    return listaCandidatos;
}

listavertices_t Grafo::guloso(listavertices_t todosHoteisCandidatos, listavertices_t todosClientesCandidatos){

    /*--------------------------------*/
    /*-------FASE CONSTRUTIVA---------*/
    /*--------------------------------*/

    listavertices_t listaCandidatos = selecionaHoteisCandidatos(todosHoteisCandidatos, numeroDeTrips());

    std::cout << "________________ETAPA_CONSTRUTIVA_________________" << std::endl;
    std::cout << std::endl;
    imprimeListaVertices(listaCandidatos);

    /*--------------------------------*/
    /*---------FASE INSERÇÃO----------*/
    /*--------------------------------*/

    std::cout << "________________ETAPA_DE_INSERCAO_________________" << std::endl;
    std::cout << std::endl;
    listaCandidatos = insereClientes(listaCandidatos, todosClientesCandidatos);

    return listaCandidatos;
}

void Grafo::geraSolucao(){

    /*--------------------------------*/
    /*--------FASE PREPARAÇÃO---------*/
    /*--------------------------------*/

    listavertices_t todosHoteisCandidatos(this->listaVertices.begin(), this->listaVertices.begin() + 2 + numeroDeHoteis());

    std::cout << std::endl;
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << "________________LISTA_DE_HOTEIS_CANDIDATOS_________________" << std::endl;
    std::cout << std::endl;
    imprimeListaVertices(todosHoteisCandidatos);

    listavertices_t todosClientesCandidatos(this->listaVertices.begin() + 2 + numeroDeHoteis(), this->listaVertices.end());

    std::cout << "___________LISTA_DE_PONTOS_TURISTICOS_CANDIDATOS____________" << std::endl;
    std::cout << std::endl;
    imprimeListaVertices(todosClientesCandidatos);

    listavertices_t solucao = guloso(todosHoteisCandidatos, todosClientesCandidatos);

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

    g.setaTamMaxTrips(tamTrips);

    listatour_t CheckTripInicial(nTrips, 0);
    g.setaCheckTrips(CheckTripInicial);

    for (size_t i = 0; i < nVertices; i++) {
        std::string str_linha;

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

void Grafo::imprimeGrafoHelper(){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << "Lista de Vertices: " << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < numeroDeVertices(); i++) {
        if(i == 0){ std::cout << "H_i" << " | ";}
        else if(i == 1){ std::cout << "H_f" << " | ";}
        else if(i < 2 + numeroDeHoteis())
            { std::cout << "H" << listaVertices[i].id() << " | ";}
        else{ std::cout << "V" << listaVertices[i].id() << " | ";}

        std::cout << this->listaVertices[i].toString() << std::endl;
    }
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

void Grafo::imprimeListaVertices(listavertices_t verticesToPrint){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < verticesToPrint.size(); i++) {
        if(verticesToPrint[i].isHotel())
            { std::cout << "H" << verticesToPrint[i].id() << " | ";}
        else{ std::cout << "V" << verticesToPrint[i].id() << " | ";}

        std::cout << verticesToPrint[i].toString();
        if(i != verticesToPrint.size()-1){ std::cout << " | " << matrizDist[verticesToPrint[i].id()][verticesToPrint[i+1].id()]; }
        std::cout << " | " << std::endl;
    }
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

void Grafo::imprimeListaOrdenada(listavertices_t verticesToPrint, size_t idOrigem){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < verticesToPrint.size(); i++) {
        if(verticesToPrint[i].isHotel())
            { std::cout << "H" << verticesToPrint[i].id() << " | ";}
        else{ std::cout << "V" << verticesToPrint[i].id() << " | ";}

        std::cout << verticesToPrint[i].toString() << " | ";
        std::cout << this->matrizDist[idOrigem][verticesToPrint[i].id()] << std::endl;
    }
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

void Grafo::imprimeListaTripTour(){
    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
    std::cout << "Lista da Tour com " << numeroDeTrips() << " Trips" <<  std::endl;
    std::cout << std::endl;
    float somaTour = 0;
    for (size_t i = 0; i < numeroDeTrips(); i++) {
        std::cout << "TRIP_" << i << " - " << this->listaTamanhoTrips[i] << " | " << this->listaTamanhoMaxTrips[i] <<  " | " << this->listaCheckTrips[i] << std::endl;
        somaTour += this->listaTamanhoTrips[i];
    }
    std::cout << "TOUR" << " - " << somaTour << " | " << tamanhoMaximo() << std::endl;

    std::cout << "___________________________________________________________" << std::endl;
    std::cout << std::endl;
}

struct InfoSolucao {
    double distanciaTotal;
    double tempoViajando;
};
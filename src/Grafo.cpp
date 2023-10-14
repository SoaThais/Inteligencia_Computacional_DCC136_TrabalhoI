#include "Grafo.hpp"
#include <iomanip>
#include <algorithm>


#define ERR_EMPTY_CANDIDATES             1
#define ERR_NOT_ENOUGH_CANDIDATES        2

Grafo::Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax):
    _graphName(graphName),
    _numeroHoteis(numeroDeHoteis),
    _numeroTrips(numeroDeTrips),
    _tMax(tMax)
{
    this->listaVertices.reserve(numeroDeVertices);
    this->listaVariancias.reserve(numeroDeVertices);
    this->listaTamanhoTrips.reserve(numeroDeTrips);
}

/*-----------Sets/Gets-----------*/

void Grafo::setaTamTrips(listatour_t tamTrips) {
    this->listaTamanhoTrips = tamTrips;
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
    std::cout << "Hello Thais " << std::endl;
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

/*-----------Auxiliares-----------*/

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
    //std::cerr << "Variancia: " << variancia << std::endl;
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
            //if(i==1){std::cerr << std::setprecision(2) << matrizDistancias[i][j] << ",";}
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

    //std::cout << "LISTA ORDENADA" << std::endl;
    //imprimeListaOrdenada(listaOrdenada, idOrigem);
    return listaOrdenada;
}

listavertices_t Grafo::selecionaHoteisCandidatos(listavertices_t hoteis, int nTrips) {

    listavertices_t resultado;
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

    return resultado;
}

Vertice Grafo::selecionaClienteIdeal(listaids_t insereEntre, listavertices_t clientesCandidatos){

    if (clientesCandidatos.size() == 1){
        return clientesCandidatos[0];
    }

    //calculaToleranciaPorTrip([h0,hx],[hx,hxx],[hxx,hf]) - calcula o de menor distancia com tolerancia
    //h0 [v1,v5] hx [v2,v8] hxx [v3,v7] hf
    Vertice v(0, 0, 0, 0, false);

    listavertices_t possiveis0 = quickSort(insereEntre[0], clientesCandidatos);
    listavertices_t possiveis1 = quickSort(insereEntre[1], clientesCandidatos);
    double menorDist0 = distanciaEuclidiana(getVerticeById(insereEntre[0]), possiveis0[0]);
    double menorDist1 = distanciaEuclidiana(getVerticeById(insereEntre[1]), possiveis1[0]);
    double minimo = 1;
    size_t limitador = 5;

    if(menorDist0 <= minimo){
        v = possiveis0[0];
    }
    else if(menorDist1 <= minimo){
        v = possiveis1[0];
    }
    else if (clientesCandidatos.size() <= limitador){
        v = maiorScore(clientesCandidatos);
    }
    else{

        size_t i = 0;
        listavertices_t tolerancia0;
        listavariancias_t distancias0;
        do
        {
            distancias0.push_back(distanciaEuclidiana(getVerticeById(insereEntre[0]), possiveis0[++i]));
            tolerancia0.push_back(possiveis0[i]);
        } while (distancias0[i] < menorDist0 + listaVariancias[insereEntre[0]] && i <= limitador);

        size_t j = 0;
        listavertices_t tolerancia1;
        listavariancias_t distancias1;

        do
        {
            distancias1.push_back(distanciaEuclidiana(getVerticeById(insereEntre[1]), possiveis1[++j]));
            tolerancia1.push_back(possiveis1[j]);
        } while (distancias1[j] < menorDist1 + listaVariancias[insereEntre[1]] && j <= limitador);

        listavertices_t toleranciaCombo;
        for(size_t k = 0; k < limitador; k++){
            if(distancias0[k] <= distancias1[k])
                toleranciaCombo.push_back(tolerancia0[k]);
            else
                toleranciaCombo.push_back(tolerancia1[k]);
        }
        std::cout << "Combo: " << std::endl;
        imprimeListaVertices(toleranciaCombo);

        v = maiorScore(toleranciaCombo);
    }

    return v;
}

listavertices_t Grafo::insereClientes(listavertices_t listaCandidatos, listavertices_t clientesCandidatos) {

    //inserePorTour([h0,hf]) - aloca espaços para inserção (verificando se cada trip não passou do limite)
    //h0 va hx vb hxx vc hf
    int breakFlag = 0;
    int k = 0;
    do
    {
        for (int i = listaCandidatos.size() - 1; i > 0; i--) {

            if(clientesCandidatos.empty()){
                breakFlag = ERR_EMPTY_CANDIDATES;
                break;
            }
            // if(clientesCandidatos.size() < listaCandidatos.size() - 1){
            //     breakFlag = ERR_NOT_ENOUGH_CANDIDATES;
            // }

            // imprimeListaVertices(clientesCandidatos);

            listaids_t insereEntre;

            insereEntre.push_back(listaCandidatos.at(i).id());
            insereEntre.push_back(listaCandidatos.at(i-1).id());

            std::cout << "Vai inserir entre: " << insereEntre[0] << " e " << insereEntre[1];

            Vertice v = selecionaClienteIdeal(insereEntre, clientesCandidatos);

            std::cout << " o vertice " << v.id() << std::endl;

            listaCandidatos.insert(listaCandidatos.begin() + i, v);

            int posicao = getVerticeIndex(clientesCandidatos, v);
            clientesCandidatos.erase(clientesCandidatos.begin() + posicao);
            //remove v de clientesCandidatos

        }

        k++;
        std::cout << "Lista de candidatos na " << k << "º insercao " << std::endl;
        imprimeListaVertices(listaCandidatos);

    } while (breakFlag == 0);

    std::cout << "Parou por FLAG: " << breakFlag << std::endl;
    return listaCandidatos;
}

listavertices_t Grafo::guloso(listavertices_t todosHoteisCandidatos, listavertices_t todosClientesCandidatos){

    //FASE CONSTRUTIVA

    //seleciona [ntrips-1] candidatos entre h0 e hf para a lista
    listavertices_t listaCandidatos = selecionaHoteisCandidatos(todosHoteisCandidatos, numeroDeTrips());

    std::cout << "Lista de candidatos:" << std::endl;
    imprimeListaVertices(listaCandidatos);

    //FASE INSERÇÃO

    //insere vertices clientes
    listaCandidatos = insereClientes(listaCandidatos, todosClientesCandidatos);

    return listaCandidatos;
}

void Grafo::geraSolucao(){

    //gera lista de candidatos com todos os hoteis
    listavertices_t todosHoteisCandidatos(this->listaVertices.begin(), this->listaVertices.begin() + 2 + numeroDeHoteis());
    imprimeListaVertices(todosHoteisCandidatos);

    listavertices_t todosClientesCandidatos(this->listaVertices.begin() + 2 + numeroDeHoteis(), this->listaVertices.end());
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

        std::cout << verticesToPrint[i].toString() << std::endl;
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

struct InfoSolucao {
    double distanciaTotal;
    double tempoViajando;
};
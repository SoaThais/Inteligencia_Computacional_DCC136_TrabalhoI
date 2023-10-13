#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <set>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <list>

#include <iostream>

#include "Vertice.hpp"

using listavertices_t = std::vector<Vertice>;
using listatour_t = std::vector<float>;


// using listcand_t = std::set<size_t>;
// using solList_t = std::list<std::vector<size_t>>;


class Grafo {
    private:
        std::string _graphName;

        size_t _numeroHoteis;
        size_t _numeroTrips;
        size_t _tMax;
        listavertices_t listaVertices;
        listatour_t listaTamanhoTrips;
        double** matrizDist;

        void setaTamTrips(std::vector <float> tamTrips);
        void setaMatrizDists();

        Vertice& getVerticeById(size_t id);
        double distanciaEuclidiana(Vertice a, Vertice b);
        double** calculaMatrizDistancias();

        listavertices_t selecionaHoteisCandidatos(std::vector<Vertice> hoteis, int nTrips);

    public:
        Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax);

        //Entrada
        static Grafo lerArquivo(std::istream& arqEntrada, std::string nomeArquivo);

        //Saida
        void imprimeGrafo();
        void imprimeGrafoHelper();
        void imprimeListaVertices(listavertices_t listaVertice);

        //Solucao

        listavertices_t guloso();
        void geraSolucao();

        //Gets
        size_t numeroDeVertices() const { return this->listaVertices.size(); };
        size_t numeroDeHoteis() const { return this->_numeroHoteis; };
        size_t numeroDeTrips() const { return this->_numeroTrips; };
        size_t tamanhoMaximo() const { return this->_tMax; };
        std::string graphName() const { return this->_graphName; };
};

#endif // GRAFO_HPP
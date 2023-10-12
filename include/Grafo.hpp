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

//vetorzão
//h0 - h5 - h7 - hf
//ntrips + 1 hoteis
//ntrips - 1 deles são aleatorios

using listcand_t = std::set<size_t>;
using solList_t = std::list<std::vector<size_t>>;


class Grafo {
    private:
        std::string _graphName;

        size_t _numeroHoteis;
        size_t _numeroTrips;
        size_t _tMax;
        listavertices_t listaVertices;
        listatour_t listaTamanhoTrips;

        Vertice& getVerticeById(size_t id);
        double** calculaMatrizDist(const size_t nVert);
        double distanciaTotal(solList_t& solList);

    public:
        Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax);
        void setaTamTrips(std::vector <float> tamTrips);

        static Grafo lerArquivo(std::istream& arqEntrada, std::string nomeArquivo);
        void imprimeGrafo();

        size_t numeroDeVertices() const;
        std::string graphName() { return this->_graphName; };
};

#endif // GRAFO_HPP
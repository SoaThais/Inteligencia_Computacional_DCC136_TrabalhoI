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

        Vertice& getVerticeById(size_t id);
        listavertices_t selecionaHoteisCandidatos(std::vector<Vertice> hoteis, int nTrips);

    public:
        Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax);
        void setaTamTrips(std::vector <float> tamTrips);

        static Grafo lerArquivo(std::istream& arqEntrada, std::string nomeArquivo);

        void imprimeGrafo();
        void imprimeGrafoHelper();
        void imprimeListaVertices(listavertices_t listaVertice);

        listavertices_t guloso();
        void geraSolucao();

        size_t numeroDeVertices() const;
        std::string graphName() { return this->_graphName; };
};

#endif // GRAFO_HPP
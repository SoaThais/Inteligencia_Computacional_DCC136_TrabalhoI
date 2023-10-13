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
using listavariancias_t = std::vector<double>;
using listatour_t = std::vector<float>;
using listaids_t = std::vector<size_t>;


// using listcand_t = std::set<size_t>;
// using solList_t = std::list<std::vector<size_t>>;


class Grafo {
    private:
        std::string _graphName;

        size_t _numeroHoteis;
        size_t _numeroTrips;
        size_t _tMax;
        listavertices_t listaVertices;
        listavariancias_t listaVariancias;
        listatour_t listaTamanhoTrips;
        double** matrizDist;

        //Sets e Gets
        void setaTamTrips(listatour_t tamTrips);
        void setaVariancias(listavariancias_t variancias);
        void setaMatrizDists();
        Vertice& getVerticeById(size_t id);

        //Auxiliares
        double distanciaEuclidiana(Vertice a, Vertice b);
        double calculaVariancia(double media, double *distancias);
        double** calculaMatrizDistancias();
        Vertice maiorScore(listavertices_t vertices);

        //Auxiliares Solucao
        listavertices_t selecionaHoteisCandidatos(listavertices_t hoteis, int nTrips);
        Vertice selecionaCandidatoIdeal(listaids_t insereEntre, listavertices_t verticesCandidatos);
        listavertices_t insereCandidatos(listavertices_t listaCandidatos, listavertices_t verticesCandidatos);


    public:
        Grafo(std::string graphName, size_t numeroDeVertices, size_t numeroDeHoteis, size_t numeroDeTrips, size_t tMax);

        //Solucao
        listavertices_t guloso(listavertices_t todosHoteisCandidatos, listavertices_t todosVerticesCandidatos);
        void geraSolucao();

        //Entrada
        static Grafo lerArquivo(std::istream& arqEntrada, std::string nomeArquivo);

        //Saida
        void imprimeGrafo();
        void imprimeGrafoHelper();
        void imprimeListaVertices(listavertices_t listaVertice);

        //Gets
        size_t numeroDeVertices() const { return this->listaVertices.size(); };
        size_t numeroDeHoteis() const { return this->_numeroHoteis; };
        size_t numeroDeTrips() const { return this->_numeroTrips; };
        size_t tamanhoMaximo() const { return this->_tMax; };
        std::string graphName() const { return this->_graphName; };
};

#endif // GRAFO_HPP
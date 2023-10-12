#include "Vertice.hpp"

Vertice::Vertice(size_t id, double x, double y, double score, bool isHotel):
    _idV(id),
    _x(x),
    _y(y),
    _score(score),
    _isHotel(isHotel)
{
}

size_t Vertice::id() const
{
    return this->_idV;
}

double Vertice::x() const
{
    return this->_x;
}

double Vertice::y() const
{
    return this->_y;
}

double Vertice::score()
{
    return this->_score;
}

bool Vertice::isHotel() const
{
    return this->_isHotel;
}
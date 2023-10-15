#include "Vertice.hpp"
#include <utility>

Vertice::Vertice(size_t id, double x, double y, double score, bool isHotel):
    _idV(id),
    _x(x),
    _y(y),
    _score(score),
    _isHotel(isHotel),
    _tripID(0,0)
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

std::pair<size_t, size_t> Vertice::tripId()
{
    return this->_tripID;
}

void Vertice::updateTripId(size_t t1, size_t t2)
{
    this->_tripID = std::make_pair(t1, t2);
}

std::string Vertice::toString() {
  std::string s;

  // Adicione o id, as coordenadas, o score e o tipo do vértice à string s
  s += std::to_string(id()) + " | ";
  s += "(" + std::to_string(x()) + ", " + std::to_string(y()) + ") | ";
  s += std::to_string(score()) + " | ";
  s += isHotel() ? "Hotel" : "Normal";
  s += " | TRIP-" + std::to_string(tripId().first) + "-" + std::to_string(tripId().second);

  return s;
}
#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <string>

class Vertice {
    private:
        size_t _idV;
        double _x;
        double _y;
        double _score;
        bool _isHotel;

    public:
        Vertice(size_t id, double x, double y, double score, bool isHotel);

        size_t id() const;
        double x() const;
        double y() const;
        double score();
        bool isHotel() const;

        std::string toString();
};

#endif // VERTICE_HPP
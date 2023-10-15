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
        std::pair<size_t, size_t> _tripID;

    public:
        Vertice(size_t id, double x, double y, double score, bool isHotel);
        void updateTripId(size_t t1, size_t t2);

        size_t id() const;
        double x() const;
        double y() const;
        double score();
        bool isHotel() const;
        bool operator==(const Vertice& other) const {
            return (this->id() == other.id());
        }
        std::pair<size_t, size_t> tripId();
        std::string toString();
};

#endif // VERTICE_HPP
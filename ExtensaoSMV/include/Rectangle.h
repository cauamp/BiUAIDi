#include "Point.h"

/**
 * @class Rectangle
 * @brief Representa um retângulo em um espaço 2D.
 *
 * A classe Rectangle fornece funcionalidades para trabalhar com retângulos, incluindo verificar se um ponto está contido dentro do retângulo e verificar a interseção com outro retângulo.
 */
class Rectangle
{
private:
    Point _lb, _rt;
public:

    Rectangle(Point _lb = Point(0, 0), Point _rt = Point(0, 0)) : _lb(_lb), _rt(_rt) {}

    bool contains(const Point &p) const
    {
        return (p.getX() >= this->_lb.getX() && p.getX() <= this->_rt.getX() && p.getY() >= this->_lb.getY() && p.getY() <= this->_rt.getY());
    }

    Point getLB() const
    {
        return _lb;
    }

    Point getRT() const
    {
        return _rt;
    }
};
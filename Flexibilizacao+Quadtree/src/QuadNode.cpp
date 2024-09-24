#include "QuadNode.h"

QuadNode::QuadNode(Rectangle boundary, quadnodekey_t key, quadnodeaddr_t ne, quadnodeaddr_t nw, quadnodeaddr_t se, quadnodeaddr_t sw, Point *ponto) : _boundary(boundary), key(INVALIDKEY), ne(ne), nw(nw), se(se), sw(sw), _point(ponto)
{
}

void QuadNode::reset()
{
    key = INVALIDKEY;
    ne = nw = se = sw = INVALIDADDR;
    _point = nullptr;
}

quadnodeaddr_t QuadNodeManager::createNode(const QuadNode &pn)
{
    nodes[_size++] = pn;
    return _size - 1; // Retorna o índice do novo nó
}

void QuadNodeManager::deleteNode(quadnodeaddr_t addr)
{
    if (addr >= 0 && static_cast<size_t>(addr) < _size)
    {
        nodes[addr].reset();
    }
}

void QuadNodeManager::putNode(quadnodeaddr_t addr, QuadNode &pn)
{
    if (addr >= 0 && static_cast<size_t>(addr) < _size)
    {
        pn.key = addr;
        nodes[addr] = pn;
    }
}

QuadNode QuadNodeManager::getNode(quadnodeaddr_t addr) const
{
    if (addr >= 0 && static_cast<size_t>(addr) < _size)
    {
        return nodes[addr];
    }
    return QuadNode(Rectangle(Point(0, 0), Point(0, 0)));
}

void QuadNodeManager::initialize(long capacity)
{
    _capacity = capacity;
    nodes = new QuadNode[_capacity];
}

void QuadNodeManager::destroy()
{
    delete[] nodes;
}

quadnodeaddr_t QuadNodeManager::localize(quadnodeaddr_t addr, const Point &p)
{
    QuadNode nx = getNode(addr);
    Rectangle _boundary = nx._boundary;
    Point *_point = nx._point;
    Point mid(_point->getX(), _point->getY());

    Rectangle neRet = Rectangle(mid, Point(_boundary.getRT().getX(), _boundary.getRT().getY()));
    Rectangle nwRet = Rectangle(Point(_boundary.getLB().getX(), mid.getY()), Point(mid.getX(), _boundary.getRT().getY()));
    Rectangle swRet = Rectangle(Point(_boundary.getLB().getX(), _boundary.getLB().getY()), mid);
    Rectangle seRet = Rectangle(Point(mid.getX(), _boundary.getLB().getY()), Point(_boundary.getRT().getX(), mid.getY()));

    if (_boundary.contains(p))
    {
        if (neRet.contains(p))
        {
            if (nodes[addr].ne == INVALIDADDR)
            {
                nodes[addr].ne = createNode(QuadNode(neRet));
            }
            return nodes[addr].ne;
        }
        else if (nwRet.contains(p))
        {
            if (nodes[addr].nw == INVALIDADDR)
            {
                nodes[addr].nw = createNode(QuadNode(nwRet));
            }
            return nodes[addr].nw;
        }
        else if (swRet.contains(p))
        {
            if (nodes[addr].sw == INVALIDADDR)
            {
                nodes[addr].sw = createNode(QuadNode(swRet));
            }
            return nodes[addr].sw;
        }
        else if (seRet.contains(p))
        {
            if (nodes[addr].se == INVALIDADDR)
            {
                nodes[addr].se = createNode(QuadNode(seRet));
            }
            return nodes[addr].se;
        }
    }
    return INVALIDADDR;
}
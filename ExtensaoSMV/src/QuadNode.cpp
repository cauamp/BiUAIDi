// QuadNode.cpp
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

void QuadNodeManager::initialize(long capacity)
{
    smv = &SMV::getInstance();

    int bytesallocated;
    nodes = reinterpret_cast<QuadNode *>(smv->initPage(bytesallocated));

    if (nodes == nullptr)
    {
        throw QuadNodeManagerException("Falha ao alocar memória para nodes usando SMV.");
    }

    if (static_cast<int>(capacity * sizeof(QuadNode)) > bytesallocated)
    {
        throw QuadNodeManagerException("SMV não forneceu memória suficiente.");
    }

    _capacity = bytesallocated / sizeof(QuadNode);
    std::cout << "QuadNodeManager initialized with capacity: " << _capacity << std::endl;
    _size = 0;

    for (size_t i = 0; i < _capacity; ++i)
    {
        new (&nodes[i]) QuadNode();
    }
    std::cout << "QuadNodeManager inicializado." << std::endl;
}

void QuadNodeManager::destroy()
{
    if (nodes != nullptr)
    {
        for (size_t i = 0; i < _capacity; ++i)
        {
            nodes[i].~QuadNode();
        }

        nodes = nullptr;
        _size = 0;
        _capacity = 0;
    }
}

quadnodeaddr_t QuadNodeManager::createNode(const QuadNode &pn)
{
    if (_size >= _capacity)
    {
        throw QuadNodeManagerException("Capacidade máxima de nós atingida.");
    }

    if (nodes == nullptr)
    {
        std::cerr << "QuadNodeManager: Memória para nodes não alocada." << std::endl;
        return INVALIDADDR;
    }

    quadnodeaddr_t addr = _size++;
    nodes[addr] = pn;

    return addr;
}

void QuadNodeManager::deleteNode(quadnodeaddr_t addr)
{
    if (addr < 0 || static_cast<size_t>(addr) >= _size)
    {
        std::cerr << "QuadNodeManager: Endereço fora do intervalo." << std::endl;
        return;
    }
    nodes[addr].reset();
}

QuadNode QuadNodeManager::getNode(quadnodeaddr_t addr) const
{
    if (addr < 0 || static_cast<size_t>(addr) >= _size)
    {
        std::cerr << "QuadNodeManager: Endereço fora do intervalo." << std::endl;
        return QuadNode();
    }
    return nodes[addr];
}

void QuadNodeManager::putNode(quadnodeaddr_t addr, QuadNode &pn)
{
    // Verificar se o endereço está dentro do intervalo válido
    if (addr < 0 || static_cast<size_t>(addr) >= _size)
    {
        std::cerr << "QuadNodeManager: Endereço fora do intervalo. Addr: " << addr << ", Size: " << _size << std::endl;
        return;
    }

    if (nodes == nullptr)
    {
        std::cerr << "QuadNodeManager: Memória para nodes não alocada." << std::endl;
        return;
    }

    nodes[addr] = pn;
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

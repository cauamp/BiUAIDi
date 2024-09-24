#include "QuadTree.h"
#include <algorithm>
#include <iostream>

QuadTree::QuadTree(long numNodes, Rectangle boundary)
    : _root(0), _nodeManager()
{
    _nodeManager.initialize(numNodes);

    QuadNode root_node(boundary, 0, INVALIDADDR, INVALIDADDR, INVALIDADDR, INVALIDADDR, nullptr);
    _root = _nodeManager.createNode(root_node); // Use createNode instead of putNode
}

QuadTree::~QuadTree()
{
    destroy();
}

quadnodeaddr_t QuadTree::search(Point &p)
{
    quadnodeaddr_t current = _root;

    while (current != INVALIDADDR)
    {
        QuadNode currentNode = _nodeManager.getNode(current);

        if (currentNode._point == nullptr)
        {
            break;
        }
        if (*(currentNode._point) == p)
        {
            return current;
        }

        current = _nodeManager.localize(current, p);
    }

    return INVALIDADDR;
}

quadnodeaddr_t QuadTree::insert(Point &p)
{
    quadnodeaddr_t current = _root;

    while (true)
    {
        QuadNode currentNode = _nodeManager.getNode(current);

        if (!currentNode._boundary.contains(p))
        {
            return INVALIDADDR;
        }
        if (currentNode._point == nullptr)
        {
            currentNode._point = &p;
            _nodeManager.putNode(current, currentNode);
            return current;
        }
        else
        {
            current = _nodeManager.localize(current, p);
            if (current == INVALIDADDR) // Check if localization was successful
            {
                return INVALIDADDR;
            }
        }
    }
}

void QuadTree::destroy()
{
    _nodeManager.destroy();
}

double heuristic(const Point &p, const Rectangle &box)
{
    double xMin = box.getLB().getX();
    double xMax = box.getRT().getX();
    double yMin = box.getLB().getY();
    double yMax = box.getRT().getY();

    Point center((xMin + xMax) / 2.0, (yMin + yMax) / 2.0);
    Point bottomLeft(xMin, yMin);
    Point bottomRight(xMax, yMin);
    Point topLeft(xMin, yMax);
    Point topRight(xMax, yMax);

    double distCenter = p.distance(center);
    double distBottomLeft = p.distance(bottomLeft);
    double distBottomRight = p.distance(bottomRight);
    double distTopLeft = p.distance(topLeft);
    double distTopRight = p.distance(topRight);

    return std::min({distBottomLeft, distBottomRight, distTopLeft, distTopRight, distCenter});
}

void QuadTree::KNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq)
{
    // Iterate over all nodes in memory
    for (size_t i = 0; i < _nodeManager._size; ++i)
    {
        try
        {
            QuadNode currentNode = _nodeManager.getNode(i);

            if (currentNode._point != nullptr)
            {
                double dist = currentNode._point->distance(p);
                Pair<double, Point> d(dist, *currentNode._point);
                if (currentNode._point->isActive())
                {
                    if (pq.size() < K)
                    {
                        pq.push(d);
                    }
                    else if (dist < pq.top().getFirst())
                    {
                        pq.pop();
                        pq.push(d);
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao acessar o nó: " << e.what() << '\n';
        }
    }
}

void QuadTree::HeuristicKNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq)
{
    PriorityQueue<Pair<double, quadnodeaddr_t>> pq_aux(100, true);
    pq_aux.push(Pair<double, quadnodeaddr_t>(0, _root));

    size_t visited = 0;
    size_t stagnated = 0;
    while (!pq_aux.empty() && (stagnated <= 10))
    {
        quadnodeaddr_t current = pq_aux.top().getSecond();
        pq_aux.pop();

        try
        {
            QuadNode currentNode = _nodeManager.getNode(current);

            if (!currentNode._boundary.contains(p))
            {
                continue;
            }
            visited++;

            const Point *ponto = currentNode._point;

            if (ponto != nullptr)
            {
                double dist = ponto->distance(p);

                Pair<double, Point> d(dist, *ponto);
                if (ponto->isActive())
                {
                    if (pq.size() < K)
                    {
                        pq.push(d);
                        stagnated = 0;
                    }
                    else if (dist < pq.top().getFirst())
                    {
                        pq.pop();
                        pq.push(d);
                        stagnated = 0;
                    }
                    else
                    {
                        stagnated++;
                    }
                }

                // Add child nodes to priority queue
                if (currentNode.ne != INVALIDADDR)
                {
                    double heuristicDist = heuristic(p, _nodeManager.getNode(currentNode.ne)._boundary);
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.ne));
                }
                if (currentNode.nw != INVALIDADDR)
                {
                    double heuristicDist = heuristic(p, _nodeManager.getNode(currentNode.nw)._boundary);
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.nw));
                }
                if (currentNode.sw != INVALIDADDR)
                {
                    double heuristicDist = heuristic(p, _nodeManager.getNode(currentNode.sw)._boundary);
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.sw));
                }
                if (currentNode.se != INVALIDADDR)
                {
                    double heuristicDist = heuristic(p, _nodeManager.getNode(currentNode.se)._boundary);
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.se));
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao acessar o nó: " << e.what() << '\n';
        }
    }
}

#include "QuadTree.h"
#include <algorithm>

QuadTree::QuadTree(long numNodes, Rectangle boundary) : root(0)
{
    nodeManager.initialize(numNodes);
    QuadNode root_node(boundary, 0, INVALIDADDR, INVALIDADDR, INVALIDADDR, INVALIDADDR, nullptr);
    nodeManager.createNode(root_node);
}

quadnodeaddr_t QuadTree::search(Point &p)
{
    quadnodeaddr_t current = root;

    while (current != INVALIDADDR)
    {
        QuadNode currentNode = nodeManager.getNode(current);

        if (currentNode._point == nullptr)
        {
            break;
        }
        if (*(currentNode._point) == p)
        {
            return current;
        }

        current = nodeManager.localize(current, p);
    }

    return INVALIDADDR;
}

quadnodeaddr_t QuadTree::insert(Point &p)
{

    quadnodeaddr_t current = root;

    while (true)
    {
        QuadNode currentNode = nodeManager.getNode(current);

        if (!currentNode._boundary.contains(p))
        {
            return INVALIDADDR;
        }
        if (currentNode._point == nullptr)
        {
            currentNode._point = &p;
            nodeManager.putNode(current, currentNode);
            return current;
        }
        else
        {
            current = nodeManager.localize(current, p);
        }
    }
}

void QuadTree::destroy()
{
    nodeManager.destroy();
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

void QuadTree::KNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq) const
{
    for(size_t i = 0; i < nodeManager._size; i++){
        QuadNode currentNode = nodeManager.getNode(i);
        if(currentNode._point != nullptr){
            double dist = currentNode._point->distance(p);
            Pair<double, Point> d(dist, *currentNode._point);
            if(currentNode._point->isActive()){
                if(pq.size() < K){
                    pq.push(d);
                }else if(dist < pq.top().getFirst()){
                    pq.pop();
                    pq.push(d);
                }
            }
        }
    }
}

void QuadTree::HeuristicKNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq) const
{
    PriorityQueue<Pair<double, quadnodeaddr_t>> pq_aux(nodeManager._capacity / 2, true);
    pq_aux.push(Pair<double, quadnodeaddr_t>(0, root));

    size_t visited = 0;
    size_t stagnated = 0;
    while (!pq_aux.empty() && (stagnated <= nodeManager._capacity / 10 || pq.size() < K))
    {
        quadnodeaddr_t current = pq_aux.top().getSecond();

        pq_aux.pop();

        QuadNode currentNode = nodeManager.getNode(current);

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

            if (currentNode.ne != INVALIDADDR)
            {
                double heuristicDist = heuristic(p, nodeManager.getNode(currentNode.ne)._boundary);
                try
                {
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.ne));
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            if (currentNode.nw != INVALIDADDR)
            {
                double heuristicDist = heuristic(p, nodeManager.getNode(currentNode.nw)._boundary);
                try
                {
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.nw));
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            if (currentNode.sw != INVALIDADDR)
            {
                double heuristicDist = heuristic(p, nodeManager.getNode(currentNode.sw)._boundary);
                try
                {
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.sw));
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            if (currentNode.se != INVALIDADDR)
            {
                double heuristicDist = heuristic(p, nodeManager.getNode(currentNode.se)._boundary);
                try
                {
                    pq_aux.push(Pair<double, quadnodeaddr_t>(heuristicDist, currentNode.se));
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
    }

}

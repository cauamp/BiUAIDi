#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadNode.h"

/**
 * @class QuadTree
 * @brief Implementa uma estrutura de árvore quaternária para armazenamento e manipulação de pontos em um espaço 2D.
 *
 * A QuadTree divide o espaço em sub-regiões (nós) para organizar eficientemente a inserção, busca, remoção e consultas
 * de pontos, como a busca dos K vizinhos mais próximos.
 */
class QuadTree
{
private:
    quadnodeaddr_t _root;         ///< Endereço do nó raiz da árvore.
    QuadNodeManager _nodeManager; ///< Gerenciador de nós que armazena e gerencia os nós da árvore quaternária.

public:
    /**
     * @brief Construtor da classe QuadTree.
     * @param numNodes Número máximo de nós que a árvore pode conter.
     * @param boundary Limites iniciais que definem a área coberta pela árvore.
     */
    QuadTree(long numNodes, Rectangle boundary);

    /**
     * @brief Destrutor da classe QuadTree.
     */
    ~QuadTree();	

    /**
     * @brief Busca um ponto na árvore quaternária.
     * @param p O ponto a ser buscado.
     * @return O endereço do nó que contém o ponto, ou INVALIDADDR se o ponto não for encontrado.
     */
    quadnodeaddr_t search(Point &p);

    /**
     * @brief Insere um ponto na árvore quaternária.
     * @param p O ponto a ser inserido.
     * @return O endereço do nó onde o ponto foi inserido.
     */
    quadnodeaddr_t insert(Point &p);

    /**
     * @brief Destroi a árvore quaternária, liberando os recursos alocados.
     */
    void destroy();

    /**
     * @brief Realiza uma busca pelos K pontos mais próximos de um ponto dado.
     * @param p O ponto de referência para a busca.
     * @param K O número de vizinhos mais próximos a serem encontrados.
     * @param pq Uma fila de prioridade que armazenará os pares (distância, ponto) dos K vizinhos mais próximos.
     */
    void KNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq);

    /**
     * @brief Realiza uma busca pelos K pontos mais próximos de um ponto dado considerando uma heuristica.
     * @param p O ponto de referência para a busca.
     * @param K O número de vizinhos mais próximos a serem encontrados.
     * @param pq Uma fila de prioridade que armazenará os pares (distância, ponto) dos K vizinhos mais próximos.
     */
    void HeuristicKNNSearch(const Point &p, int K, PriorityQueue<Pair<double, Point>> &pq);
};

#endif // QUADTREE_H

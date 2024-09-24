#ifndef QUADNODE_H
#define QUADNODE_H

#include <iostream>
#include <cmath>
#include "Point.h"
#include "Rectangle.h"
#include "PriorityQueue.h"
#include "Pair.h"

#define INVALIDKEY -1   // Definição para chave inválida
#define INVALIDADDR -2  // Definição para endereço inválido

// Definição de tipos para chave e endereço do nó na QuadTree
using quadnodekey_t = long;
using quadnodeaddr_t = long;

/**
 * @class QuadNode
 * @brief Representa um nó de uma QuadTree, que pode conter um ponto e subdivisões para as regiões Nordeste, Noroeste, Sudeste e Sudoeste.
 */
class QuadNode {
private:
    Rectangle _boundary;   ///< Limites do quadrante que o nó representa
    quadnodekey_t key;     ///< Chave única do nó
    quadnodeaddr_t ne;     ///< Endereço do nó filho no quadrante Nordeste
    quadnodeaddr_t nw;     ///< Endereço do nó filho no quadrante Noroeste
    quadnodeaddr_t se;     ///< Endereço do nó filho no quadrante Sudeste
    quadnodeaddr_t sw;     ///< Endereço do nó filho no quadrante Sudoeste
    Point *_point;         ///< Ponto armazenado neste nó, se houver

    /**
     * @brief Construtor do QuadNode.
     * @param boundary Limites do quadrante.
     * @param key Chave única do nó.
     * @param ne Endereço do nó filho no quadrante Nordeste.
     * @param nw Endereço do nó filho no quadrante Noroeste.
     * @param se Endereço do nó filho no quadrante Sudeste.
     * @param sw Endereço do nó filho no quadrante Sudoeste.
     * @param ponto Ponto armazenado neste nó.
     */
    QuadNode(Rectangle boundary = Rectangle(), quadnodekey_t key = INVALIDKEY, 
             quadnodeaddr_t ne = INVALIDADDR, quadnodeaddr_t nw = INVALIDADDR, 
             quadnodeaddr_t se = INVALIDADDR, quadnodeaddr_t sw = INVALIDADDR, 
             Point *ponto = nullptr);

    /**
     * @brief Reseta o conteúdo do nó, definindo valores padrão.
     */
    void reset();

    friend class QuadNodeManager;  ///< Permite que QuadNodeManager acesse membros privados
    friend class QuadTree;         ///< Permite que QuadTree acesse membros privados
};

/**
 * @class QuadNodeManager
 * @brief Gerencia os nós da QuadTree, permitindo a criação, manipulação e destruição de nós.
 */
class QuadNodeManager {
private:
    QuadNode *nodes;       ///< Vetor dinâmico de nós
    size_t _size = 0;      ///< Número atual de nós
    size_t _capacity = 0;  ///< Capacidade máxima de nós

public:
    /**
     * @brief Inicializa o gerenciador de nós com uma capacidade específica.
     * @param capacity Capacidade inicial para o vetor de nós.
     */
    void initialize(long capacity);

    /**
     * @brief Cria um novo nó e o adiciona ao gerenciador.
     * @param pn Nó a ser adicionado.
     * @return Endereço do nó criado.
     */
    quadnodeaddr_t createNode(const QuadNode &pn);

    /**
     * @brief Exclui um nó específico com base no endereço.
     * @param addr Endereço do nó a ser excluído.
     */
    void deleteNode(quadnodeaddr_t addr);

    /**
     * @brief Recupera um nó específico com base no endereço.
     * @param addr Endereço do nó a ser recuperado.
     * @return O nó recuperado.
     */
    QuadNode getNode(quadnodeaddr_t addr) const;

    /**
     * @brief Armazena um nó em um endereço específico.
     * @param addr Endereço onde o nó será armazenado.
     * @param pn Nó a ser armazenado.
     */
    void putNode(quadnodeaddr_t addr, QuadNode &pn);

    /**
     * @brief Destroi todos os nós e libera a memória.
     */
    void destroy();

    /**
     * @brief Localiza o quadrante dentro do nó que contém um ponto específico.
     * @param addr Endereço do nó onde a busca será iniciada.
     * @param p Ponto a ser localizado.
     * @return Endereço do quadrante que contém o ponto.
     */
    quadnodeaddr_t localize(quadnodeaddr_t addr, const Point &p);

    friend class QuadTree;  ///< Permite que QuadTree acesse membros privados
};

#endif

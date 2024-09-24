/**
 * @file QuadNode.h
 * @brief Arquivo de cabeçalho para as classes QuadNode e QuadNodeManager.
 *
 * Este arquivo contém a definição das classes QuadNode e QuadNodeManager, que são responsáveis pela representação
 * e gerenciamento de nós de uma QuadTree, utilizados para particionar o espaço em uma estrutura de árvore espacial.
 */

#ifndef QUADNODE_H
#define QUADNODE_H

#include <iostream>
#include <cmath>
#include "Point.h"
#include "Rectangle.h"
#include "PriorityQueue.h"
#include "Pair.h"
#include "SMV.h"

#define INVALIDKEY -1  /**< Definição para chave inválida */
#define INVALIDADDR -2 /**< Definição para endereço inválido */

using quadnodekey_t = long;  /**< Tipo de dado para a chave de QuadNode */
using quadnodeaddr_t = long; /**< Tipo de dado para o endereço de QuadNode */

/**
 * @class QuadNode
 * @brief Classe que representa um nó da QuadTree.
 *
 * A classe QuadNode é usada para armazenar os limites espaciais de um retângulo e os ponteiros para seus nós filhos (NE, NW, SE, SW).
 * Cada nó contém um ponto associado e pode ser dividido em quatro quadrantes.
 */
class QuadNode
{
private:
    Rectangle _boundary; /**< Limites espaciais do nó, representado por um retângulo */
    quadnodekey_t key;   /**< Chave única associada ao nó */
    quadnodeaddr_t ne;   /**< Endereço do nó filho nordeste */
    quadnodeaddr_t nw;   /**< Endereço do nó filho noroeste */
    quadnodeaddr_t se;   /**< Endereço do nó filho sudeste */
    quadnodeaddr_t sw;   /**< Endereço do nó filho sudoeste */
    Point *_point;       /**< Ponto armazenado neste nó */

    /**
     * @brief Construtor privado da classe QuadNode.
     *
     * Inicializa um QuadNode com valores padrão ou especificados.
     *
     * @param boundary Retângulo que define os limites do nó.
     * @param key Chave associada ao nó.
     * @param ne Endereço do nó filho nordeste.
     * @param nw Endereço do nó filho noroeste.
     * @param se Endereço do nó filho sudeste.
     * @param sw Endereço do nó filho sudoeste.
     * @param ponto Ponteiro para o ponto armazenado neste nó.
     */
    QuadNode(Rectangle boundary = Rectangle(), quadnodekey_t key = INVALIDKEY,
             quadnodeaddr_t ne = INVALIDADDR, quadnodeaddr_t nw = INVALIDADDR,
             quadnodeaddr_t se = INVALIDADDR, quadnodeaddr_t sw = INVALIDADDR,
             Point *ponto = nullptr);

    /**
     * @brief Redefine os valores do nó para os padrões.
     *
     * Reseta o conteúdo do nó, desassociando chaves e pontos.
     */
    void reset();

    friend class QuadNodeManager;
    friend class QuadTree;
};

/**
 * @class QuadNodeManager
 * @brief Classe responsável pelo gerenciamento de nós QuadNode.
 *
 * A classe QuadNodeManager gerencia a criação, exclusão e manipulação de QuadNodes, armazenando-os em um vetor de nós.
 * Implementa a lógica de alocação de memória e fornece operações de inserção, remoção e busca de nós.
 */
class QuadNodeManager
{
private:
    QuadNode *nodes;  /**< Vetor de QuadNodes gerenciados */
    size_t _size;     /**< Tamanho atual do vetor de nós */
    size_t _capacity; /**< Capacidade máxima do vetor de nós */
    SMV* smv;          /**< Instância da classe SMV para gerenciar memória virtual */

public:
    /**
     * @brief Inicializa o gerenciador de nós com uma capacidade específica.
     *
     * @param capacity Capacidade inicial para o vetor de nós.
     */
    void initialize(long capacity);

    /**
     * @brief Destroi o gerenciador de nós, liberando todos os recursos alocados.
     */
    void destroy();

    /**
     * @brief Cria um novo nó QuadNode no vetor de nós.
     *
     * @param pn Referência para o nó a ser criado.
     * @return Endereço do novo nó criado.
     */
    quadnodeaddr_t createNode(const QuadNode &pn);

    /**
     * @brief Exclui um nó QuadNode com base no endereço fornecido.
     *
     * @param addr Endereço do nó a ser excluído.
     */
    void deleteNode(quadnodeaddr_t addr);

    /**
     * @brief Retorna uma cópia de um nó QuadNode com base no endereço fornecido.
     *
     * @param addr Endereço do nó a ser recuperado.
     * @return O nó QuadNode correspondente ao endereço fornecido.
     */
    QuadNode getNode(quadnodeaddr_t addr) const;

    /**
     * @brief Atualiza um nó QuadNode em um endereço específico.
     *
     * @param addr Endereço do nó a ser atualizado.
     * @param pn Referência para o nó atualizado.
     */
    void putNode(quadnodeaddr_t addr, QuadNode &pn);

    /**
     * @brief Localiza um nó com base em um ponto específico.
     *
     * @param addr Endereço inicial da busca.
     * @param p Ponto a ser localizado no QuadNode.
     * @return O endereço do nó localizado.
     */
    quadnodeaddr_t localize(quadnodeaddr_t addr, const Point &p);

    /**
     * @class QuadNodeManagerException
     * @brief Classe de exceção específica para erros no QuadNodeManager.
     *
     * Define uma exceção personalizada para tratar erros relacionados ao gerenciamento de nós.
     */
    class QuadNodeManagerException : public std::runtime_error
    {
    public:
        /**
         * @brief Construtor da exceção QuadNodeManagerException.
         *
         * @param message Mensagem de erro associada à exceção.
         */
        explicit QuadNodeManagerException(const std::string &message)
            : std::runtime_error(message) {}
    };

    friend class QuadTree;
};

#endif // QUADNODE_H

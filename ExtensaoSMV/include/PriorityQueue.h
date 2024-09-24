#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <cstdlib>
#include "Pair.h"

/**
 * @brief Classe template que representa uma fila de prioridade (heap).
 * @tparam T Tipo de dado armazenado na fila de prioridade.
 */
template <typename T>
class PriorityQueue
{
private:
    T *_heap;      ///< Ponteiro para o array que representa o heap
    int _size;     ///< Tamanho atual da fila de prioridade
    int _capacity; ///< Capacidade máxima da fila de prioridade (padrão é 10000)
    bool _mode;    ///< Modo de operação da fila de prioridade (true = min heap, false = max heap)

    /**
     * @brief Troca dois elementos no heap.
     * @param a Referência para o primeiro elemento.
     * @param b Referência para o segundo elemento.
     */
    void swap(T &a, T &b);

    /**
     * @brief Move um elemento para cima no heap para manter a propriedade do heap.
     * @param index Índice do elemento a ser movido.
     */
    void heapifyUp(int index);

    /**
     * @brief Move um elemento para baixo no heap para manter a propriedade do heap.
     * @param index Índice do elemento a ser movido.
     */
    void heapifyDown(int index);

    /**
     * @brief Obtém o índice do pai de um elemento no heap.
     * @param index Índice do elemento filho.
     * @return Índice do elemento pai.
     */
    int parent(int index);

    /**
     * @brief Obtém o índice do filho esquerdo de um elemento no heap.
     * @param index Índice do elemento pai.
     * @return Índice do filho esquerdo.
     */
    int leftChild(int index);

    /**
     * @brief Obtém o índice do filho direito de um elemento no heap.
     * @param index Índice do elemento pai.
     * @return Índice do filho direito.
     */
    int rightChild(int index);

public:
    /**
     * @brief Construtor da classe PriorityQueue com capacidade e modo personalizados.
     * @param capacity Capacidade máxima da fila de prioridade.
     * @param mode Modo de operação da fila de prioridade.
     */
    PriorityQueue(int capacity = 1000, bool mode = false);

    /**
     * @brief Destrutor da classe PriorityQueue.
     */
    ~PriorityQueue();

    /**
     * @brief Se possível adiciona um elemento à fila de prioridade.
     * @param data Elemento a ser adicionado.
     * @details Este método tenta adicionar um elemento à fila de prioridade. Se o heap tiver capacidade, o elemento será adicionado normalmente. Caso contrário, o lança um excessão.
     */
    void push(T data);

    /**
     * @brief Adiciona um elemento à fila de prioridade.
     * @param data Elemento a ser adicionado.
     * @details Este método adiciona um elemento à fila de prioridade. Se o heap estiver cheio, o elemento será adicionado forçadamente, substituindo o elemento de menor prioridade.
     */
    void forcePush(T data);

    /**
     * @brief Obtém o elemento de maior prioridade da fila.
     * @return Elemento de maior prioridade.
     */
    T top();

    /**
     * @brief Remove o elemento de maior prioridade da fila.
     */
    void pop();

    /**
     * @brief Verifica se a fila de prioridade está vazia.
     * @return true se a fila estiver vazia, false caso contrário.
     */
    bool empty();

    /**
     * @brief Retorna o tamanho da fila de prioridade.
     *
     * @return O tamanho da fila de prioridade.
     */
    int size();

    /**
     * @brief Troca o modo de operação e atualiza a fila de prioridade.
     */
    void toggleMode();
};

#endif // PRIORITY_QUEUE_H
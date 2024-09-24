#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Pair.h"
#include "Point.h"
#include <string>
#include "Address.h"
#include <iostream>

/**
 * @brief Função de hash que converte uma string em um valor numérico.
 *
 * Esta função aplica uma combinação de multiplicação por um número primo 
 * e a posição de cada caractere na string para gerar um valor de hash.
 *
 * @param key A chave do tipo string que será convertida em hash.
 * @param tableSize O tamanho da tabela hash para ajustar o valor gerado.
 * @return O valor de hash ajustado para o tamanho da tabela.
 */
unsigned int hashFunction(const std::string &key, int tableSize)
{
    unsigned long long hash = 0;
    unsigned long long prime = 53; // Número primo usado como base
    int keyLength = key.length();

    for (int i = 0; i < keyLength; i++)
    {
        hash = hash * prime + static_cast<unsigned long long>(key[i]) * (i + 1);
    }

    return hash % tableSize;
}

/**
 * @class HashNode
 * @brief Estrutura de nó para armazenar pares chave-valor na tabela hash.
 *
 * @tparam K Tipo da chave.
 * @tparam V Tipo do valor.
 */
template <typename K, typename V>
struct HashNode
{
    K key;  ///< Chave do tipo genérico K.
    V value;  ///< Valor associado à chave do tipo genérico V.
    HashNode *next;  ///< Ponteiro para o próximo nó em caso de colisão.

    /**
     * @brief Construtor da estrutura HashNode.
     *
     * @param k A chave a ser armazenada.
     * @param v O valor a ser armazenado.
     */
    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

/**
 * @class HashTable
 * @brief Implementação de uma tabela hash usando listas encadeadas para colisões.
 *
 * Esta classe permite operações eficientes de inserção e busca de pares chave-valor.
 *
 * @tparam K Tipo da chave.
 * @tparam V Tipo do valor.
 */
template <typename K, typename V>
class HashTable
{
private:
    HashNode<K, V *> **table;  ///< Ponteiro para um array de ponteiros de nós.
    int tableSize;  ///< Tamanho da tabela hash.

public:
    /**
     * @brief Construtor da tabela hash.
     *
     * Inicializa a tabela hash com o tamanho especificado, alocando memória
     * e definindo todos os ponteiros como nulos.
     *
     * @param size Tamanho da tabela hash.
     */
    HashTable(int size) : tableSize(size)
    {
        table = new HashNode<K, V *> *[tableSize];
        for (int i = 0; i < tableSize; i++)
        {
            table[i] = nullptr;
        }
    }

    /**
     * @brief Destrutor da tabela hash.
     *
     * Libera a memória alocada para a tabela hash e para todos os nós armazenados na tabela.
     */
    ~HashTable()
    {
        for (int i = 0; i < tableSize; i++)
        {
            HashNode<K, V *> *entry = table[i];
            while (entry != nullptr)
            {
                HashNode<K, V *> *prev = entry;
                entry = entry->next;
                delete prev->value;  ///< Libera o valor apontado pelo nó.
                delete prev;  ///< Libera o nó em si.
            }
            table[i] = nullptr;
        }
        delete[] table;  ///< Libera o array de ponteiros.
    }

    /**
     * @brief Insere um par chave-valor na tabela hash.
     *
     * Se a chave já existir na tabela, o valor associado a ela é atualizado.
     *
     * @param key A chave a ser inserida na tabela.
     * @param value O valor associado à chave.
     */
    void insert(const K &key, V *value)
    {
        int hashValue = hashFunction(key, tableSize);  ///< Calcula o valor de hash.
        HashNode<K, V *> *prev = nullptr;
        HashNode<K, V *> *entry = table[hashValue];

        // Percorre a lista encadeada na posição de hash até encontrar a chave ou o final da lista.
        while (entry != nullptr && entry->key != key)
        {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr)
        {
            // Se a chave não existir, cria um novo nó.
            entry = new HashNode<K, V *>(key, value);
            if (prev == nullptr)
            {
                table[hashValue] = entry;  ///< Insere o nó na tabela.
            }
            else
            {
                prev->next = entry;  ///< Insere o nó na lista encadeada.
            }
        }
        else
        {
            // Se a chave já existir, atualiza o valor.
            entry->value = value;
        }
    }

    /**
     * @brief Busca uma chave na tabela hash e retorna o valor associado a ela.
     *
     * @param key A chave a ser buscada na tabela.
     * @return O valor associado à chave, ou nullptr se a chave não for encontrada.
     */
    V *search(const K &key)
    {
        int hashValue = hashFunction(key, tableSize);  ///< Calcula o valor de hash.
        HashNode<K, V *> *entry = table[hashValue];

        // Percorre a lista encadeada na posição de hash.
        while (entry != nullptr)
        {
            // Se encontrar a chave, retorna o valor associado.
            if (entry->key == key)
            {
                return entry->value;
            }

            entry = entry->next;  ///< Avança para o próximo nó.
        }

        return nullptr;  ///< Retorna nullptr se a chave não for encontrada.
    }
};

// Instanciação da classe HashTable para o tipo específico de chave e valor.
template class HashTable<std::string, AddressInfo>;

#endif // HASHTABLE_H
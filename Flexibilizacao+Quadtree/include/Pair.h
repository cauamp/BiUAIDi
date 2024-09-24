#ifndef PAIR_H
#define PAIR_H
#include "Point.h"

/**
 * @class Pair
 * @brief Template genérico para representar um par de valores.
 *
 * Esta classe template pode armazenar dois valores de tipos diferentes ou iguais. 
 * Fornece operadores de comparação para facilitar a ordenação e a comparação entre pares.
 *
 * @tparam T1 Tipo do primeiro elemento do par.
 * @tparam T2 Tipo do segundo elemento do par.
 */
template <typename T1, typename T2>
class Pair {
public:
    /**
     * @brief Construtor padrão que inicializa ambos os elementos com seus respectivos valores padrão.
     */
    Pair() : first(T1()), second(T2()) {}

    /**
     * @brief Construtor que inicializa o par com valores específicos.
     * @param first Valor do primeiro elemento.
     * @param second Valor do segundo elemento.
     */
    Pair(const T1 &first, const T2 &second) : first(first), second(second) {}

    /**
     * @brief Retorna o primeiro elemento do par.
     * @return O primeiro elemento do par.
     */
    T1 getFirst() const {
        return first;
    }

    /**
     * @brief Retorna o segundo elemento do par.
     * @return O segundo elemento do par.
     */
    T2 getSecond() const {
        return second;
    }

    /**
     * @brief Define o valor do primeiro elemento do par.
     * @param value Novo valor para o primeiro elemento.
     */
    void setFirst(const T1 &value) {
        first = value;
    }

    /**
     * @brief Define o valor do segundo elemento do par.
     * @param value Novo valor para o segundo elemento.
     */
    void setSecond(const T2 &value) {
        second = value;
    }

    /**
     * @brief Operador de igualdade para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se os pares forem iguais, false caso contrário.
     */
    bool operator==(const Pair &outro) const {
        return first == outro.first && second == outro.second;
    }

    /**
     * @brief Operador de desigualdade para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se os pares forem diferentes, false caso contrário.
     */
    bool operator!=(const Pair &outro) const {
        return !(*this == outro);
    }

    /**
     * @brief Operador de menor que para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se o primeiro elemento deste par for menor que o do outro par, false caso contrário.
     */
    bool operator<(const Pair &outro) const {
        return first < outro.first;
    }

    /**
     * @brief Operador de maior que para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se o primeiro elemento deste par for maior que o do outro par, false caso contrário.
     */
    bool operator>(const Pair &outro) const {
        return first > outro.first;
    }

    /**
     * @brief Operador de menor ou igual para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se o primeiro elemento deste par for menor ou igual ao do outro par, false caso contrário.
     */
    bool operator<=(const Pair &outro) const {
        return !(*this > outro);
    }

    /**
     * @brief Operador de maior ou igual para comparação entre pares.
     * @param outro Par a ser comparado.
     * @return true se o primeiro elemento deste par for maior ou igual ao do outro par, false caso contrário.
     */
    bool operator>=(const Pair &outro) const {
        return !(*this < outro);
    }

private:
    T1 first;  ///< Primeiro elemento do par.
    T2 second; ///< Segundo elemento do par.
};

// Instanciações explícitas do template para alguns tipos comuns
template class Pair<double, Point>;
template class Pair<double, long>;
template class Pair<double, int>;

#endif // PAIR_H

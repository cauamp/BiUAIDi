#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <string>

/**
 * @class Point
 * @brief Representa um ponto em um plano 2D com coordenadas (x, y), um identificador e um status de ativação.
 *
 * Esta classe fornece funcionalidades básicas para manipulação de pontos em um espaço bidimensional, 
 * incluindo cálculo de distância, ativação/desativação do ponto e comparação de igualdade.
 */
class Point
{
private:
    double _x, _y;  ///< Coordenadas X e Y do ponto.
    std::string _id;        ///< Identificador único do ponto.
    bool _active;   ///< Status de ativação do ponto.

public:
    /**
     * @brief Construtor padrão que inicializa o ponto com coordenadas (x, y), id e status de ativação.
     * @param x Coordenada X do ponto.
     * @param y Coordenada Y do ponto.
     * @param id Identificador do ponto.
     * @param active Status de ativação do ponto.
     */
    Point(double x = 0, double y = 0, std::string id = "", bool active = true) : _x(x), _y(y), _id(id), _active(active) {}

    /**
     * @brief Ativa o ponto, definindo o status de ativação como verdadeiro.
     */
    void activate()
    {
        _active = true;
    }

    /**
     * @brief Desativa o ponto, definindo o status de ativação como falso.
     */
    void deactivate()
    {
        _active = false;
    }

    /**
     * @brief Verifica se o ponto está ativo.
     * @return true se o ponto estiver ativo, false caso contrário.
     */
    bool isActive() const
    {
        return _active;
    }

    /**
     * @brief Calcula a distância euclidiana entre este ponto e outro ponto.
     * @param outro O outro ponto com o qual calcular a distância.
     * @return A distância euclidiana entre os dois pontos.
     */
    double distance(const Point &outro) const
    {
        double dx = _x - outro._x;
        double dy = _y - outro._y;
        return sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief Operador de igualdade para comparação entre dois pontos.
     * @param outro O ponto a ser comparado.
     * @return true se as coordenadas dos pontos forem iguais, false caso contrário.
     */
    bool operator==(const Point &outro) const
    {
        return _x == outro._x && _y == outro._y;
    }

    /**
     * @brief Retorna o identificador do ponto.
     * @return O identificador do ponto.
     */
    std::string getId() const
    {
        return _id;
    }

    /**
     * @brief Retorna a coordenada X do ponto.
     * @return A coordenada X do ponto.
     */
    double getX() const
    {
        return _x;
    }

    /**
     * @brief Retorna a coordenada Y do ponto.
     * @return A coordenada Y do ponto.
     */
    double getY() const
    {
        return _y;
    }
};

#endif // POINT_H

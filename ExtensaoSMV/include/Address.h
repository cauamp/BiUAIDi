#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <ostream>

/**
 * @file Address.h
 * @brief Definição da estrutura AddressInfo e suas funções relacionadas.
 * 
 * Este arquivo contém a definição da estrutura AddressInfo, que representa informações de um endereço.
 * A estrutura contém campos como ponto de referência, identificação, tipo, logradouro, número, bairro, região e CEP.
 * Também são fornecidas funções para ativar e desativar um endereço, além de um operador de inserção para impressão.
 */
struct AddressInfo
{
    Point* _ponto = nullptr;
    std::string _idend;
    long _id_logrado;
    std::string _sigla_tipo;
    std::string _nome_logra;
    std::string _numero_imo;
    std::string _nome_bairr;
    std::string _nome_regio;
    int _cep;
    bool _ativo = true;

    AddressInfo() = default;

    AddressInfo(Point &ponto, const std::string &idend, long id_logrado, const std::string &sigla_tipo,
                const std::string &nome_logra, const std::string &numero_imo, const std::string &nome_bairr,
                const std::string &nome_regio, int cep)
        : _ponto(&ponto), _idend(idend), _id_logrado(id_logrado), _sigla_tipo(sigla_tipo), _nome_logra(nome_logra),
          _numero_imo(numero_imo), _nome_bairr(nome_bairr), _nome_regio(nome_regio), _cep(cep) {}

    void activate()
    {
        _ativo = true;
        _ponto->activate();
    }
    void deactivate()
    {
        _ativo = false;
        _ponto->deactivate();
    }
};

std::ostream &operator<<(std::ostream &os, const AddressInfo &address)
{
    os << address._sigla_tipo << " " << address._nome_logra << ", "
       << address._numero_imo << ", " << address._nome_bairr << ", "
       << address._nome_regio << ", " << address._cep;
    return os;
}

#endif // ADDRESS_H

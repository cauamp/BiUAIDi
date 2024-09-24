/**
 * @file SMV.h
 * @brief Arquivo de cabeçalho para a classe SMV.
 *
 * Este arquivo contém a definição da classe SMV, que é responsável por gerenciar páginas de memória virtual.
 * A classe SMV implementa um sistema de memória virtual com proteção de leitura e escrita, validação de dados em memória física e cache, e suporte a memória secundária.
 */

#ifndef SMV_H
#define SMV_H

#define NOPROT 0    /* não protegido */
#define READPROT 1  /* protegido para leitura */
#define WRITEPROT 2 /* protegido para escrita */
#define VALID 4     /* dados na memória física (RAM) */
#define DIRTY 8     /* dados no cache, a página foi escrita */
#define AVAIL 16    /* dados no cache, protegidos para leitura/escrita */
#define READ 32     /* dados no cache, protegidos para escrita */
#define DISCO 64    /* dados na memória secundária */

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <ucontext.h>
#include <cerrno>
#include <cmath>
#include <random>
#include <chrono>

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

#define NUMPAGE 10

#ifndef CUSTOM_MEMTOSWAPRATIO
#ifndef MEMTOSWAPRATIO
#define MEMTOSWAPRATIO 0.5
#endif
#endif

/**
 * @class SMVPage
 * @brief Classe que representa uma página de memória virtual.
 *
 * A classe SMVPage armazena informações sobre o status da página, endereço lógico e físico,
 * além de estatísticas de acesso, como o número de acessos, vezes que ficou válida, suja ou foi para o disco.
 */
class SMVPage
{
public:
    int status;     /**< Status da página, que pode incluir proteções, validade e disponibilidade */
    void *logaddr;  /**< Endereço lógico da página */
    void *physaddr; /**< Endereço físico da página */
    int nacc = 0;   /**< Número de acessos à página */
    int nvalid = 0; /**< Número de vezes que a página se tornou válida */
    int ndirty = 0; /**< Número de vezes que a página ficou suja */
    int navail = 0; /**< Número de vezes que a página ficou disponível */
    int nread = 0;  /**< Número de vezes que a leitura foi permitida */
    int ndisk = 0;  /**< Número de vezes que a página foi escrita no disco */
    unsigned long lastAccessTime;
};

/**
 * @class SMV
 * @brief Classe responsável por gerenciar o sistema de memória virtual.
 *
 * A classe SMV implementa funcionalidades necessárias para inicializar e finalizar páginas de memória,
 * além de tratar exceções de segmentação. Também gerencia o vetor de páginas, controlando o acesso e a troca
 * de páginas entre a memória principal e a memória secundária utilizando a política LRU.
 */
class SMV
{
public:
    /**
     * @brief Método estático para obter a instância única da classe SMV.
     *
     * @return Ponteiro para a instância única da classe SMV.
     */
    static SMV &getInstance()
    {
        static SMV instance; // Instância única criada de maneira segura e destruída apenas uma vez.
        return instance;
    }

    /**
     * @brief Inicializa uma página de memória.
     *
     * @param bytesAllocated Retorna o número de bytes alocados.
     * @return Retorna o endereço lógico da página alocada.
     */
    char *initPage(int &bytesAllocated);

    /**
     * @brief Finaliza o sistema de memória virtual.
     *
     * Libera as páginas alocadas e encerra o swap.
     */
    void endPage();

    /**
     * @brief Obtém a razão entre a memória principal e a memória secundária.
     *
     * Este método retorna o valor atual da razão entre a memória principal e a memória
     * secundária usada pelo sistema de memória virtual.
     *
     * @return Retorna o valor da razão entre memória principal e secundária.
     */
    static double getMemToSwapRatio();

    /**
     * @brief Define a razão entre a memória principal e a memória secundária.
     *
     * Este método define o valor da razão entre a memória principal e a memória secundária
     * usada pelo sistema de memória virtual. A definição da razão deve ocorrer antes da
     * inicialização do sistema de memória.
     *
     * @param ratio Valor da nova razão entre memória principal e memória secundária.
     */
    static void setMemToSwapRatio(double ratio);

private:
    /**
     * @brief Construtor da classe SMV.
     *
     * Inicializa o sistema de memória virtual e define os parâmetros iniciais.
     */
    SMV();

    /**
     * @brief Destrutor da classe SMV.
     *
     * Responsável por finalizar o sistema de memória virtual, liberando os recursos alocados.
     */
    ~SMV();

    /**
     * @brief Manipulador de exceção de segmentação.
     *
     * Trata as exceções de segmentação (SIGSEGV) e realiza o gerenciamento de páginas.
     *
     * @param sig Número do sinal.
     * @param sip Informações sobre o sinal.
     * @param uap Contexto da exceção.
     */
    void segvHandler(int sig, siginfo_t *sip, ucontext_t *uap);

    /**
     * @brief Instala o manipulador de sinal.
     *
     * Define o manipulador para capturar e tratar exceções de segmentação.
     */
    void installSignalHandler();

    /**
     * @brief Manipulador estático de exceção de segmentação.
     *
     * Função estática para capturar exceções de segmentação em contexto externo.
     *
     * @param sig Número do sinal.
     * @param sip Informações sobre o sinal.
     * @param context Contexto da exceção.
     */
    static void handleSegv(int sig, siginfo_t *sip, void *context);

    SMVPage pvet[NUMPAGE];         /**< Vetor de páginas de memória virtual */
    char *raw_physpage = nullptr;  /**< Endereço da memória física bruta */
    char *raw_logpage = nullptr;   /**< Endereço da memória lógica bruta */
    char *physpage = nullptr;      /**< Endereço da memória física */
    char *logpage = nullptr;       /**< Endereço da memória lógica */
    int swap = -1;                 /**< Descritor de arquivo para swap */
    int pagesInMemory = 0;         /**< Número de páginas atualmente na memória */
    static SMV *instance;          /**< Instância única da classe SMV */
    static double _MEMTOSWAPRATIO; /**< Razão entre memória principal e memória secundária */

    // Previne a cópia e a atribuição
    SMV(const SMV &) = delete;
    SMV &operator=(const SMV &) = delete;
};

#endif

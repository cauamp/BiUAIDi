#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include "QuadTree.h"
#include "Address.h"
#include "HashTable.h"

HashTable<std::string, AddressInfo> *loadFile(std::ifstream &inputFile, int NumEnderecos, QuadTree &quadTree)
{

    HashTable<std::string, AddressInfo> *estacoes = new HashTable<std::string, AddressInfo>(NumEnderecos);

    std::string line;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string idend, sid_logrado, sigla_tipo, nome_logra, numero_imo, nome_bairr, nome_regio, scep, sx, sy;
        double x, y;
        int cep;
        long id_logradouro;

        if (std::getline(iss, idend, ';') &&
            std::getline(iss, sid_logrado, ';') &&
            std::getline(iss, sigla_tipo, ';') &&
            std::getline(iss, nome_logra, ';') &&
            std::getline(iss, numero_imo, ';') &&
            std::getline(iss, nome_bairr, ';') &&
            std::getline(iss, nome_regio, ';') &&
            std::getline(iss, scep, ';') &&
            std::getline(iss, sx, ';') &&
            std::getline(iss, sy, ';'))
        {
            id_logradouro = std::stol(sid_logrado);
            cep = std::stoi(scep);
            x = std::stod(sx);
            y = std::stod(sy);

            Point *ponto = new Point(x, y, idend);
            AddressInfo *estacao = new AddressInfo(*ponto, idend, id_logradouro, sigla_tipo, nome_logra, numero_imo, nome_bairr, nome_regio, cep);
            quadTree.insert(*ponto);
            estacoes->insert(idend, estacao);
        }
    }
    inputFile.close();

    return estacoes;
}

void consultar(QuadTree &quadTree, HashTable<std::string, AddressInfo> &estacoes, double x, double y, int n)
{

    Point p(x, y);
    PriorityQueue<Pair<double, Point>> pq(n);
    quadTree.KNNSearch(p, n, pq);

    pq.toggleMode();
    while (!pq.empty())
    {
        Pair<double, Point> p = pq.top();
        pq.pop();

        double dist = p.getFirst();
        std::string id = p.getSecond().getId();
        AddressInfo *estacao = estacoes.search(id);

        if (estacao != nullptr)
        {
            std::cout << *estacao << std::fixed << std::setprecision(3) << " (" << dist << ")" << std::endl;
        }
    }
}

void ativar(QuadTree &quadTree, HashTable<std::string, AddressInfo> &estacoes, int numEnderecos, std::string id)
{
    AddressInfo *estacao = estacoes.search(id);
    if (estacao == nullptr)
    {
        std::cout << "Ponto de recarga " << id << " não encontrado." << std::endl;
        return;
    }
    if (!estacao->_ativo)
    {
        estacao->activate();
        std::cout << "Ponto de recarga " << id << " ativado." << std::endl;
    }
    else
    {
        std::cout << "Ponto de recarga " << id << " já estava ativo." << std::endl;
    }
}

void desativar(QuadTree &quadTree, HashTable<std::string, AddressInfo> &estacoes, int numEnderecos, std::string id)
{
    AddressInfo *estacao = estacoes.search(id);
    if (estacao == nullptr)
    {
        std::cout << "Ponto de recarga " << id << " não encontrado." << std::endl;
        return;
    }
    if (estacao->_ativo)
    {
        estacao->deactivate();
        std::cout << "Ponto de recarga " << id << " desativado." << std::endl;
    }
    else
    {
        std::cout << "Ponto de recarga " << id << " já estava desativado." << std::endl;
    }
}

int main(int argc, char *argv[])
{
    bool tFlag = false; // Variável booleana para verificar o codigo esta no modo de teste

    // Verifica se o número de argumentos é suficiente (mínimo de 5, sem contar o -t)
    if (argc < 5 || argc > 8)
    {
        std::cerr << "Uso: ./tp3.out -b <arquivo_base> -e <arquivo_eventos> [-t] [MEMTOSWAPRATIO]" << std::endl;
        return 1;
    }

    std::string genFilePath, inputFilePath;
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-b" && (i + 1) < argc)
        {
            genFilePath = argv[++i];
        }
        else if (arg == "-e" && (i + 1) < argc)
        {
            inputFilePath = argv[++i];
        }
        else if (arg == "-t")
        {
            tFlag = true;
            if (argc == 7)
            {
                std::string prop = argv[++i];
                SMV::setMemToSwapRatio(std::stod(prop)); // Atualiza a razão com o valor fornecido
            }
        }
        else
        {
            std::cerr << "Parâmetro inválido: " << arg << std::endl;
            return 1;
        }
    }


    std::string line;
    std::ifstream genFile(genFilePath);

    int numEnderecos;

    if (std::getline(genFile, line))
    {
        std::istringstream iss(line);
        iss >> numEnderecos;
    }

    QuadTree quadTree(numEnderecos, Rectangle(Point(150000, 7500000), Point(7500000, 10000000)));

    HashTable<std::string, AddressInfo> *estacoes = loadFile(genFile, numEnderecos, quadTree);

    std::ifstream inputFile(inputFilePath);
    int numInputs;
    if (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        iss >> numInputs;
    }

    if (tFlag)
    {
        std::cout << "INITIALIZED" << std::endl;
    }

    while (std::getline(inputFile, line))
    {
        std::cout << line << std::endl;
        char command = line[0];
        std::istringstream iss(line.substr(1));

        if (command == 'C')
        {

            double x, y;
            int n;
            iss >> x >> y >> n;
            consultar(quadTree, *estacoes, x, y, n);
        }
        else if (command == 'A')
        {
            std::string id;
            iss >> id;
            ativar(quadTree, *estacoes, numEnderecos, id);
        }
        else if (command == 'D')
        {
            std::string id;
            iss >> id;
            desativar(quadTree, *estacoes, numEnderecos, id);
        }
    }

    if (tFlag)
    {
        std::cout << "FINISHED" << std::endl;
    }

    inputFile.close();
    estacoes->~HashTable();
    quadTree.~QuadTree();

    return 0;
}

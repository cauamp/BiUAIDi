#include "SMV.h"

SMV *SMV::instance = nullptr;

double SMV::_MEMTOSWAPRATIO = MEMTOSWAPRATIO;

SMV::SMV()
{
    instance = this;
    std::cout << "MEMTOSWAPRATIO: " << _MEMTOSWAPRATIO << std::endl;
}

SMV::~SMV()
{
    endPage();
    instance = nullptr;
}

char *SMV::initPage(int &bytesAllocated)
{
    std::cout << "Initializing SMV" << std::endl;
    installSignalHandler();

    raw_physpage = new char[NUMPAGE * PAGESIZE + PAGESIZE - 1];
    raw_logpage = new char[NUMPAGE * PAGESIZE + PAGESIZE - 1];

    if (!raw_physpage || !raw_logpage)
    {
        throw std::runtime_error("Memory allocation failed");
    }

    std::cout << "Memory allocated: ";
    bytesAllocated = NUMPAGE * PAGESIZE;
    std::cout << bytesAllocated << " Bytes" << std::endl;
    physpage = reinterpret_cast<char *>((reinterpret_cast<long>(raw_physpage) + PAGESIZE - 1) & ~(PAGESIZE - 1));
    logpage = reinterpret_cast<char *>((reinterpret_cast<long>(raw_logpage) + PAGESIZE - 1) & ~(PAGESIZE - 1));

    std::cout << "Creating swap file" << std::endl;
    char swapname[30];
    sprintf(swapname, "./smvdat/smvswap.%d", getpid());
    swap = open(swapname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    std::cout << "Swap file created" << std::endl;

    char pagenull[PAGESIZE] = {0};

    for (int i = 0; i < NUMPAGE; i++)
    {
        pvet[i].status = DISCO;
        pvet[i].logaddr = logpage + i * PAGESIZE;
        pvet[i].physaddr = physpage + i * PAGESIZE;
        write(swap, pagenull, PAGESIZE);
    }
    std::cout << "Swap file initialized" << std::endl;
    if (mprotect(logpage, NUMPAGE * PAGESIZE, PROT_NONE))
    {
        throw std::runtime_error("mprotect failed on logpage");
    }
    std::cout << "SMV initialized" << std::endl;
    return logpage;
}

void SMV::endPage()
{

    for (int j = 0; j < NUMPAGE; j++)
    {
        if (pvet[j].nacc)
        {
            std::cout << "Page " << j << ": acc " << pvet[j].nacc
                      << " val " << pvet[j].nvalid
                      << " drt " << pvet[j].ndirty
                      << " ava " << pvet[j].navail
                      << " rd " << pvet[j].nread
                      << " dsk " << pvet[j].ndisk << std::endl;
        }
        if (pvet[j].status & VALID)
        {
            lseek(swap, j * PAGESIZE, SEEK_SET);
            write(swap, pvet[j].physaddr, PAGESIZE);
            pvet[j].status &= ~VALID;
            pvet[j].status |= DISCO;
            pagesInMemory--;
        }
    }
    close(swap);
    delete[] raw_physpage;
    delete[] raw_logpage;
}

void SMV::installSignalHandler()
{
    std::cout << "Installing signal handler" << std::endl;
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handleSegv;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGALRM);
    sigaddset(&sa.sa_mask, SIGIO);
    if (sigaction(SIGSEGV, &sa, nullptr) == -1)
    {
        std::cerr << "Failed to install signal handler for SIGSEGV" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Signal handler installed" << std::endl;
}

void SMV::handleSegv(int sig, siginfo_t *sip, void *context)
{
    if (instance)
    {
        instance->segvHandler(sig, sip, reinterpret_cast<ucontext_t *>(context));
    }
}

void SMV::segvHandler(int sig, siginfo_t *sip, ucontext_t *uap)
{
    unsigned int i = (reinterpret_cast<caddr_t>(sip->si_addr) - logpage) / PAGESIZE;
    pvet[i].nacc++;
    pvet[i].lastAccessTime = std::chrono::system_clock::now().time_since_epoch().count(); // Atualiza o tempo de acesso

    for (int c = 0; c < NUMPAGE; c++)
    {
        if (pvet[c].status & DIRTY)
        {
            pvet[c].status &= ~DIRTY;
            pvet[c].status |= VALID;
            memcpy(pvet[c].physaddr, pvet[c].logaddr, PAGESIZE);
        }
    }

    if (mprotect(logpage, NUMPAGE * PAGESIZE, PROT_NONE))
    {
        throw std::runtime_error("mprotect failed");
    }

    if (pvet[i].status & DISCO)
    {
        // Se a página atual está marcada como "DISCO" (não válida na memória), precisamos processá-la para substituição

        // Verifica se a memória em uso excedeu a capacidade permitida pela razão de memória
        if (pagesInMemory + 1 > NUMPAGE * _MEMTOSWAPRATIO)
        {
            // Encontrar a página menos recentemente usada (LRU)
            unsigned long oldestAccessTime = std::chrono::system_clock::now().time_since_epoch().count();
            int discard = -1;

            // Itera sobre todas as páginas para encontrar a menos recentemente usada
            for (int j = 0; j < NUMPAGE; j++)
            {
                // Verifica se a página é válida e se seu tempo de acesso é o mais antigo encontrado até agora
                if (pvet[j].status & VALID && pvet[j].lastAccessTime < oldestAccessTime)
                {
                    oldestAccessTime = pvet[j].lastAccessTime;
                    discard = j; // Armazena o índice da página a ser descartada
                }
            }

            // Se uma página a ser descartada foi encontrada
            if (discard != -1)
            {
                // Move o ponteiro para o início da área de troca (swap) da página a ser descartada
                lseek(swap, discard * PAGESIZE, SEEK_SET);
                // Grava a página na área de troca
                write(swap, pvet[discard].physaddr, PAGESIZE);
                // Atualiza o status da página: remove o status VALID e adiciona DISCO
                pvet[discard].status &= ~VALID;
                pvet[discard].status |= DISCO;
                // Reduz o número de páginas na memória
                pagesInMemory--;
                std::cout << "Page " << discard << " swapped out" << std::endl;
            }
        }

        // Atualiza a página atual para o status VALID
        pvet[i].status &= ~DISCO;
        pvet[i].status |= VALID;
        // Move o ponteiro para o início da área de troca da página atual
        lseek(swap, i * PAGESIZE, SEEK_SET);
        // Lê a página da área de troca para a memória física
        read(swap, pvet[i].physaddr, PAGESIZE);
        // Aumenta o número de páginas na memória
        pagesInMemory++;
    }
    else if (pvet[i].status & VALID)
    {
        pvet[i].status &= ~VALID;
        pvet[i].status |= AVAIL;
        if (mprotect(pvet[i].logaddr, PAGESIZE, PROT_READ | PROT_WRITE))
        {
            throw std::runtime_error("mprotect failed");
        }
        memcpy(pvet[i].logaddr, pvet[i].physaddr, PAGESIZE);
        if (mprotect(pvet[i].logaddr, PAGESIZE, PROT_NONE))
        {
            throw std::runtime_error("mprotect failed");
        }
    }
    else if (pvet[i].status & AVAIL)
    {
        pvet[i].status &= ~AVAIL;
        pvet[i].status |= READ;
        if (mprotect(pvet[i].logaddr, PAGESIZE, PROT_READ))
        {
            throw std::runtime_error("mprotect failed");
        }
    }
    else if (pvet[i].status & READ)
    {
        pvet[i].status &= ~READ;
        pvet[i].status |= DIRTY;
        if (mprotect(pvet[i].logaddr, PAGESIZE, PROT_READ | PROT_WRITE))
        {
            throw std::runtime_error("mprotect failed");
        }
    }
}

double SMV::getMemToSwapRatio()
{
    return _MEMTOSWAPRATIO;
}

void SMV::setMemToSwapRatio(double ratio)
{
#ifdef CUSTOMMEMTOSWAPRATIO
    std::cerr << "MEMTOSWAPRATIO can`t be redefined" << std::endl;
#endif
#ifndef CUSTOMMEMTOSWAPRATIO
    if (instance != nullptr)
    {
        std::cerr << "MEMTOSWAPRATIO must be defined before inicialization" << std::endl;
    }
    _MEMTOSWAPRATIO = ratio;
#endif
}
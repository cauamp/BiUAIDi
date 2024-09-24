# Estações de Recarga da BiUAIDi

**Disciplina:** DCC205 – Estruturas de Dados  
**Período:** 2024/1  
**Professores:** Wagner Meira Jr. / Eder Ferreira de Figueiredo  
**Trabalho Prático 3 (TP3)**  

## Descrição

Este projeto implementa uma aplicação para a fabricante de carros elétricos BiUAIDi, que visa identificar as estações de recarga mais próximas de uma posição geográfica fornecida, utilizando estruturas de dados eficientes para realizar consultas, ativações e desativações de estações.
[Enunciado Completo](./Enunciado.pdf)

### Funcionalidades do Sistema
[Relatorio Inicial](./Flexibilizacao+Quadtree/RelatorioInicial.pdf)

1. **Flexibilização do aplicativo**: O aplicativo foi modificado para trabalhar com um conjunto dinâmico de estações de recarga. O sistema agora suporta a ativação e desativação de estações e realiza consultas a partir de arquivos de entrada.
   
2. **Uso de QuadTree**: Foi implementada a estrutura de dados QuadTree para substituir o vetor original, permitindo consultas sub-lineares sobre as estações mais próximas. A QuadTree utiliza alocação vetorial, com apontadores entre nós representados como índices do vetor.

3. **Avaliação Experimental**: A implementação foi avaliada comparativamente com a versão anterior baseada em vetor, medindo o desempenho em termos de tempo de execução e localidade de referência, incluindo consultas e atualizações de estações.

### Extensões Implementadas
[Relatorio Adicional](./ExtensaoSMV/RelatorioSMV.pdf)

4. **Arquitetura de Dois Níveis**: A aplicação foi estendida para funcionar em uma arquitetura de dois níveis, simulando um ambiente com memória limitada, replicando dados conforme necessário e implementando uma política de substituição de páginas.

5. **Avaliação Experimental Estendida**: O aplicativo estendido foi avaliado em termos de latência de consultas e volume de dados transferidos entre níveis de memória.

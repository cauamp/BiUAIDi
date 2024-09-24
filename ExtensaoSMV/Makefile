# Compilador e opções
CXX = g++
CXXFLAGS = -Wall -std=c++11
INCLUDES = -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
BIN = $(BIN_DIR)/tp3.out

# Regra padrão
all: $(BIN)

# Regra para criar o executável
$(BIN): $(OBJ)
	@mkdir -p $(BIN_DIR)  # Garante que o diretório de binários existe
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Regra para compilar arquivos .cpp em arquivos .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Garante que o diretório de objetos existe
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -f $(OBJ)       # Remove arquivos objeto
	rm -f $(BIN)       # Remove o executável
	rm -rf $(OBJ_DIR)  # Remove o diretório de objetos
	rm -rf $(BIN_DIR)  # Remove o diretório de binários (se existir)

# Dependência para o comando definido no enunciado
allsubmetido: all

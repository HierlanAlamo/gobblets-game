# Compilador
CXX = g++
CXXFLAGS = -Wall -O2

# Arquivos fonte
SRC = main.cpp render.cpp board.cpp bot.cpp object.cpp cell.cpp piece.cpp

# Nome do executável
OUT = GobbletsGame

# Bibliotecas SDL2
LIBS = -lSDL2 -lSDL2_mixer

# Regra padrão
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(SRC) -o $(OUT) $(LIBS)

# Limpar binários
clean:
	rm -f $(OUT)

#include <SFML/Graphics.hpp>

using namespace sf;

bool verifyPosition(Vector2f position, std::vector<Sprite>& colocadas);

bool verifyBlocks(std::vector<Sprite>& sprites, char condition, std::vector<Sprite>& colocadas, int max);

void verificarBlocosColocados(std::vector<Sprite>& blocos, std::string& pontuacao);

void deletarCamada(std::vector<Sprite>& blocos, int& camada);

void moverCamada(std::vector<Sprite>& blocos, char direcao, int nBlocos, int& camadaMin);
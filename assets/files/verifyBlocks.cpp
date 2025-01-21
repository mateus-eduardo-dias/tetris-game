#include "verifyBlocks.hpp"
#include <iostream>
#include <map>

bool verifyBlocks(std::vector<Sprite>& sprites, char condition, std::vector<Sprite>& colocadas, int max) {
    if (condition == 'd') {
        for (int i = 0; i < sprites.size(); i++) {
            Vector2f alvo = sprites[i].getPosition() + Vector2f(0.f, 32.f);
            if (alvo.y > max) return false;
            for (int j = 0; j < colocadas.size(); j++) {
                if (colocadas[j].getGlobalBounds().contains(alvo)) {
                    return false;
                }
            }
        }
    } else if (condition == 'l') {
        for (int i = 0; i < sprites.size(); i++) {
            Vector2f alvo = sprites[i].getPosition() + Vector2f(-32.f, 0.f);
            if (alvo.x < max) return false;
            for (int j = 0; j < colocadas.size(); j++) {
                if (colocadas[j].getGlobalBounds().contains(alvo)) {
                    return false;
                }
            }
        }
    } else if (condition == 'r') {
        for (int i = 0; i < sprites.size(); i++) {
            Vector2f alvo = sprites[i].getPosition() + Vector2f(32.f, 0.f);
            if (alvo.x > max) return false;
            for (int j = 0; j < colocadas.size(); j++) {
                if (colocadas[j].getGlobalBounds().contains(alvo)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void verificarBlocosColocados(std::vector<Sprite>& blocos, std::string& pontuacao) {
    std::map<int, int> camadas;
    for (int i = 0; i < blocos.size(); i++) {
        int camada = blocos[i].getPosition().y;
        if (camadas.find(camada) == camadas.end()) {
            camadas[camada] = 1;
        } else {
            camadas[camada] ++;
            if (camadas[camada] == 10) {
                deletarCamada(blocos, camada);
                moverCamada(blocos, 'd', 1, camada);
                int temp_pontuacao = std::stoi(pontuacao);
                temp_pontuacao ++;
                pontuacao = std::to_string(temp_pontuacao);
                if (pontuacao.length() == 2) {
                    pontuacao = "0" + pontuacao;
                } else if (pontuacao.length() == 1) {
                    pontuacao = "00" + pontuacao;
                }
                camadas[camada] = 0;
            }
        }
    }
}

void deletarCamada(std::vector<Sprite>& blocos, int& camada) {
    std::vector<Sprite> listaTemporaria;
    for (int i = 0; i < blocos.size(); i++) {
        if (blocos[i].getPosition().y != camada) {
            listaTemporaria.push_back(blocos[i]);
        }
    }
    blocos.clear();
    blocos = listaTemporaria;
}

void moverCamada(std::vector<Sprite>& blocos, char direcao, int nBlocos, int& camadaMin) {
    if (direcao == 'd') {
        for (int i = 0; i < blocos.size(); i++) {
            if (blocos[i].getPosition().y < camadaMin) {
                blocos[i].move(Vector2f(0.f, float(32 * nBlocos)));
            }
        }
    }
}

bool verifyPosition(Vector2f position, std::vector<Sprite>& colocadas) {
    for (int i = 0; i < colocadas.size(); i++) {
        if (colocadas[i].getGlobalBounds().contains(position)) {
            return false;
        }
    }
    return true;
}
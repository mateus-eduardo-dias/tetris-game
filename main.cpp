#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "assets/files/makeBlock.hpp"
#include "assets/files/verifyBlocks.hpp"

using namespace sf;

void movAnim(Transformable& objeto, int& frame, int framesMax, Vector2f targetVal, bool& status) {
    int frames = framesMax - frame;
    Vector2f posicao = objeto.getPosition();
    float toAddX = 0;
    float toAddY = 0;
    if (posicao.x != targetVal.x) {
        toAddX = (targetVal.x - posicao.x) / frames;
    }
    if (posicao.y != targetVal.y) {
        toAddY = (targetVal.y - posicao.y) / frames;
    }
    else if (toAddX == 0 && toAddY == 0) {
        status = true;
        frame = 0;
    }
    objeto.setPosition(Vector2f(posicao.x + toAddX, posicao.y + toAddY));
    frame ++;
}
Color blinkAnim(Transformable& objeto, bool& blinkStatus, Color& cor, int speed) {
    if (blinkStatus) {
        if (speed + int(cor.a) > 255) {
            cor.a = uint8_t(255);
            blinkStatus = false;
        } else {
            cor.a += uint8_t(speed);
        }
    } else {
        if (int(cor.a) - speed < 0) {
            cor.a = uint8_t(0);
            blinkStatus = true;
        } else {
            cor.a -= uint8_t(speed);
        }
    }
    return cor;
}

Color changeColor(Transformable& objeto, std::vector<Color>& cores, int& corAtual) {
    if (corAtual + 1 >= cores.size()) corAtual = 0;
    else corAtual ++;

    return cores[corAtual];
}

int main(int argc, char* argv[]) {
    constexpr int WIN_SX = 1280;
    constexpr int WIN_SY = 720;
    constexpr int WIN_FPS = 60;
    const std::string WIN_TITLE = "Tetris";

    const std::string assets = std::filesystem::absolute(argv[0]).parent_path().string() + "\\assets" ;

    RenderWindow window(VideoMode({WIN_SX, WIN_SY}), WIN_TITLE, Style::Titlebar, State::Windowed);
    window.setFramerateLimit(WIN_FPS);

    Font tetrisFnt(assets + "\\fonts\\tetris.ttf");
    Font arialFnt(assets + "\\fonts\\arial.ttf");
    Font sPixelFnt(assets + "\\fonts\\SuperPixel-m2L8j.ttf");

    Text titulo(tetrisFnt, "Tetris", 80);
    int centroTextoX = titulo.getGlobalBounds().getCenter().x;
    int centroTargetX = WIN_SX/2;
    int centroTargetY = WIN_SY/2;
    titulo.setPosition(Vector2f(float(centroTargetX-centroTextoX), -80.f));
    bool titulo_ready = false;
    int titulo_iter = 0;

    std::vector<Color> tituloCores = {Color::White, Color::Blue, Color::Red, Color::Green, Color::Yellow};
    int tituloCorAtual = 0;


    Text altText(tetrisFnt, "PRESS ENTER TO START");
    int centroX = altText.getGlobalBounds().getCenter().x;
    altText.setPosition(Vector2f(float(centroTargetX-centroX), 350.f));
    Color altTextColor = Color{255,255,255,0};
    altText.setFillColor(altTextColor);
    bool altTextBlnkAnim = true;

    std::vector<Drawable*> objTelaInicial = {&titulo, &altText};

    // TELA READY

    Text readyTxt(tetrisFnt, "Ready", 100);
    centroX = readyTxt.getGlobalBounds().getCenter().x;
    int centroY = readyTxt.getGlobalBounds().getCenter().y;

    readyTxt.setPosition(Vector2f(float(centroTargetX - centroX), float(centroTargetY - centroY)));
    int readyTimer = 0;
    std::vector<Drawable*> objReady = {&readyTxt};

    // TELA JOGO

    RectangleShape bordaJogo;
    bordaJogo.setSize(Vector2f(320.f, 640.f)); // block size: 32x32; max 20 layers; max 10 columns
    bordaJogo.setPosition(Vector2f(475.f, 35.f));

    float XBlockMin = 475.f;
    float XBlockSpawn = 635.f;
    float XBlockMax = 763.f;

    float YBlockMin = 35.f;
    float& YBlockSpawn = YBlockMin;
    float YBlockMax = 674.f;

    Vector2f blockSpawn = Vector2f(XBlockSpawn, YBlockSpawn);

    bordaJogo.setFillColor(Color::Transparent);
    bordaJogo.setOutlineThickness(5.f);
    bordaJogo.setOutlineColor(Color::White);

    RectangleShape bordaNext;
    bordaNext.setSize(Vector2f(200.f, 250.f));
    bordaNext.setPosition(Vector2f(900.f, 35.f));

    bordaNext.setFillColor(Color::White);
    bordaNext.setOutlineThickness(2.f);
    bordaNext.setOutlineColor(Color::White);

    RectangleShape bordaNext2;
    bordaNext2.setSize(Vector2f(200.f, 75.f)); // colocar a shape (block) no Y 110 +
    bordaNext2.setPosition(Vector2f(900.f, 35.f));
    bordaNext2.setFillColor(Color::Black);

    Text nextTxt(sPixelFnt, "NEXT", 50);
    nextTxt.setPosition(Vector2f(float(bordaNext2.getGlobalBounds().getCenter().x - nextTxt.getGlobalBounds().getCenter().x), 40.f));
    nextTxt.setFillColor(Color::White);

    Vector2f nextBlockCenter = Vector2f(1000.f, 197.f);

    RectangleShape bordaPontuacao;
    bordaPontuacao.setSize(Vector2f(200.f, 100.f));
    bordaPontuacao.setPosition(Vector2f(900.f, 350.f));

    bordaPontuacao.setFillColor(Color::Transparent);
    bordaPontuacao.setOutlineThickness(2.f);
    bordaPontuacao.setOutlineColor(Color::White);

    std::string pontuacao = "000";
    Text pontuacaoTxt(tetrisFnt, pontuacao, 50);
    pontuacaoTxt.setPosition(bordaPontuacao.getGlobalBounds().getCenter() - pontuacaoTxt.getGlobalBounds().getCenter());

    Texture blockAqua(assets + "\\graphics\\sblocks\\aqua64.png");
    Texture blockBlue(assets + "\\graphics\\sblocks\\blue64.png");
    Texture blockGreen(assets + "\\graphics\\sblocks\\green64.png");
    Texture blockOrange(assets + "\\graphics\\sblocks\\orange64.png");
    Texture blockPurple(assets + "\\graphics\\sblocks\\purple64.png");
    Texture blockRed(assets + "\\graphics\\sblocks\\red64.png");
    Texture blockYellow(assets + "\\graphics\\sblocks\\yellow64.png");

    Texture blockI(assets + "\\graphics\\shapeblocks\\I.png");
    Texture blockJ(assets + "\\graphics\\shapeblocks\\J.png");
    Texture blockL(assets + "\\graphics\\shapeblocks\\L.png");
    Texture blockO(assets + "\\graphics\\shapeblocks\\O.png");
    Texture blockS(assets + "\\graphics\\shapeblocks\\S.png");
    Texture blockT(assets + "\\graphics\\shapeblocks\\T.png");
    Texture blockZ(assets + "\\graphics\\shapeblocks\\Z.png");

    std::vector<Texture> texturas = {blockAqua, blockBlue, blockGreen, blockOrange, blockPurple, blockRed, blockYellow};
    std::vector<Texture> texturasPre = {blockI, blockJ, blockL, blockO, blockS, blockT, blockZ};

    char lista[7] = {'i', 'j', 'l', 'o', 's', 't', 'z'};

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, texturas.size() - 1);

    int nEscolhido = dist(mt);
    std::vector<Texture*> texturasUsadas = {new Texture(texturas[nEscolhido])};
    char formato = lista[nEscolhido];
    makeSet ms;
    ms.createSet(*texturasUsadas[0], formato);

    nEscolhido = dist(mt);
    std::vector<Sprite*> proximoBloco = {new Sprite(makePreAsset(texturasPre[nEscolhido], nextBlockCenter))};
    char formatoProximoBloco = lista[nEscolhido];

    std::vector<Sprite> conjSpritesAtivos = ms.getSet();
    for (int i = 0; i < conjSpritesAtivos.size(); i++) {
        conjSpritesAtivos[i].setPosition(conjSpritesAtivos[i].getGlobalBounds().position + blockSpawn);
    }

    std::vector<Sprite> pecasColocadas = {};

    std::vector<Drawable*> objJogo = {&bordaJogo, &bordaNext, &bordaNext2, &nextTxt, &bordaPontuacao, &pontuacaoTxt};

    // TELA PAUSE

    Text avisoPause(tetrisFnt, "Jogo pausado", 50);
    avisoPause.setPosition(Vector2f(float(WIN_SX/2 - avisoPause.getGlobalBounds().getCenter().x), float(WIN_SY/2 - avisoPause.getGlobalBounds().getCenter().y)));
    
    Text avisoContinuar(arialFnt, "ESC NOVAMENTE PARA CONTINUAR");
    avisoContinuar.setPosition(Vector2f(float(WIN_SX/2 - avisoContinuar.getGlobalBounds().getCenter().x), 500.f));

    Text avisoVoltar(arialFnt, "Q PARA VOLTAR AO MENU PRINCIPAL");
    avisoVoltar.setPosition(Vector2f(float(WIN_SX/2 - avisoVoltar.getGlobalBounds().getCenter().x), 600.f));

    std::vector<Drawable*> objPause = {&avisoPause, &avisoContinuar, &avisoVoltar};

    // TELA FINAL

    Text gameOverTxt(tetrisFnt, "GAME OVER", 100);
    float centroXGO = gameOverTxt.getGlobalBounds().getCenter().x;
    float centroYGO = gameOverTxt.getGlobalBounds().getCenter().y;
    gameOverTxt.setPosition(Vector2f(float(WIN_SX/2 - gameOverTxt.getGlobalBounds().getCenter().x), -50.f));
    int gameovertxt_iter = 0;

    Text pontGameOverTxt(tetrisFnt, "Pontuacao final: 000");

    float centroXPGO = pontGameOverTxt.getGlobalBounds().getCenter().x;
    float centroYPGO = pontGameOverTxt.getGlobalBounds().getCenter().y;
    pontGameOverTxt.setPosition(Vector2f(float(centroTargetX - centroXPGO), float(centroTargetY - centroYPGO) + 100.f));
    pontGameOverTxt.setFillColor(Color{255,255,255,0});
    Color pontGameOverTxtColor = Color{255,255,255,0};
    bool pontGameOverTxtAnim = false;
    bool pontGameOverTxtAnimStatus = false;

    Text avisoVoltar2(arialFnt, "Q PARA VOLTAR AO MENU");
    float centroXAGO = avisoVoltar2.getGlobalBounds().getCenter().x;
    float centroYAGO = avisoVoltar2.getGlobalBounds().getCenter().y;
    avisoVoltar2.setPosition(Vector2f(float(centroTargetX - centroXAGO), float(centroTargetY - centroYAGO) + 300.f));
    avisoVoltar2.setFillColor(Color{255,255,255,0});
    

    std::vector<Drawable*> objGameOver = {&gameOverTxt, &pontGameOverTxt, &avisoVoltar2};
    bool gameOverAnim = false;

    //std::vector<Drawable*>* cenaAtiva = &objTelaInicial;
    std::vector<Drawable*>* cenaAtiva = &objTelaInicial;

    Music musica(assets + "\\audio\\tetris.ogg");
    musica.setLooping(true);
    musica.play();

    int game_iter = 0;

    while (window.isOpen()) {
        if (cenaAtiva == &objTelaInicial) {
            if (!titulo_ready) {
                movAnim(titulo, titulo_iter, WIN_FPS*1, Vector2f(float(centroTargetX - centroTextoX), 0.f), titulo_ready); // WIN_FPS * 1 = 1 segundo
            } else {
                altText.setFillColor(blinkAnim(altText, altTextBlnkAnim, altTextColor, 10));
                titulo_iter ++;
                if (titulo_iter == 60) {
                    titulo.setFillColor(changeColor(titulo, tituloCores, tituloCorAtual));
                    titulo_iter = 0;
                }
            }
        }
        else if (cenaAtiva == &objReady) {
            readyTimer ++;
            if (readyTimer == 120) {
                readyTimer = 0;
                cenaAtiva = &objJogo;
                nEscolhido = dist(mt);
                for (int i = 0; i < texturasUsadas.size(); i ++) {
                    delete texturasUsadas[i];
                }
                texturasUsadas = {new Texture(texturas[nEscolhido])};
                formato = lista[nEscolhido];
                ms.deleteSet();
                ms.createSet(*texturasUsadas[0], formato);

                nEscolhido = dist(mt);
                delete proximoBloco[0];
                proximoBloco = {new Sprite(makePreAsset(texturasPre[nEscolhido], nextBlockCenter))};
                formatoProximoBloco = lista[nEscolhido];

                conjSpritesAtivos = ms.getSet();
                for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                    conjSpritesAtivos[i].setPosition(conjSpritesAtivos[i].getGlobalBounds().position + blockSpawn);
                }
                pecasColocadas.clear();
            }
        }
        
        if (game_iter == 60) {
            game_iter = 0;
            if (verifyBlocks(conjSpritesAtivos, 'd', pecasColocadas, YBlockMax)) {
                for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                    conjSpritesAtivos[i].move(Vector2f(0.f, 32.f));
                }
            } else {
                for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                    pecasColocadas.push_back(conjSpritesAtivos[i]);
                }
                conjSpritesAtivos.clear();
                ms.deleteSet();
                ms.createSet(texturas[nEscolhido], formatoProximoBloco);
                conjSpritesAtivos = ms.getSet();
                for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                    conjSpritesAtivos[i].setPosition(conjSpritesAtivos[i].getGlobalBounds().position + blockSpawn);
                }
                bool exec = true;
                for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                    if (!verifyPosition(conjSpritesAtivos[i].getPosition(), pecasColocadas)) {
                        musica.play();
                        cenaAtiva = &objGameOver;
                        gameOverAnim = false;
                        game_iter = 0;
                        exec = false;
                    }
                }
                if (exec) {
                    delete proximoBloco[0];
                    proximoBloco.clear();
                    nEscolhido = dist(mt);
                    proximoBloco = {new Sprite(makePreAsset(texturasPre[nEscolhido], nextBlockCenter))};
                    formatoProximoBloco = lista[nEscolhido];
                    verificarBlocosColocados(pecasColocadas, pontuacao);
                    pontuacaoTxt.setString(pontuacao);
                    pontGameOverTxt.setString("Pontuacao final: " + pontuacao);
                }
            }
        }


        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                for (int i = 0; i < proximoBloco.size(); i++) {
                    delete proximoBloco[i];
                }
                for (int i = 0; i < texturasUsadas.size(); i++) {
                    delete texturasUsadas[i];
                }
                window.close();
            }
            else if (const Event::KeyPressed* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->scancode == Keyboard::Scancode::C) {
                    for (int i = 0; i < proximoBloco.size(); i++) {
                        delete proximoBloco[i];
                    }
                    for (int i = 0; i < texturasUsadas.size(); i++) {
                        delete texturasUsadas[i];
                    }
                    window.close();
                } else if (keyPressed->scancode == Keyboard::Scancode::Q && cenaAtiva == &objGameOver || keyPressed->scancode == Keyboard::Scancode::Q && cenaAtiva == &objPause) {
                    cenaAtiva = &objTelaInicial;
                    game_iter = 0;
                } else if (keyPressed->scancode == Keyboard::Scancode::Enter && titulo_ready && cenaAtiva == &objTelaInicial) {
                    titulo_iter = 0;
                    cenaAtiva = &objReady;
                    musica.stop();
                } else if (keyPressed->scancode == Keyboard::Scancode::M) {
                    int volume = musica.getVolume();
                    if (volume == 0) {
                        musica.setVolume(100);
                    } else {
                        musica.setVolume(0);
                    }
                }
                if (cenaAtiva == &objJogo) {
                    if (keyPressed->scancode == Keyboard::Scancode::Left || keyPressed->scancode == Keyboard::Scancode::A) {
                        if (verifyBlocks(conjSpritesAtivos, 'l', pecasColocadas, XBlockMin)) {
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                conjSpritesAtivos[i].move(Vector2f(-32.f, 0.f));
                            }
                        } else {
                            continue;
                        }
                    } else if (keyPressed->scancode == Keyboard::Scancode::Right || keyPressed->scancode == Keyboard::Scancode::D) {
                        if (verifyBlocks(conjSpritesAtivos, 'r', pecasColocadas, XBlockMax)) {
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                conjSpritesAtivos[i].move(Vector2f(32.f, 0.f));
                            }
                        } else {
                            continue;
                        }
                    } else if (keyPressed->scancode == Keyboard::Scancode::Down || keyPressed->scancode == Keyboard::Scancode::S) {
                        if (verifyBlocks(conjSpritesAtivos, 'd', pecasColocadas, YBlockMax)) {
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                conjSpritesAtivos[i].move(Vector2f(0.f, 32.f));
                            }
                        } else {
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                pecasColocadas.push_back(conjSpritesAtivos[i]);
                            }
                            conjSpritesAtivos.clear();
                            ms.deleteSet();
                            ms.createSet(texturas[nEscolhido], formatoProximoBloco);
                            conjSpritesAtivos = ms.getSet();
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                conjSpritesAtivos[i].setPosition(conjSpritesAtivos[i].getGlobalBounds().position + blockSpawn);
                            }
                            bool exec = true;
                            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                                if (!verifyPosition(conjSpritesAtivos[i].getPosition(), pecasColocadas)) {
                                    musica.play();
                                    cenaAtiva = &objGameOver;
                                    gameOverAnim = false;
                                    game_iter = 0;
                                    exec = false;
                                }
                            }
                            if (exec) {
                                delete proximoBloco[0];
                                proximoBloco.clear();
                                nEscolhido = dist(mt);
                                proximoBloco = {new Sprite(makePreAsset(texturasPre[nEscolhido], nextBlockCenter))};
                                formatoProximoBloco = lista[nEscolhido];
                                verificarBlocosColocados(pecasColocadas, pontuacao);
                                pontuacaoTxt.setString(pontuacao);
                                pontGameOverTxt.setString("Pontuacao final: " + pontuacao);
                            }
                        }
                    } else if (keyPressed->scancode == Keyboard::Scancode::Escape) {
                        cenaAtiva = &objPause;
                        musica.play();
                    }
                } else if (cenaAtiva == &objPause) {
                    if (keyPressed->scancode == Keyboard::Scancode::Escape) {
                        cenaAtiva = &objJogo;
                        musica.stop();
                    }
                }
            }
        }
        window.clear(Color::Black);
        for (int i = 0; i < cenaAtiva->size(); i++) {
            window.draw(*(*cenaAtiva)[i]);
        }
        if (cenaAtiva == &objJogo) {
            game_iter ++;
            for (int i = 0; i < conjSpritesAtivos.size(); i++) {
                window.draw(conjSpritesAtivos[i]);
            }
            for (int i = 0; i < pecasColocadas.size(); i++) {
                window.draw(pecasColocadas[i]);
            }
            for (int i = 0; i < proximoBloco.size(); i++) {
                window.draw(*proximoBloco[i]);
            }
        }
        if (!gameOverAnim) {
            movAnim(gameOverTxt, gameovertxt_iter, WIN_FPS*2, Vector2f(float(centroTargetX - centroXGO), float(centroTargetY - centroYGO)), gameOverAnim);
            if (gameOverAnim) {
                pontGameOverTxtAnim = true;
            }
        } else if (pontGameOverTxtAnim) {
            pontGameOverTxt.setFillColor(blinkAnim(pontGameOverTxt, pontGameOverTxtAnimStatus, pontGameOverTxtColor, 10));
            avisoVoltar2.setFillColor(pontGameOverTxtColor);
            if (pontGameOverTxtColor == Color{255,255,255,255}) pontGameOverTxtAnim = false;
        }
        window.display();
    }
}
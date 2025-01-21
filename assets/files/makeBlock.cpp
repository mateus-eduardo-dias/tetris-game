#include "makeBlock.hpp"

using namespace sf;

void makeSet::createSet(Texture& textura, char& formato) {
    
    while (this->conjSprites.size() > 0) {
        this->conjSprites.clear();
    } 

    Sprite peca1(textura), peca2(textura), peca3(textura), peca4(textura);
    peca1.setScale(Vector2f(.5f, .5f));
    peca2.setScale(Vector2f(.5f, .5f));
    peca3.setScale(Vector2f(.5f, .5f));
    peca4.setScale(Vector2f(.5f, .5f));

    if (formato == 'i') {
        peca1.setPosition(Vector2f(0.f, 0.f));
        peca2.setPosition(Vector2f(0.f, 32.f));
        peca3.setPosition(Vector2f(0.f, 64.f));
        peca4.setPosition(Vector2f(0.f, 96.f));
    } else if (formato == 'j') {
        peca1.setPosition(Vector2f(0.f, 0.f));
        peca2.setPosition(Vector2f(0.f, 32.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(64.f, 32.f));
    } else if (formato == 'l') {
        peca1.setPosition(Vector2f(64.f, 0.f));
        peca2.setPosition(Vector2f(64.f, 32.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(0.f, 32.f));
    } else if (formato == 'o') {
        peca1.setPosition(Vector2f(0.f, 0.f));
        peca2.setPosition(Vector2f(0.f, 32.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(32.f, 0.f));
    } else if (formato == 's') {
        peca1.setPosition(Vector2f(64.f, 0.f));
        peca2.setPosition(Vector2f(32.f, 0.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(0.f, 32.f));
    } else if (formato == 't') {
        peca1.setPosition(Vector2f(32.f, 0.f));
        peca2.setPosition(Vector2f(0.f, 32.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(64.f, 32.f));
    } else if (formato == 'z') {
        peca1.setPosition(Vector2f(0.f, 0.f));
        peca2.setPosition(Vector2f(32.f, 0.f));
        peca3.setPosition(Vector2f(32.f, 32.f));
        peca4.setPosition(Vector2f(64.f, 32.f));
    }

    this->conjSprites.push_back(peca1);
    this->conjSprites.push_back(peca2);
    this->conjSprites.push_back(peca3);
    this->conjSprites.push_back(peca4);
}

std::vector<Sprite> makeSet::getSet() {
    return this->conjSprites;
}

void makeSet::deleteSet(){
    this->conjSprites.clear();
}


Sprite makePreAsset(Texture& textura, Vector2f& nextBlockCenter) {
    Sprite temp(textura);
    temp.setColor(Color::Black);
    temp.setPosition(Vector2f(nextBlockCenter - temp.getGlobalBounds().getCenter()));
    return temp;
}
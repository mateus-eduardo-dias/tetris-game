#include <SFML/Graphics.hpp>

using namespace sf;

class makeSet {
    private:
        std::vector<Sprite> conjSprites;
    public:
        void createSet(Texture& textura, char& formato);
        std::vector<Sprite> getSet();
        void deleteSet();
};

Sprite makePreAsset(Texture& textura, Vector2f& nextBlockCenter);

// void placeNext(Sprite& object, Sprite& next, Sprite& next2);
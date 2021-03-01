#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <list>
#include <random>
#include <time.h>

using namespace std;

enum Direction { Up, Down, Left, Right, None };

class Character {
    public:
        sf::Texture _texture;
        sf::Sprite _gfx;
        sf::Vector2f _pos;
        sf::Vector2f _vel;
        sf::RenderWindow& _win;

        Character() { } // Implemented solely for array usage

        Character(string filePath,
                  sf::Vector2f position,
                  sf::Vector2f velocity,
                  sf::RenderWindow& window
                  ):_win(window) {
                    _texture = initializeTexture(filePath);
                    _gfx = initializeSprite();
                    _pos = position;
                    _vel = velocity;
                  }

        // Requires OpenGL Context Control
        void displaySelf() {
            _gfx.setPosition(_pos);
            _win.draw(_gfx);
        }

        void moveSelf() {
            _pos.x += _vel.x;
            _pos.y += _vel.y;
        }

        void changeVelocity(sf::Vector2f changeVector) {
            _vel.x += changeVector.x;
            _vel.y += changeVector.y;
        }

        bool checkCollision(Character& o1, Character& o2) {
            // get bounding coords
            sf::Rect<float> o1Bounds = o1._gfx.getGlobalBounds();
            sf::Rect<float> o2Bounds = o2._gfx.getGlobalBounds();
            return o1Bounds.intersects(o2Bounds);
        }

    private:
        sf::Texture initializeTexture(string filePath) {
            sf::Texture texture;
            texture.loadFromFile(filePath);
            return texture;
        }

        sf::Sprite initializeSprite() {
            sf::Sprite sprite;
            sprite.setTexture(_texture, true);
            return sprite;
        }
};

class Ghost  : public Character {
    public:
        Ghost() : Character() { } // Implemented solely for array usage

        Ghost(string filePath, sf::RenderWindow& window) : Character(filePath, sf::Vector2f(450, 800), sf::Vector2f(0.05, 0), window) {
            _gfx.setRotation(90);
            _gfx.setOrigin(sf::Vector2f(16, 16));
        };

        void changeDirection(Direction direction) {
            switch (direction) {
            case Left:
                _vel = sf::Vector2f(-0.05, 0);
                break;
            case Right:
                _vel = sf::Vector2f(0.05, 0);
                break;
            case Up:
                _vel = sf::Vector2f(0, -0.05);
                break;
            case Down:
                _vel = sf::Vector2f(0, 0.05);
                break;
            }
        }

        Direction movementLogic(){
            changeDirection((Direction)(rand() % 4));
            moveSelf();
            displaySelf();
        }
};

class Pacman : public Character {
    public:
        Pacman(sf::RenderWindow& window) : Character("./assets/pacman.png", sf::Vector2f(450, 800), sf::Vector2f(0.15, 0), window) {
            _gfx.setRotation(90);
            _gfx.setOrigin(sf::Vector2f(16, 16));
        };

        void changeDirection(Direction direction) {
            switch (direction) {
            case Left:
                _vel = sf::Vector2f(-0.15, 0);
                _gfx.setRotation(270);
                break;
            case Right:
                _vel = sf::Vector2f(0.15, 0);
                _gfx.setRotation(90);
                break;
            case Up:
                _vel = sf::Vector2f(0, -0.15);
                _gfx.setRotation(0);
                break;
            case Down:
                _vel = sf::Vector2f(0, 0.15);
                _gfx.setRotation(180);
                break;
            }
        }
};

class Fruit;

class Pellet;

class SuperPellet;

class Wall {
    public:
        sf::Rect<float> _boundingRect;
        sf::RectangleShape _gfx;

        Wall(sf::Rect<float> boundingRect) {
            _boundingRect = boundingRect;
            _gfx = getGFX(boundingRect);
        }

        private:
            sf::RectangleShape getGFX(sf::Rect<float> r) {
                sf::RectangleShape gfx = sf::RectangleShape(sf::Vector2f(r.width, r.height));
                gfx.setPosition(sf::Vector2f(r.left, r.top));
                gfx.setFillColor(sf::Color::White);
                return gfx;
            }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Pacman!");
    Pacman player = Pacman(window);
    Ghost ghosts[4];
    list<Wall> walls;

    // #region spooky ghosts
    Ghost inky   = Ghost("./assets/inky.png", window);
    Ghost pinky  = Ghost("./assets/pinky.png", window);
    Ghost blinky = Ghost("./assets/blinky.png", window);
    Ghost clyde  = Ghost("./assets/clyde.png", window);

    ghosts[0] = inky;
    ghosts[1] = pinky;
    ghosts[2] = blinky;
    ghosts[3] = clyde;
    // #endregion

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
                player.changeDirection(Up);
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
                player.changeDirection(Down);
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
                player.changeDirection(Left);
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
                player.changeDirection(Right);
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        for(int i = 0; i < 4; i++) {
            ghosts[i].moveSelf();
        }

        window.clear();
        player.moveSelf();
        player.displaySelf();
        window.display();
    }

    return 0;
}

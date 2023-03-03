#include <stuff/qoi.hpp>

#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace stf::integers;

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }

    std::ifstream ifs(argv[1]);

    if (!ifs) {
        return 2;
    }

    std::vector<u8> data{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>{}};

    stf::qoi::image orig_image{};
    if (auto res = orig_image.from_memory(data); !res) {
        //return 3;
    }

    sf::RenderWindow window(sf::VideoMode({orig_image.width(), orig_image.height()}), "tracer");
    window.setFramerateLimit(60);

    sf::Image sf_image {};
    sf::Texture texture {};
    sf::Sprite sprite {};

    auto update_sprite = [&](stf::qoi::image<> const& image) {
        sf_image.create({image.width(), image.height()});
        for (u32 y = 0; y < image.height(); y++) {
            for (u32 x = 0; x < image.width(); x++) {
                stf::qoi::color c = image.at(x, y);
                sf_image.setPixel({x, y}, {c.r, c.g, c.b, c.a});
            }
        }

        std::ignore = texture.loadFromImage(sf_image);
        sprite.setTexture(texture);
    };

    stf::qoi::image display_image = orig_image;
    double tolerance = 0.;
    update_sprite(display_image);

    while (window.isOpen()) {
        for (sf::Event event; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                bool change = false;
                if (event.key.code == sf::Keyboard::Hyphen) {
                    change = true;
                    tolerance -= 0.5;
                    if (tolerance < 0)
                        tolerance = 0;
                } else if (event.key.code == sf::Keyboard::Equal && event.key.shift) {
                    change = true;
                    tolerance += 0.5;
                }

                std::cout << "Tolerance: " << tolerance << std::endl;

                if (change) {
                    std::vector<uint8_t> re_encode{};
                    orig_image.to_memory(back_inserter(re_encode), tolerance);
                    display_image.from_memory(re_encode);
                    update_sprite(display_image);
                }
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

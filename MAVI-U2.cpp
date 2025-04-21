#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <cmath>
#include <vector>

const float SCALE = 30.0f;
const float DEG_TO_RAD = 3.14159f / 180.0f;

sf::Vector2f toSFMLPos(const b2Vec2& pos) 
{
    return sf::Vector2f(pos.x * SCALE, 600.0f - (pos.y * SCALE));
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ejercicio 6");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    //Suelo

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 10.0f / SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    groundBody->CreateFixture(&groundBox, 0.0f);

    sf::RectangleShape groundRect(sf::Vector2f(800.0f, 20.0f));
    groundRect.setFillColor(sf::Color::Green);
    groundRect.setOrigin(400.0f, 10.0f);
    groundRect.setPosition(toSFMLPos(groundBody->GetPosition()));

    //Cañon

    float cannonAngle = 45.0f;
    sf::RectangleShape cannon(sf::Vector2f(50.0f, 10.0f));
    cannon.setOrigin(0, 5);
    cannon.setPosition(50, 590);
    cannon.setFillColor(sf::Color::Red);

    //Lista de balas activas

    std::vector<std::pair<b2Body*, sf::CircleShape>> bullets;

    //Loop

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Input para angulo

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && cannonAngle < 90)
        {
            cannonAngle += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && cannonAngle > 0)
        {
            cannonAngle -= 1.0f;
        }

        //Disparo

        static bool wasSpacePressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
        {
            if (!wasSpacePressed) 
            {
                wasSpacePressed = true;

                //Crear bala
                b2BodyDef bulletDef;
                bulletDef.type = b2_dynamicBody;
                float angleRad = cannonAngle * DEG_TO_RAD;
                float cannonX = cannon.getPosition().x;
                float cannonY = cannon.getPosition().y;

                float spawnX = cannonX + std::cos(angleRad) * 50.0f;
                float spawnY = cannonY - std::sin(angleRad) * 50.0f;

                bulletDef.position.Set(spawnX / SCALE, (600.0f - spawnY) / SCALE);

                b2Body* bulletBody = world.CreateBody(&bulletDef);

                b2CircleShape circle;
                circle.m_radius = 5.0f / SCALE;

                b2FixtureDef bulletFixture;
                bulletFixture.shape = &circle;
                bulletFixture.density = 1.0f;
                bulletFixture.restitution = 0.6f;
                bulletBody->CreateFixture(&bulletFixture);

                //impulso

                float power = 1.2f;
                bulletBody->ApplyLinearImpulseToCenter(b2Vec2(std::cos(angleRad) * power, std::sin(angleRad) * power), true);

                //Grafica

                sf::CircleShape bulletShape(5.0f);
                bulletShape.setOrigin(5.0f, 5.0f);
                bulletShape.setFillColor(sf::Color::Yellow);

                bullets.push_back({ bulletBody, bulletShape });
            }
        }
        else 
        {
            wasSpacePressed = false;
        }

        //Actualizar fisica

        world.Step(1.0f / 60.0f, 6, 2);

        //Actualiza posicion de proyectil/es

        for (size_t i = 0; i < bullets.size(); ++i) 
        {
            b2Body* body = bullets[i].first;
            sf::CircleShape& shape = bullets[i].second;
            shape.setPosition(toSFMLPos(body->GetPosition()));
        }

        //Actualiza cañon

        cannon.setRotation(-cannonAngle);

        //Dibujar

        window.clear();
        window.draw(groundRect);
        window.draw(cannon);
        for (size_t i = 0; i < bullets.size(); ++i) 
        {
            window.draw(bullets[i].second);
        }
        window.display();
    }

    return 0;
}
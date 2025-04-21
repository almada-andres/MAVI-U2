#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

const float SCALE = 30.0f;
const float WINDOW_WIDTH = 800.0f;
const float WINDOW_HEIGHT = 600.0f;

//Convierte posicion box2d a SFML (Por que sfml tiene Y invertida, ya me hizo sufrir en un ejercicio anterior)

sf::Vector2f toSFMLPos(const b2Vec2& box2dPos) 
{
    return sf::Vector2f(box2dPos.x * SCALE, WINDOW_HEIGHT - (box2dPos.y * SCALE));
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT)), "Ejercicio 4");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    //Plano con friccion

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(WINDOW_WIDTH / 2 / SCALE, 100.0f / SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((WINDOW_WIDTH / 2) / SCALE, 10.0f / SCALE);

    b2FixtureDef groundFixture;
    groundFixture.shape = &groundBox;
    groundFixture.friction = 0.1f;      //Friccion del plano
    groundBody->CreateFixture(&groundFixture);

    sf::RectangleShape groundRect(sf::Vector2f(WINDOW_WIDTH, 20.0f));
    groundRect.setFillColor(sf::Color::Green);
    groundRect.setOrigin(WINDOW_WIDTH / 2, 10.0f);
    groundRect.setPosition(toSFMLPos(groundBody->GetPosition()));

    //Caja

    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(WINDOW_WIDTH / 2 / SCALE, 300.0f / SCALE);
    b2Body* boxBody = world.CreateBody(&boxBodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(25.0f / SCALE, 25.0f / SCALE);

    b2FixtureDef boxFixture;
    boxFixture.shape = &boxShape;
    boxFixture.density = 1.0f;
    boxFixture.friction = 0.1f;         //Friccion de la caja
    boxBody->CreateFixture(&boxFixture);

    sf::RectangleShape boxRect(sf::Vector2f(50.0f, 50.0f));
    boxRect.setFillColor(sf::Color::Blue);
    boxRect.setOrigin(25.0f, 25.0f);

    //Loop

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Input

        const float forceMagnitude = 10.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        {
            boxBody->ApplyForceToCenter(b2Vec2(forceMagnitude, 0.0f), true);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
        {
            boxBody->ApplyForceToCenter(b2Vec2(-forceMagnitude, 0.0f), true);
        }

        world.Step(1.0f / 60.0f, 6, 2);

        //Actualizar posiciones

        boxRect.setPosition(toSFMLPos(boxBody->GetPosition()));
        boxRect.setRotation(boxBody->GetAngle() * 180.0f / b2_pi);

        window.clear();
        window.draw(groundRect);
        window.draw(boxRect);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

const float SCALE = 30.0f;

//Funcion para convertir posiciones de box2d a SFML (por que tenia invertidas las posiciones del piso visualmente)

sf::Vector2f toSFMLPos(const b2Vec2& box2dPos) 
{
    return sf::Vector2f(box2dPos.x * SCALE, 600.0f - (box2dPos.y * SCALE));
}


int main()
{
    //Ventana

    sf::RenderWindow window(sf::VideoMode(800, 600), "Ejercicio 1");
    window.setFramerateLimit(60);

    //Mundo con gravedad

    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    //Piso

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 50.0f / SCALE);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((800.0f / 2) / SCALE, (20.0f / 2) / SCALE);

    groundBody->CreateFixture(&groundBox, 0.0f);

    sf::RectangleShape groundRect(sf::Vector2f(800.0f, 20.0f));
    groundRect.setFillColor(sf::Color::Green);
    groundRect.setOrigin(400.0f, 10.0f);

    //Bloque

    b2BodyDef blockBodyDef;
    blockBodyDef.type = b2_dynamicBody;
    blockBodyDef.position.Set(400.0f / SCALE, 400.0f / SCALE);

    b2Body* blockBody = world.CreateBody(&blockBodyDef);

    b2PolygonShape blockBox;
    blockBox.SetAsBox((40.0f / 2) / SCALE, (40.0f / 2) / SCALE);

    b2FixtureDef blockFixture;
    blockFixture.shape = &blockBox;
    blockFixture.density = 1.0f;
    blockFixture.friction = 0.3f;

    blockBody->CreateFixture(&blockFixture);

    sf::RectangleShape blockRect(sf::Vector2f(40.0f, 40.0f));
    blockRect.setFillColor(sf::Color::Blue);
    blockRect.setOrigin(20.0f, 20.0f);
    blockRect.setPosition(400.0f, 100.0f);

    //Loop principal

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(1.0f / 60.0f, 6, 2);

        //Actualizar posicion del bloq ue desde box2d

        blockRect.setPosition(toSFMLPos(blockBody->GetPosition()));
        float angle = blockBody->GetAngle();
        blockRect.setRotation(angle * 180.0f / b2_pi);

        groundRect.setPosition(toSFMLPos(groundBody->GetPosition()));

        //Dibujar

        window.clear(sf::Color::Black);
        window.draw(groundRect);
        window.draw(blockRect);
        window.display();
    }

    return 0;
}
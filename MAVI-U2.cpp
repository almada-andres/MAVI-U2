#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <cmath>

const float SCALE = 30.0f;
const float DEG_TO_RAD = 3.14159f / 180.0f;
const float WINDOW_WIDTH = 800.0f;
const float WINDOW_HEIGHT = 600.0f;

//Convierte posicion box2d a SFML.-

sf::Vector2f toSFMLPos(const b2Vec2& box2dPos) 
{
    return sf::Vector2f(box2dPos.x * SCALE, WINDOW_HEIGHT - (box2dPos.y * SCALE));
}


int main() 
{
    //Ventana

    sf::RenderWindow window(sf::VideoMode((unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT), "Ejercicio 5 - Caja en plano inclinado");
    window.setFramerateLimit(60);

    //mundo Box2d

    b2Vec2 gravity(0.0f, -10.0f);  //Gravedad normal hacia abajo (redondeando) 
    b2World world(gravity);

    //angulo del plano

    float angleDeg = 30.0f;
    float angleRad = angleDeg * DEG_TO_RAD;

    //Plano

    b2BodyDef rampBodyDef;
    rampBodyDef.position.Set(400.0f / SCALE, 300.0f / SCALE);
    b2Body* rampBody = world.CreateBody(&rampBodyDef);

    b2PolygonShape rampBox;
    rampBox.SetAsBox((300.0f / 2) / SCALE, (20.0f / 2) / SCALE);

    b2FixtureDef rampFixture;
    rampFixture.shape = &rampBox;
    rampFixture.friction = 0.5f;        //Friccion
    rampBody->CreateFixture(&rampFixture);
    rampBody->SetTransform(rampBody->GetPosition(), -angleRad);     //rotar el plano

    sf::RectangleShape rampRect(sf::Vector2f(300.0f, 20.0f));
    rampRect.setFillColor(sf::Color::Green);
    rampRect.setOrigin(150.0f, 10.0f);
    rampRect.setRotation(angleDeg);     //Para que coincida ocn el de box2d

    //Caja

    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(300.0f / SCALE, 400.0f / SCALE);
    b2Body* boxBody = world.CreateBody(&boxBodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((40.0f / 2) / SCALE, (40.0f / 2) / SCALE);

    b2FixtureDef boxFixture;
    boxFixture.shape = &boxShape;
    boxFixture.density = 1.0f;
    boxFixture.friction = 0.5f;
    boxBody->CreateFixture(&boxFixture);

    sf::RectangleShape boxRect(sf::Vector2f(40.0f, 40.0f));
    boxRect.setFillColor(sf::Color::Red);
    boxRect.setOrigin(20.0f, 20.0f);

    //Loop

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        world.Step(1.0f / 60.0f, 6, 2);

        //Actualizar graficos

        rampRect.setPosition(toSFMLPos(rampBody->GetPosition()));
        boxRect.setPosition(toSFMLPos(boxBody->GetPosition()));
        boxRect.setRotation(boxBody->GetAngle() * 180.0f / b2_pi);

        window.clear();
        window.draw(rampRect);
        window.draw(boxRect);
        window.display();
    }

    return 0;
}
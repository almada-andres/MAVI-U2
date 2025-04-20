#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

const float SCALE = 30.0f;
const int WIDTH = 800;
const int HEIGHT = 600;

//Convertir de box2d a coordenadas SFML

sf::Vector2f toSFMLPos(const b2Vec2& box2dPos) 
{
    return sf::Vector2f(box2dPos.x * SCALE, HEIGHT - (box2dPos.y * SCALE));
}


int main() 
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ejercicio 2");
    window.setFramerateLimit(60);

    //Munod sin gravedad

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    //---------------------------
    //Bordes

    b2BodyDef borderBodyDef;
    borderBodyDef.position.Set(0.0f, 0.0f);
    b2Body* borderBody = world.CreateBody(&borderBodyDef);

    b2EdgeShape edge;

    //suelo

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(WIDTH / SCALE, 0));
    borderBody->CreateFixture(&edge, 0.0f);

    //Techo

    edge.SetTwoSided(b2Vec2(0, HEIGHT / SCALE), b2Vec2(WIDTH / SCALE, HEIGHT / SCALE));
    borderBody->CreateFixture(&edge, 0.0f);

    //Izquierdo

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(0, HEIGHT / SCALE));
    borderBody->CreateFixture(&edge, 0.0f);

    //Derecho

    edge.SetTwoSided(b2Vec2(WIDTH / SCALE, 0), b2Vec2(WIDTH / SCALE, HEIGHT / SCALE));
    borderBody->CreateFixture(&edge, 0.0f);
    
    //----------------------

    //--------------------
    //Pelota

    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(WIDTH / 2 / SCALE, HEIGHT / 2 / SCALE);
    b2Body* ballBody = world.CreateBody(&ballBodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 20.0f / SCALE;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 0.0f;
    ballFixtureDef.restitution = 1.0f;
    ballBody->CreateFixture(&ballFixtureDef);

    //"movimiento inicial"

    ballBody->SetLinearVelocity(b2Vec2(5.0f, -4.0f));

    //Pelota en SFML

    sf::CircleShape ballShape(20.0f);
    ballShape.setFillColor(sf::Color::Red);
    ballShape.setOrigin(20.0f, 20.0f);

    //-----------------

    //Loop

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(1.0f / 60.0f, 8, 3);


        //Actualizar la posicion de la pelota

        ballShape.setPosition(toSFMLPos(ballBody->GetPosition()));

        window.clear(sf::Color::Black);
        window.draw(ballShape);
        window.display();
    }


    return 0;
}
#include <box2d/box2d.h>
#include <iostream>

int main()
{
    b2World world(b2Vec2(0.0f, -10.0f)); // Mundo con gravedad

    // Piso fijo
    b2Body* piso = world.CreateBody(new b2BodyDef());
    b2PolygonShape formaPiso;
    formaPiso.SetAsBox(50, 10);
    piso->CreateFixture(&formaPiso, 0.0f);

    // Bloque que cae
    b2BodyDef defBloque;
    defBloque.type = b2_dynamicBody;
    defBloque.position.Set(0, 20);
    b2Body* bloque = world.CreateBody(&defBloque);

    b2PolygonShape formaBloque;
    formaBloque.SetAsBox(1, 1);
    bloque->CreateFixture(&formaBloque, 1.0f);

    //Tiempo de simulacion

    for (int i = 0; i < 60; i++)
    {
        world.Step(1.0f / 60.0f, 6, 2);
        auto pos = bloque->GetPosition();
        std::cout << "y = " << pos.y << std::endl;
    }

    return 0;
}
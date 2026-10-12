#pragma once
#include "raylib.h"
#include "city.h"

// Jugador a pie: desplazamiento relativo a la cámara, gravedad básica y
// colisión contra edificios. Tercera persona.
class Player {
public:
    void init(Vector3 spawn);
    void update(float dt, const City& city, bool vehicleControlled);
    void draw() const;

    Vector3 position() const { return m_pos; }
    float heading() const { return m_yaw; }

    // Control directo usado cuando conduce un vehículo.
    void setPosition(Vector3 p) { m_pos = p; }
    void setHeading(float yaw) { m_yaw = yaw; }

private:
    Vector3 m_pos{ 0, 0, 0 };
    Vector3 m_vel{ 0, 0, 0 };
    float m_yaw = 0.0f;      // dirección que mira el personaje
    float m_bodyYaw = 0.0f;  // orientación visual del cuerpo
    bool m_grounded = true;
};

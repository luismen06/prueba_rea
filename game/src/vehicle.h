#pragma once
#include "raylib.h"
#include "city.h"

// Vehículo conducible: física de bicicleta simple (dirección + aceleración +
// fricción). Se entra/sale con la tecla E.
class Vehicle {
public:
    void init(Vector3 spawn);
    void update(float dt, const City& city, bool beingDriven);
    void draw() const;

    Vector3 position() const { return m_pos; }
    float heading() const { return m_yaw; }
    Vector3 seatOffset() const; // dónde aparece el jugador al salir

private:
    Vector3 m_pos{ 0, 0, 0 };
    float m_yaw = 0.0f;
    float m_speed = 0.0f;     // velocidad longitudinal (m/s)
    float m_steer = 0.0f;     // ángulo de ruedas (visual)
    Color m_color{ 180, 40, 40, 255 };
};

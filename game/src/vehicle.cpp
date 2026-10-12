#include "vehicle.h"
#include <cmath>

void Vehicle::init(Vector3 spawn) {
    m_pos = spawn;
    m_yaw = 0.0f;
    m_speed = 0.0f;
    m_steer = 0.0f;
}

void Vehicle::update(float dt, const City& city, bool beingDriven) {
    if (beingDriven) {
        // Acelerar / frenar.
        if (IsKeyDown(KEY_W)) m_speed += 22.0f * dt;
        if (IsKeyDown(KEY_S)) m_speed -= 22.0f * dt;

        // Dirección proporcional a la velocidad.
        float steerInput = 0.0f;
        if (IsKeyDown(KEY_A)) steerInput = -1.0f;
        if (IsKeyDown(KEY_D)) steerInput = 1.0f;
        m_steer += (steerInput * 0.6f - m_steer) * fminf(1.0f, dt * 8.0f);

        float grip = Clamp(fabsf(m_speed) * 0.06f, 0.0f, 1.0f);
        m_yaw += m_steer * grip * (m_speed >= 0 ? 1.0f : -1.0f) * dt * 2.2f;
    }

    // Fricción y límites.
    m_speed *= (1.0f - fminf(1.0f, dt * 1.4f));
    if (fabsf(m_speed) < 0.05f) m_speed = 0.0f;
    m_speed = Clamp(m_speed, -18.0f, 38.0f);

    // Integrar.
    Vector3 dir = { sinf(m_yaw), 0.0f, cosf(m_yaw) };
    m_pos.x += dir.x * m_speed * dt;
    m_pos.z += dir.z * m_speed * dt;

    // Colisión: rebotar suavemente.
    Vector3 before = m_pos;
    city.resolveCollision(m_pos, 1.4f);
    if (before.x != m_pos.x || before.z != m_pos.z) {
        m_speed *= -0.25f; // rebote
    }
}

Vector3 Vehicle::seatOffset() const {
    // Salir por el lado izquierdo del coche.
    Vector3 side = { cosf(m_yaw), 0.0f, -sinf(m_yaw) };
    return { m_pos.x + side.x * 2.2f, m_pos.y, m_pos.z + side.z * 2.2f };
}

void Vehicle::draw() const {
    Vector3 p = m_pos;

    // Chasis.
    DrawCube({ p.x, p.y + 0.6f, p.z }, 1.9f, 0.7f, 4.2f, m_color);
    DrawCubeWires({ p.x, p.y + 0.6f, p.z }, 1.9f, 0.7f, 4.2f, Color{ 20, 20, 20, 255 });

    // Cabina.
    Vector3 cabOff = { sinf(m_yaw) * -0.2f, 0, cosf(m_yaw) * -0.2f };
    DrawCube({ p.x + cabOff.x, p.y + 1.15f, p.z + cabOff.z }, 1.7f, 0.6f, 2.0f,
             Color{ 160, 35, 35, 255 });
    // Cristal delantero.
    DrawCube({ p.x + sinf(m_yaw) * 0.9f, p.y + 1.15f, p.z + cosf(m_yaw) * 0.9f },
             1.5f, 0.5f, 0.1f, Color{ 150, 190, 220, 200 });

    // Ruedas.
    float c = cosf(m_yaw), s = sinf(m_yaw);
    Vector3 corners[4] = {
        {  0.95f,  1.5f }, { -0.95f,  1.5f },
        {  0.95f, -1.5f }, { -0.95f, -1.5f }
    };
    for (auto& co : corners) {
        Vector3 wp = { p.x + co.x * c + co.y * s, p.y + 0.35f,
                       p.z + co.y * c - co.x * s };
        DrawCylinderEx({ wp.x - s * 0.15f, wp.y, wp.z - c * 0.15f },
                       { wp.x + s * 0.15f, wp.y, wp.z + c * 0.15f },
                       0.35f, 0.35f, 8, Color{ 25, 25, 28, 255 });
    }
}

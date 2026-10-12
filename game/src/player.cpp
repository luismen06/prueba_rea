#include "player.h"
#include <cmath>

void Player::init(Vector3 spawn) {
    m_pos = spawn;
    m_vel = { 0, 0, 0 };
    m_yaw = 0.0f;
    m_bodyYaw = 0.0f;
}

void Player::update(float dt, const City& city, bool vehicleControlled) {
    if (vehicleControlled) return; // El vehículo controla su posición.

    // Cámara en tercera persona: la cámara mira al jugador desde detrás.
    // Calculamos la dirección "adelante" desde la cámara para el movimiento.
    Camera3D cam = { 0 };
    // Reutilizamos la lógica de cámara definida en main; aquí tomamos el
    // yaw guardado del cuerpo para mover relativo a donde mira.
    float camYaw = m_bodyYaw;

    Vector3 forward = { sinf(camYaw), 0, cosf(camYaw) };
    Vector3 right = { cosf(camYaw), 0, -sinf(camYaw) };

    Vector3 wish = { 0, 0, 0 };
    if (IsKeyDown(KEY_W)) { wish.x += forward.x; wish.z += forward.z; }
    if (IsKeyDown(KEY_S)) { wish.x -= forward.x; wish.z -= forward.z; }
    if (IsKeyDown(KEY_A)) { wish.x -= right.x;  wish.z -= right.z; }
    if (IsKeyDown(KEY_D)) { wish.x += right.x;  wish.z += right.z; }

    float len = sqrtf(wish.x * wish.x + wish.z * wish.z);
    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 12.0f : 6.0f;
    if (len > 1e-4f) {
        wish.x /= len; wish.z /= len;
        m_vel.x = wish.x * speed;
        m_vel.z = wish.z * speed;
        // Orientar cuerpo hacia el movimiento.
        m_bodyYaw = atan2f(wish.x, wish.z);
    } else {
        m_vel.x = 0; m_vel.z = 0;
    }

    // Integrar posición.
    m_pos.x += m_vel.x * dt;
    m_pos.z += m_vel.z * dt;

    // Colisión contra edificios.
    city.resolveCollision(m_pos, 0.5f);

    // Rebajar velocidad residual.
    m_yaw = m_bodyYaw;
}

void Player::draw() const {
    // Cuerpo (cápsula simplificada con dos esferas + cilindro).
    Vector3 base = m_pos;
    float bob = 0.0f;
    Color skin = Color{ 224, 172, 138, 255 };
    Color shirt = Color{ 60, 90, 160, 255 };
    Color pants = Color{ 40, 40, 48, 255 };

    // Piernas.
    DrawCylinder({ base.x, base.y + 0.45f, base.z }, 0.22f, 0.22f, 0.9f, 8, pants);
    // Torso.
    DrawCylinder({ base.x, base.y + 1.25f, base.z }, 0.28f, 0.34f, 0.8f, 10, shirt);
    // Cabeza.
    DrawSphere({ base.x, base.y + 1.85f, base.z }, 0.24f, skin);

    // Indicador de dirección (pequeña nariz).
    Vector3 nose = { base.x + sinf(m_bodyYaw) * 0.35f, base.y + 1.85f,
                     base.z + cosf(m_bodyYaw) * 0.35f };
    DrawSphere(nose, 0.06f, Color{ 200, 120, 100, 255 });
}

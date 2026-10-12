#include "city.h"
#include <cstdlib>
#include <cmath>

bool resolveCircleVsBox(Vector3& pos, float radius, const Box& b) {
    float cx = Clamp(pos.x, b.min.x, b.max.x);
    float cz = Clamp(pos.z, b.min.z, b.max.z);
    float dx = pos.x - cx;
    float dz = pos.z - cz;
    float d2 = dx * dx + dz * dz;

    if (d2 > radius * radius) return false;

    if (d2 > 1e-6f) {
        float d = sqrtf(d2);
        float push = (radius - d);
        pos.x += (dx / d) * push;
        pos.z += (dz / d) * push;
    } else {
        // Centro dentro de la caja: expulsar por el eje de menor penetración.
        float left = pos.x - b.min.x;
        float right = b.max.x - pos.x;
        float top = pos.z - b.min.z;
        float bottom = b.max.z - pos.z;
        float m = fminf(fminf(left, right), fminf(top, bottom));
        if (m == left)       pos.x = b.min.x - radius;
        else if (m == right) pos.x = b.max.x + radius;
        else if (m == top)   pos.z = b.min.z - radius;
        else                 pos.z = b.max.z + radius;
    }
    return true;
}

void City::generate(unsigned seed, int gridCount, float blockSize, float roadWidth) {
    m_blocks.clear();
    srand(seed);
    m_blockSize = blockSize;
    m_roadWidth = roadWidth;
    m_gridCount = (float)gridCount;

    float cell = blockSize + roadWidth;
    float origin = -0.5f * gridCount * cell;

    for (int gz = 0; gz < gridCount; ++gz) {
        for (int gx = 0; gx < gridCount; ++gx) {
            Block blk;
            float minX = origin + gx * cell;
            float minZ = origin + gz * cell;
            blk.bounds = { {minX, 0, minZ}, {minX + blockSize, 40, minZ + blockSize} };
            blk.center = { minX + blockSize * 0.5f, 0, minZ + blockSize * 0.5f };

            // Dividir la manzana en 2x2 lotes y poner edificios de altura variable.
            float lot = blockSize * 0.5f;
            for (int lz = 0; lz < 2; ++lz) {
                for (int lx = 0; lx < 2; ++lx) {
                    float pad = 3.0f;
                    float bx = minX + lx * lot + pad;
                    float bz = minZ + lz * lot + pad;
                    float bw = lot - pad * 2.0f;

                    float h = 6.0f + (rand() % 100) * 0.35f; // 6..41
                    // Algunos lotes quedan vacíos (plazas).
                    if (rand() % 5 == 0) continue;

                    blk.buildings.push_back({ {bx, 0, bz}, {bx + bw, h, bz + bw} });
                }
            }
            m_blocks.push_back(blk);
        }
    }
}

void City::drawBlock(const Block& b) const {
    // Suelo de la manzana.
    Vector3 s = { b.bounds.max.x - b.bounds.min.x, 0.1f, b.bounds.max.z - b.bounds.min.z };
    Vector3 c = { b.center.x, -0.05f, b.center.z };
    DrawCube(c, s.x, s.y, s.z, Color{ 70, 70, 78, 255 });
    DrawCubeWires(c, s.x, s.y, s.z, Color{ 40, 40, 46, 255 });

    for (const Box& w : b.buildings) {
        Vector3 ws = { w.max.x - w.min.x, w.max.y - w.min.y, w.max.z - w.min.z };
        Vector3 wc = { (w.min.x + w.max.x) * 0.5f, w.max.y * 0.5f, (w.min.z + w.max.z) * 0.5f };
        // Tinte variado por altura para dar sensación urbana.
        unsigned char g = (unsigned char)Clamp(90.0f + w.max.y * 2.0f, 90.0f, 190.0f);
        Color col = Color{ (unsigned char)(g * 0.7f), (unsigned char)(g * 0.75f), g, 255 };
        DrawCube(wc, ws.x, ws.y, ws.z, col);
        DrawCubeWires(wc, ws.x, ws.y, ws.z, Color{ 30, 30, 40, 255 });

        // Ventas simples como líneas horizontales.
        for (float y = 3.0f; y < ws.y; y += 4.0f) {
            DrawCube({ wc.x, y, wc.z + ws.z * 0.5f + 0.02f}, ws.x * 0.85f, 0.6f, 0.02f,
                     Color{ 180, 200, 220, 255 });
        }
    }
}

void City::draw(const Vector3& focus) const {
    // Asfalto continuo de fondo.
    DrawPlane({ focus.x, 0.0f, focus.z }, { m_gridCount * (m_blockSize + m_roadWidth) * 2.0f,
                                            m_gridCount * (m_blockSize + m_roadWidth) * 2.0f },
              Color{ 45, 45, 50, 255 });

    for (const Block& b : m_blocks) {
        // Culling básico por distancia para rendimiento.
        float dx = b.center.x - focus.x;
        float dz = b.center.z - focus.z;
        if (dx * dx + dz * dz > 900.0f * 900.0f) continue;
        drawBlock(b);
    }
}

void City::resolveCollision(Vector3& pos, float radius) const {
    for (const Block& b : m_blocks) {
        float dx = b.center.x - pos.x;
        float dz = b.center.z - pos.z;
        if (dx * dx + dz * dz > 600.0f * 600.0f) continue;
        for (const Box& w : b.buildings) {
            if (w.max.y < 1.0f) continue;
            if (w.containsXZ(pos.x, pos.z, radius)) {
                resolveCircleVsBox(pos, radius, w);
            }
        }
    }
}

Vector3 City::randomStreetSpawn() const {
    if (m_blocks.empty()) return { 0, 0, 0 };
    // Spawn en el centro de una calle entre manzanas.
    const Block& b = m_blocks[rand() % m_blocks.size()];
    float cell = m_blockSize + m_roadWidth;
    return { b.center.x + cell * 0.5f, 0.0f, b.center.z + cell * 0.5f };
}

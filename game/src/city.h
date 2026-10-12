#pragma once
#include "raylib.h"
#include "types.h"
#include <vector>

// Ciudad procedural: rejilla de manzanas con edificios y calles.
// Patrón inspirado en "streaming por celdas": solo las manzanas cercanas
// a la cámara se consideran activas para lógica/colisión.
class City {
public:
    void generate(unsigned seed, int gridCount, float blockSize, float roadWidth);
    void draw(const Vector3& focus) const;

    // Resuelve colisión de un círculo (jugador/vehículo) contra edificios
    // cercanos al punto 'focus'. Actualiza pos en el sitio.
    void resolveCollision(Vector3& pos, float radius) const;

    Vector3 randomStreetSpawn() const;
    float groundLevel() const { return 0.0f; }

private:
    struct Block {
        Box bounds;          // manzana completa
        std::vector<Box> buildings; // cajas de edificios dentro de la manzana
        Vector3 center;
    };

    std::vector<Block> m_blocks;
    float m_blockSize = 0.0f;
    float m_roadWidth = 0.0f;
    float m_gridCount = 0.0f;

    void drawBlock(const Block& b) const;
};

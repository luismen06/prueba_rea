#pragma once
#include "raylib.h"
#include "raymath.h"   // Clamp, Lerp, etc.

// Tipos matemáticos compartidos (reutilizamos los de raylib: Vector2/Vector3)

// Caja alineada al eje (AABB) para colisión simple de edificios.
struct Box {
    Vector3 min;
    Vector3 max;

    bool containsXZ(float x, float z, float radius) const {
        return x + radius > min.x && x - radius < max.x &&
               z + radius > min.z && z - radius < max.z;
    }
};

// Empuja un punto fuera de una caja en el eje XZ de menor penetración.
// Devuelve true si hubo colisión y la posición fue corregida.
bool resolveCircleVsBox(Vector3& pos, float radius, const Box& b);

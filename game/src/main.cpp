#include "raylib.h"
#include "city.h"
#include "player.h"
#include "vehicle.h"
#include <cmath>
#include <cstdio>

// Cámara de tercera persona que sigue a un objetivo con órbita suave.
struct ChaseCam {
    Vector3 target{ 0, 1.5f, 0 };
    float distance = 9.0f;
    float pitch = 0.32f;
    float yaw = 0.0f;

    void follow(Vector3 focus, float desiredYaw, float dt) {
        // Interpolar el yaw hacia la dirección del objetivo.
        float diff = desiredYaw - yaw;
        while (diff > PI) diff -= 2 * PI;
        while (diff < -PI) diff += 2 * PI;
        yaw += diff * fminf(1.0f, dt * 3.0f);

        target.x += (focus.x - target.x) * fminf(1.0f, dt * 12.0f);
        target.y += (focus.y + 1.6f - target.y) * fminf(1.0f, dt * 12.0f);
        target.z += (focus.z - target.z) * fminf(1.0f, dt * 12.0f);
    }

    Camera3D toRaylib() const {
        Camera3D cam = { 0 };
        cam.target = target;
        cam.up = { 0, 1, 0 };
        cam.fovy = 60.0f;
        cam.projection = CAMERA_PERSPECTIVE;
        cam.position = {
            target.x - sinf(yaw) * cosf(pitch) * distance,
            target.y + sinf(pitch) * distance,
            target.z - cosf(yaw) * cosf(pitch) * distance
        };
        return cam;
    }
};

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "OpenWorld Engine - prototipo");
    SetTargetFPS(60);

    City city;
    city.generate(1337, 14, 60.0f, 18.0f);

    Player player;
    player.init(city.randomStreetSpawn());

    // Spawn del vehículo en una calle cercana.
    Vehicle vehicle;
    Vector3 vspawn = city.randomStreetSpawn();
    vspawn.x += 6.0f;
    vehicle.init(vspawn);

    ChaseCam cam;
    cam.yaw = 0.0f;

    bool inVehicle = false;
    float hudFlash = 0.0f;

    while (!WindowShouldClose()) {
        float dt = fminf(GetFrameTime(), 1.0f / 20.0f);

        // Entrar / salir del vehículo con E.
        if (IsKeyPressed(KEY_E)) {
            if (!inVehicle) {
                Vector3 pp = player.position();
                Vector3 vp = vehicle.position();
                float dx = pp.x - vp.x, dz = pp.z - vp.z;
                if (dx * dx + dz * dz < 4.5f * 4.5f) {
                    inVehicle = true;
                    hudFlash = 1.5f;
                }
            } else {
                inVehicle = false;
                player.setPosition(vehicle.seatOffset());
                player.setHeading(vehicle.heading());
                hudFlash = 1.5f;
            }
        }

        // Actualizar sistemas.
        vehicle.update(dt, city, inVehicle);
        player.update(dt, city, inVehicle);
        if (inVehicle) {
            // El jugador se pegue al asiento.
            player.setPosition(vehicle.position());
            player.setHeading(vehicle.heading());
        }

        // Cámara: sigue al jugador o al vehículo.
        Vector3 focus = inVehicle ? vehicle.position() : player.position();
        float desiredYaw = inVehicle ? vehicle.heading()
                                     : atan2f(sinf(player.heading()), cosf(player.heading()));
        cam.follow(focus, desiredYaw, dt);

        if (hudFlash > 0) hudFlash -= dt;

        // ---- Render ----
        BeginDrawing();
        ClearBackground(Color{ 120, 160, 210, 255 });

        Camera3D rc = cam.toRaylib();
        BeginMode3D(rc);

        // Sol y luces básicas.
        DrawGrid(400, 4.0f);
        city.draw(focus);
        vehicle.draw();
        if (!inVehicle) player.draw();

        // Marcador de interacción con vehículo.
        if (!inVehicle) {
            Vector3 pp = player.position();
            Vector3 vp = vehicle.position();
            float dx = pp.x - vp.x, dz = pp.z - vp.z;
            if (dx * dx + dz * dz < 4.5f * 4.5f) {
                DrawCube({ vp.x, vp.y + 3.2f, vp.z }, 0.3f, 0.3f, 0.3f, YELLOW);
            }
        }

        EndMode3D();

        // HUD.
        DrawRectangle(0, 0, GetScreenWidth(), 54, Color{ 0, 0, 0, 140 });
        DrawText(inVehicle ? "MODO: VEHICULO" : "MODO: A PIE",
                 16, 14, 26, inVehicle ? YELLOW : WHITE);

        const char* help = inVehicle
            ? "W/S acelerar-frenar  A/D girar  E salir"
            : "WASD mover  SHIFT correr  E entrar al coche (cerca)";
        DrawText(help, 320, 20, 20, Color{ 220, 220, 220, 255 });

        if (hudFlash > 0) {
            DrawText(inVehicle ? "¡Has entrado al vehiculo!" : "Has salido del vehiculo",
                     16, 60, 20, Color{ 255, 255, 160, (unsigned char)(hudFlash * 160) });
        }

        DrawFPS(16, GetScreenHeight() - 30);
        DrawText("Prototipo original de motor de mundo abierto",
                 GetScreenWidth() - 480, GetScreenHeight() - 30, 20,
                 Color{ 230, 230, 230, 200 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

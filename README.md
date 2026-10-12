# OpenWorld Engine — Proyecto

Proyecto de un motor de juego de **mundo abierto en C++** con arquitectura
inspirada en patrones observados en motores de la era 2004-2005 (análisis
estático del binario original de GTA San Andreas v1.0 US mediante REA + Ghidra).

## Principios de IP

- Sin assets, nombres, historia ni código de Rockstar Games.
- Solo patrones de arquitectura: streaming de mundo, entidades, scripting,
  físicas de vehículo, audio espacial.
- Implementación, nombres y contenidos 100% originales.

## Estructura

```
new GTA/
├── docs/                  # Análisis y diseño
│   └── ENGINE_ARCHITECTURE.md
├── engine/                # Código del motor (C++20)
├── game/                  # Juego (lógica sobre el motor)
└── gta_sa_analysis/       # Binario bajo análisis (no redistribuir)
```

## Build

```bash
cmake -S engine -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Requisitos

- CMake 3.25+, compilador C++20 (MSVC 2022 / Clang 17+)
- SDL3, Vulkan SDK (post-scaffold)

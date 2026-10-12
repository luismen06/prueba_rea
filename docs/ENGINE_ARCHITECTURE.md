# Motor de Mundo Abierto — Análisis de Arquitectura y Diseño

> Basado en análisis estático del binario `gta_sa.exe` 1.0 US (x86, PE)
> SHA-256: `b81983d080adcedf328bc7a97d6bf36c2ebf03e24d9ffd67a61e68abd1298a22`
>
> **Objeto**: extraer *patrones de arquitectura* (motor, streaming, entidades,
> física, scripting) para construir un motor **original** en C++.
> No se copian assets, nombres, historia ni código de Rockstar Games.

## 1. Metodología

- Herramienta: REA (Reverse Engineering Anything) + Ghidra 12.1.4 (descompilación)
- Alcance: análisis estático del ejecutable original v1.0 US
- Restricción de IP: solo patrones de diseño; implementación y nombres propios

## 2. Hallazgos (pendiente de análisis)

### 2.1 Estructura PE y módulos importados

### 2.2 Motor gráfico (RenderWare)

### 2.3 Sistema de streaming de mundo

### 2.4 Entidades y jerarquía de clases

### 2.5 Física y vehículos

### 2.6 Scripting y misiones

### 2.7 Audio

### 2.8 Red (multijugador)

## 3. Diseño del motor original

### 3.1 Stack tecnológico

- C++20, CMake, SDL3 + Vulkan/OpenGL, Bullet Physics, OpenAL

### 3.2 Módulos

| Módulo | Responsabilidad | Análogo observado |
|--------|-----------------|-------------------|
| core | Bucle principal, jobs, memoria | — |
| render | Pipeline, materiales, LOD | RenderWare raster |
| world | Streaming por celdas | CStreaming |
| entities | Entidades y componentes | CPlaceable/CEntity |
| physics | Rigid bodies, vehículos | physics engine |
| script | VM de bytecode para misiones | SCM/CLEO |
| audio | Buses, distancias, occlusion | RW Audio |
| net | Replicación (futuro) | multiplayer |

### 3.3 Bucle de juego

## 4. Estado

- [x] Binario extraído e inventariado (REA)
- [ ] Descompilación Ghidra
- [ ] Mapa de subsistemas
- [ ] Skeleton del motor

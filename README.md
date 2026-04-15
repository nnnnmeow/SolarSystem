# SolarSystem

A real-time 3D Solar System visualization and physics simulator built with C++ and OpenGL. Renders the Sun and all eight planets with textured surfaces, realistic orbital mechanics driven by Newtonian gravity, and interactive camera controls.

<img src="https://img.shields.io/badge/OpenGL-3.3+-blue" alt="OpenGL"> <img src="https://img.shields.io/badge/C++-17-orange" alt="C++"> <img src="https://img.shields.io/badge/CMake-3.10+-green" alt="CMake">

## Features

- **Gravitational physics** — Newton's law of universal gravitation applied between all celestial bodies, updating velocities and positions each frame
- **Textured planets** — Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, and Neptune with individual surface textures
- **Axial rotation** — each planet spins on its own axis with configurable rotation speeds
- **Collision detection** — basic collision handling between planets
- **Interactive camera** — free-fly camera with mouse look and scroll-wheel zoom

## Controls

| Input | Action |
|---|---|
| `W` / `A` / `S` / `D` | Move forward / left / backward / right |
| `Q` / `E` | Move up / down |
| Right-click + mouse | Look around (yaw / pitch) |
| Scroll wheel | Zoom (FOV 1 - 45 degrees) |
| `ESC` | Exit |

## Tech Stack

- **C++** — core language
- **OpenGL 3.3+** — rendering API
- **GLFW** — window and input management
- **GLAD** — OpenGL function loader
- **GLM** — vector and matrix math
- **stb_image** — texture loading

## Project Structure

```
SolarSystem/
├── CMakeLists.txt
├── Space.sln                    # Visual Studio 2022 solution
├── Space/
│   ├── src/
│   │   ├── Source.cpp           # Main application and render loop
│   │   ├── Config.h             # Configuration definitions
│   │   ├── Planet.h             # Planet class (physics, rendering)
│   │   ├── Shader.h             # OpenGL shader wrapper
│   │   ├── Mesh.h               # VAO/VBO/EBO mesh wrapper
│   │   ├── glad.c               # GLAD loader implementation
│   │   └── stb_image.h          # Image loading library
│   └── Shaders/
│       ├── Vertex/
│       │   └── PlanetVertex.glsl
│       ├── Fragment/
│       │   └── PlanetFragment.glsl
│       └── Textures/            # Planet surface textures (.png)
└── Dependencies/
    ├── GLFW/
    ├── glad/
    └── glm/
```

## Building

### Prerequisites

- CMake 3.10+
- C++17 compatible compiler
- OpenGL 3.3+ capable GPU
- **Windows:** pre-built GLFW libraries for vc2022 are included
- **Linux/macOS:** install GLFW3 and OpenGL dev packages via your system package manager

### CMake

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

The build automatically copies the `Shaders` directory (including textures) to the output folder.

### Visual Studio 2022

1. Open `Space.sln`
2. Build the solution (`Ctrl+Shift+B`)
3. Run (`F5`)

## Author

Yanishevskiy Maksym

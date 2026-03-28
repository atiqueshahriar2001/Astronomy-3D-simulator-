# Astronomy 3D Simulator

A real-time 3D solar system simulator built with C++ and OpenGL/GLUT. Renders an animated solar system with the Sun, 8 planets, orbital paths, moons, Saturn's rings, and a twinkling starfield background.

## Features

- **Sun** with animated glow layers and pulsating rays
- **8 planets** (Mercury through Neptune) orbiting at different speeds and distances
- **Earth's moon** orbiting the Earth
- **Saturn's rings** rendered as a flat ring geometry
- **2000 twinkling stars** as a background starfield
- **Toggleable orbit paths** for all planets
- **Dynamic lighting** from the Sun's position
- **Interactive camera** with rotation, zoom, and height control

## Controls

| Key | Action |
|-----|--------|

| `W` / `S` | Move camera up / down |
| `A` / `D` | Rotate camera left / right |
| `+` / `-` | Zoom in / out |
| `Space` | Pause / Resume animation |
| `O` | Toggle orbit paths |
| `L` | Toggle lighting |
| `Esc` | Exit |

## Dependencies

- OpenGL
- GLU (OpenGL Utility Library)
- GLUT (OpenGL Utility Toolkit)

## Building

### Linux (GCC)

```bash
g++ main.cpp -o main -lGL -lGLU -lglut -lm
```

### Windows (MinGW)

```bash
g++ main.cpp -o main.exe -lfreeglut -lopengl32 -lglu32
```

> Make sure GLUT/freeglut development libraries are installed on your system.

## Running

```bash
./main
```

Or on Windows:

```bash
main.exe
```

## Planet Data

| Planet | Distance | Radius | Color |
|--------|----------|--------|-------|

| Mercury | 10 | 1.2 | Gray |
| Venus | 16 | 1.8 | Tan |
| Earth | 24 | 2.0 | Blue (has moon) |
| Mars | 32 | 1.5 | Red |
| Jupiter | 45 | 4.5 | Orange-brown |
| Saturn | 58 | 3.8 | Gold (has rings) |
| Uranus | 72 | 3.2 | Cyan |
| Neptune | 88 | 3.0 | Dark blue |

## License

This project is provided as-is for educational and personal use.

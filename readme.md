# Astronomy 2D Simulator

This project is a 2D computer graphics simulation of a solar-system-inspired astronomy scene developed with C++ and OpenGL (GLUT). It was designed to satisfy academic graphics course requirements by combining basic drawing primitives, raster graphics algorithms, geometric transformations, and animation in a single interactive application.

---

## Project Features

* Uses graphics primitives: points, lines, polygons, and circles
* Implements DDA line drawing
* Implements Bresenham line drawing
* Implements Midpoint Circle drawing
* Shows 9 celestial bodies: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, and Pluto
* Shows animated orbiting planets, moon motion, and sun pulse
* Demonstrates 2D transformations on a satellite: translation, rotation, scaling, and shearing on both axes
* Includes a starfield, orbit rings, asteroid belt, nebula background, planet labels, and interactive keyboard controls

---

## User Controls

| Key       | Action                     |
| --------- | -------------------------- |
| Arrow keys | Move satellite            |
| `R` / `T` | Rotate satellite +/-       |
| `Z` / `X` | Scale satellite +/-        |
| `H`       | Toggle X-axis shear        |
| `J`       | Toggle Y-axis shear        |
| `Space`   | Pause / Resume animation   |
| `Esc`     | Exit application           |

---

## Technologies Used

* **C++**
* **OpenGL**
* **GLU (OpenGL Utility Library)**
* **GLUT / FreeGLUT**

---

## Build Instructions

### 🐧 Linux (GCC)

```bash
g++ main.cpp -o main -lGL -lGLU -lglut -lm
./main
```

---

### 🪟 Windows (MinGW)

```bash
g++ main.cpp -o main.exe -lfreeglut -lopengl32 -lglu32
./main.exe
```

> ⚠️ Ensure **FreeGLUT / GLUT libraries** are properly installed.

---

## Assignment Mapping

* Basic primitives:
  stars use points, orbit/rays/tails use lines, satellite and nebula use polygons, planets/sun/orbits use circles
* Graphics algorithms:
  DDA, Bresenham, and Midpoint Circle are all implemented and used directly in the drawing code
* 2D transformations:
  translation, rotation, scaling, and shearing along both axes are applied to the satellite via 3x3 matrices
* Animation:
  planets orbit continuously, the moon revolves around Earth, and the sun pulses

## Visual Highlights

* Sun with pulsating rays drawn using line and circle primitives
* Labeled planets with different orbit radius, color, size, and speed
* Earth moon system and Saturn ring effect
* Animated asteroid belt with a controllable satellite
* Extra transformation demo panel with a controllable satellite

## Project Structure

* `main.cpp` contains the full OpenGL/GLUT implementation
* `main.exe` is the compiled Windows executable
* `readme.md` contains setup and project documentation

## Academic Relevance

This project demonstrates the practical application of core computer graphics concepts in a visually understandable way. It is suitable for course submission, demonstration, and viva presentation because the required algorithms and transformations are both implemented in code and visible in the final simulation output.

## License

This project is provided for:

* Educational purposes
* Personal experiments


## Here is how to set up the standalone compiler and FreeGLUT.

## Download the Standalone Compiler (WinLibs)

1. Go to WinLibs.com.
2. Scroll down to the "Release" section.
3. Look for UCRT runtime, Win64, and download the Zip archive (e.g., "GCC 14.2.0 + LLVM/Clang...").
4. Extract it: Unzip the folder to a simple location like C:\mingw64.

## Add to Environment Variables

1. Windows needs to know where g++.exe lives.
2. Open the Start Menu, type "Environment Variables", and select Edit the system environment variables.
3. Click Environment Variables > Select Path > Click Edit.
4. Click New and paste: C:\mingw64\bin
5. Click OK on all windows.


## Download & Place FreeGLUT

1. Since you are doing a manual setup, let's put FreeGLUT directly into your compiler folders so you don't have to keep copying files into every new project.
2. Download the freeglut 3.0.0 MSVC Package from Transmission Zero.
3. Move Headers: Copy everything inside the zip's include/GL folder into C:\mingw64\include\GL.
4. Move Libraries: Copy the .lib files from the zip's lib/x64 folder into C:\mingw64\lib.
5. Move the DLL: Copy freeglut.dll from the zip's bin/x64 folder into C:\Windows\System32 (this allows any OpenGL program you write to run globally).

## Download Link(GCC + MinGW)

https://www.winlibs.com

## Download Link (OpenGL + GLUT)

https://www.transmissionzero.co.uk/software/freeglut-devel

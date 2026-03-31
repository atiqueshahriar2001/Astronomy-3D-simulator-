# 🌌 Astronomy 3D Simulator

A **real-time 3D Solar System simulator** built using **C++ and OpenGL (GLUT)**.
This project visually represents the solar system with animated planets, realistic orbital motion, lighting, and a dynamic starfield.

---

## 🚀 Features

✨ **Core Highlights**

* 🌞 Realistic **Sun** with glowing animation & pulsating rays
* 🪐 **All 8 planets** orbiting with unique speed & distance
* 🌍 **Earth's Moon** with independent orbit
* 💫 **2000+ twinkling stars** for immersive background
* 🌀 **Saturn’s rings** with custom geometry
* 🔄 Toggleable **orbit paths**
* 💡 Dynamic **lighting system** from the Sun
* 🎮 Fully **interactive camera controls**

---

## 🎮 Controls

| Key       | Action                     |
| --------- | -------------------------- |
| `W` / `S` | Move camera up / down      |
| `A` / `D` | Rotate camera left / right |
| `+` / `-` | Zoom in / out              |
| `Space`   | Pause / Resume animation   |
| `O`       | Toggle orbit paths         |
| `L`       | Toggle lighting            |
| `Esc`     | Exit application           |

---

## 🧰 Technologies Used

* **C++**
* **OpenGL**
* **GLU (OpenGL Utility Library)**
* **GLUT / FreeGLUT**

---

## ⚙️ Installation & Build

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

## 🪐 Planet Configuration

| Planet  | Distance | Radius | Color           |
| ------- | -------- | ------ | --------------- |
| Mercury | 10       | 1.2    | Gray            |
| Venus   | 16       | 1.8    | Tan             |
| Earth   | 24       | 2.0    | Blue 🌍         |
| Mars    | 32       | 1.5    | Red 🔴          |
| Jupiter | 45       | 4.5    | Orange-brown    |
| Saturn  | 58       | 3.8    | Gold 🟡 (rings) |
| Uranus  | 72       | 3.2    | Cyan 🔵         |
| Neptune | 88       | 3.0    | Dark Blue 🔷    |

---

## 📸 Future Improvements (Optional Ideas)

* 🌌 Add **asteroid belt**
* 🌗 Planet **rotation on axis**
* ☄️ Add **comets**
* 🎥 Multiple **camera modes (top view / free fly)**
* 🌍 Realistic **textures (NASA textures)**

---

## 📄 License

This project is provided for:

* 🎓 Educational purposes
* 🧪 Personal experiments


## Here is how to set up the standalone compiler and FreeGLUT.

1. Download the Standalone Compiler (WinLibs)

Go to WinLibs.com.

Scroll down to the "Release" section.

Look for UCRT runtime, Win64, and download the Zip archive (e.g., "GCC 14.2.0 + LLVM/Clang...").

Extract it: Unzip the folder to a simple location like C:\mingw64.

2. Add to Environment Variables

Windows needs to know where g++.exe lives.

Open the Start Menu, type "Environment Variables", and select Edit the system environment variables.

Click Environment Variables > Select Path > Click Edit.

Click New and paste: C:\mingw64\bin

Click OK on all windows.

3. Download & Place FreeGLUT

Since you are doing a manual setup, let's put FreeGLUT directly into your compiler folders so you don't have to keep copying files into every new project.

Download the freeglut 3.0.0 MSVC Package from Transmission Zero.

Move Headers: Copy everything inside the zip's include/GL folder into C:\mingw64\include\GL.

Move Libraries: Copy the .lib files from the zip's lib/x64 folder into C:\mingw64\lib.

Move the DLL: Copy freeglut.dll from the zip's bin/x64 folder into C:\Windows\System32 (this allows any OpenGL program you write to run globally).

## Download Link(GCC + MinGW)

https://www.winlibs.com

## Download Link (OpenGL + GLUT)

https://www.transmissionzero.co.uk/software/freeglut-devel

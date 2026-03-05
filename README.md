# First-Semester-Project
A real-time 2D retro space shooter built in C++. Features procedural obstacle generation, coordinate-based collision detection, and multi-level difficulty scaling. Developed as a Semester 1 Programming Fundamentals project.

# Alien Ware - C++ Retro Space Shooter 🚀

**Alien Ware** is a terminal-based, real-time arcade game developed during my first semester at **FAST University**. It serves as a foundational project in my programming journey, demonstrating core concepts of procedural logic, real-time input handling, and game state management.

## 🕹️ Gameplay Features
- **Real-time Game Loop**: Handles movement, projectiles, and environmental hazards simultaneously.
- **Dynamic Difficulty**: As distance increases or bosses are defeated, spawn rates for asteroids and enemy fire speed up.
- **Collision System**: Custom logic for detecting player contact with health crates ($), stars (*), and asteroids (#).
- **Two-Level System**: Complete Level 1 by reaching 3000m or destroying 5 enemy ships to unlock the faster, harder Level 2.

## 🛠️ Technical Implementation
- **Language**: C++
- **Graphics**: Windows Console API (`SetConsoleTextAttribute` and `SetConsoleCursorPosition`) for flicker-free rendering.
- **Logic**:
  - **Procedural Spawning**: Uses `rand()` and time-based thresholds to generate game objects.
  - **Entity Tracking**: Manages active projectiles using multi-dimensional arrays for bullets and enemy fire.
  - **Input**: Asynchronous keyboard input using `<conio.h>`.

## 🎮 How to Play
1. **Movement**: `W/A/S/D` or Arrow Keys.
2. **Jump/Dash**: `J` key.
3. **Fire Bullets**: `Spacebar` or `B`.
4. **Objective**: Survive as long as possible, collect stars for score, and pick up crates ($) to regain lives.

## 📈 Retrospective
Looking back from my current 4th-semester perspective (Operating Systems and Assembly), this project was my first deep dive into manual resource management and the complexity of real-time systems. While currently written procedurally, it represents the logic-building phase that led to my interest in low-level systems architecture.

---
*Developed by Rayyan Shahzad (Raykh)*

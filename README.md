# 🤖 Autonomous Robot Vacuum Simulator (C++17)

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.10%2B-064F8C?logo=cmake&logoColor=white)
![Algorithm](https://img.shields.io/badge/Pathfinding-BFS%20Shortest%20Path-brightgreen)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

An object-oriented **C++17** autonomous robot vacuum simulator that navigates 2D grid environments with obstacles, plans optimal paths to dirty tiles using **Breadth-First Search (BFS)**, manages battery reserves, and autonomously returns to its charging dock before battery depletion.

---

## ✨ Key Features

- **BFS Shortest-Path Navigation:** Dynamically computes obstacle-free shortest paths to the nearest dirty tile (`*`) across arbitrary 2D room topologies (`#` walls, `.` clean floor, `D` charging dock).
- **Predictive Battery Management:** Before committing to a target tile, verifies that remaining battery capacity covers both the outbound path and the return trip to the charging dock (`D`).
- **Autonomous Dock Recharging:** Automatically routes back to the dock to recharge when battery reserves fall below the safe return threshold, then resumes cleaning until 100% coverage is achieved.
- **Detailed Telemetry & Grid Visualization:** Renders before/after ASCII room maps and outputs a full summary of tiles cleaned, total steps traveled, and recharge cycles.

---

## 🗂️ Project Structure

```text
Robot-Vacuum-Simulator/
├── data/
│   └── room_map.txt         # 2D room layout (# = Wall, D = Dock, * = Dirt, . = Clean)
├── include/
│   └── RobotVacuum.hpp      # RobotVacuum class & Position struct declarations
├── src/
│   ├── RobotVacuum.cpp      # BFS pathfinder, battery controller & grid renderer
│   └── main.cpp             # Simulator entry point
├── CMakeLists.txt           # Cross-platform CMake build configuration
├── LICENSE                  # MIT License
└── README.md                # Project documentation
```

---

## 🚀 Build & Run Instructions

### Build with CMake
```bash
git clone https://github.com/Firatdondar/Robot-Vacuum-Simulator.git
cd Robot-Vacuum-Simulator
mkdir build && cd build
cmake ..
cmake --build .
./RobotVacuumSimulator
```

### Build Directly with g++
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/RobotVacuum.cpp -o RobotVacuumSimulator
./RobotVacuumSimulator
```

---

## 🖥️ Sample Simulation Output

```text
=========================================================
      AUTONOMOUS ROBOT VACUUM SIMULATOR (BFS ROUTER)     
=========================================================

[+] Cleaned tile at (1, 4) | Battery: 42% | Cleaned: 1/12
[+] Cleaned tile at (2, 3) | Battery: 40% | Cleaned: 2/12
...
[!] Low battery (13%). Returning to charging dock (6 steps)...
[+] Battery recharged to 45% at Dock (1, 1).
...
[*] All reachable dirty tiles cleaned! Returning to dock...

=========================================================
                    SIMULATION SUMMARY                   
=========================================================
  Dirty Tiles Cleaned    : 12 / 12
  Total Steps Traveled   : 64
  Dock Recharge Cycles   : 1
  Remaining Battery      : 26 / 45
  Final Robot Position   : Dock (1, 1)
=========================================================
```

---

## 📄 License

Distributed under the **MIT License**. See [`LICENSE`](LICENSE) for details.

#include "RobotVacuum.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    RobotVacuum robot(45);

    const std::vector<std::string> candidatePaths = {
        "data/room_map.txt",
        "../data/room_map.txt",
        "room_map.txt"
    };

    bool loaded = false;
    for (const auto& path : candidatePaths) {
        if (robot.loadRoomMap(path)) {
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        std::cerr << "[!] ERROR: Could not load 'data/room_map.txt'.\n";
        return 1;
    }

    robot.runSimulation();
    return 0;
}

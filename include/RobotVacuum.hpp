#ifndef ROBOT_VACUUM_HPP
#define ROBOT_VACUUM_HPP

#include <string>
#include <utility>
#include <vector>

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

class RobotVacuum {
public:
    explicit RobotVacuum(int maxBattery = 60);

    bool loadRoomMap(const std::string& filePath);
    void runSimulation();
    void displayGrid() const;
    void printReport() const;

private:
    std::vector<std::string> grid_;
    Position currentPos_{0, 0};
    Position dockPos_{0, 0};
    int maxBattery_;
    int battery_;
    int totalSteps_{0};
    int tilesCleaned_{0};
    int totalDirtyTiles_{0};
    int rechargeCount_{0};

    std::vector<Position> findShortestPathBFS(const Position& start, const Position& target) const;
    bool findNearestDirtyTile(Position& outTarget, std::vector<Position>& outPath) const;
    void moveAlongPath(const std::vector<Position>& path);
};

#endif // ROBOT_VACUUM_HPP

#include "RobotVacuum.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>

RobotVacuum::RobotVacuum(int maxBattery)
    : maxBattery_(maxBattery), battery_(maxBattery) {}

bool RobotVacuum::loadRoomMap(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    grid_.clear();
    totalDirtyTiles_ = 0;
    std::string line;
    int r = 0;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) continue;

        for (int c = 0; c < static_cast<int>(line.size()); ++c) {
            if (line[c] == 'D') {
                dockPos_ = {r, c};
                currentPos_ = {r, c};
            } else if (line[c] == '*') {
                ++totalDirtyTiles_;
            }
        }
        grid_.push_back(line);
        ++r;
    }
    return !grid_.empty();
}

std::vector<Position> RobotVacuum::findShortestPathBFS(const Position& start, const Position& target) const {
    int rows = static_cast<int>(grid_.size());
    int cols = static_cast<int>(grid_[0].size());

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<Position>> parent(rows, std::vector<Position>(cols, {-1, -1}));
    std::queue<Position> q;

    q.push(start);
    visited[start.row][start.col] = true;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        if (cur == target) {
            std::vector<Position> path;
            for (Position at = target; !(at == start); at = parent[at.row][at.col]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = cur.row + dr[i];
            int nc = cur.col + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && grid_[nr][nc] != '#') {
                visited[nr][nc] = true;
                parent[nr][nc] = cur;
                q.push({nr, nc});
            }
        }
    }
    return {};
}

bool RobotVacuum::findNearestDirtyTile(Position& outTarget, std::vector<Position>& outPath) const {
    int rows = static_cast<int>(grid_.size());
    int cols = static_cast<int>(grid_[0].size());
    bool found = false;
    size_t bestDist = 999999;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid_[r][c] == '*') {
                auto path = findShortestPathBFS(currentPos_, {r, c});
                if (!path.empty() && path.size() < bestDist) {
                    bestDist = path.size();
                    outTarget = {r, c};
                    outPath = std::move(path);
                    found = true;
                }
            }
        }
    }
    return found;
}

void RobotVacuum::moveAlongPath(const std::vector<Position>& path) {
    for (const auto& step : path) {
        currentPos_ = step;
        --battery_;
        ++totalSteps_;
        if (grid_[currentPos_.row][currentPos_.col] == '*') {
            grid_[currentPos_.row][currentPos_.col] = '.';
            ++tilesCleaned_;
        }
    }
}

void RobotVacuum::runSimulation() {
    std::cout << "=========================================================\n";
    std::cout << "      AUTONOMOUS ROBOT VACUUM SIMULATOR (BFS ROUTER)     \n";
    std::cout << "=========================================================\n\n";
    std::cout << "[*] Initial Room Layout (D = Dock, * = Dirt, # = Wall):\n";
    displayGrid();

    Position nextTarget{};
    std::vector<Position> pathToDirt;

    while (findNearestDirtyTile(nextTarget, pathToDirt)) {
        auto pathFromTargetToDock = findShortestPathBFS(nextTarget, dockPos_);
        int requiredBattery = static_cast<int>(pathToDirt.size() + pathFromTargetToDock.size());

        if (battery_ < requiredBattery && !(currentPos_ == dockPos_)) {
            auto returnPath = findShortestPathBFS(currentPos_, dockPos_);
            std::cout << "[!] Low battery (" << battery_ << "%). Returning to charging dock ("
                      << returnPath.size() << " steps)...\n";
            moveAlongPath(returnPath);
            battery_ = maxBattery_;
            ++rechargeCount_;
            std::cout << "[+] Battery recharged to " << battery_ << "% at Dock ("
                      << dockPos_.row << ", " << dockPos_.col << ").\n";
            continue;
        }

        moveAlongPath(pathToDirt);
        std::cout << "[+] Cleaned tile at (" << currentPos_.row << ", " << currentPos_.col
                  << ") | Battery: " << battery_ << "% | Cleaned: "
                  << tilesCleaned_ << "/" << totalDirtyTiles_ << "\n";
    }

    if (!(currentPos_ == dockPos_)) {
        auto finalReturn = findShortestPathBFS(currentPos_, dockPos_);
        std::cout << "\n[*] All reachable dirty tiles cleaned! Returning to dock...\n";
        moveAlongPath(finalReturn);
    }

    std::cout << "\n[*] Final Room Layout:\n";
    displayGrid();
    printReport();
}

void RobotVacuum::displayGrid() const {
    for (int r = 0; r < static_cast<int>(grid_.size()); ++r) {
        std::cout << "    ";
        for (int c = 0; c < static_cast<int>(grid_[r].size()); ++c) {
            if (currentPos_.row == r && currentPos_.col == c && !(currentPos_ == dockPos_)) {
                std::cout << 'R' << ' ';
            } else {
                std::cout << grid_[r][c] << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void RobotVacuum::printReport() const {
    std::cout << "=========================================================\n";
    std::cout << "                    SIMULATION SUMMARY                   \n";
    std::cout << "=========================================================\n";
    std::cout << "  Dirty Tiles Cleaned    : " << tilesCleaned_ << " / " << totalDirtyTiles_ << "\n";
    std::cout << "  Total Steps Traveled   : " << totalSteps_ << "\n";
    std::cout << "  Dock Recharge Cycles   : " << rechargeCount_ << "\n";
    std::cout << "  Remaining Battery      : " << battery_ << " / " << maxBattery_ << "\n";
    std::cout << "  Final Robot Position   : Dock (" << currentPos_.row << ", " << currentPos_.col << ")\n";
    std::cout << "=========================================================\n";
}

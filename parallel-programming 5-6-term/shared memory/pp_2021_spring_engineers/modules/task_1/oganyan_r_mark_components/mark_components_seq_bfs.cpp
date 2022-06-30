// Copyright 2021 Oganyan Robert

#include <iostream>
#include "../../modules/task_1/oganyan_r_mark_components/mark_components_seq_bfs.h"

static const std::vector<std::pair<int, int>> directions{
        {-1, 0},
        {0,  -1},
        {0,  1},
        {1,  0},
};


void bfs(std::vector<int> *img, std::pair<int, int> start,
           int *number, int width, int height) {
    if ((*img)[start.first * width + start.second] != 1) {
        return;
    }
    std::queue<std::pair<int, int>> q;
    q.push({start});
    (*img)[start.first * width + start.second] = ++(*number);
    while (!q.empty()) {
        auto cur{q.front()};
        q.pop();
        for (auto &neighbor : directions) {
            if (cur.first + neighbor.first >= height || cur.first + neighbor.first < 0
                || cur.second + neighbor.second >= width || cur.second + neighbor.second < 0) {
                continue;
            }
            if ((*img)[(cur.first + neighbor.first) * width + cur.second + neighbor.second] == 1) {
                q.push({(cur.first + neighbor.first), cur.second + neighbor.second});
                (*img)[(cur.first + neighbor.first) * width + cur.second + neighbor.second] = (*number);
            }
        }
    }
    return;
}


int MarkComponentsSeqBfs(std::vector<int> *img, int height, int width) {
    if ((*img).size() == 0) {
        throw std::invalid_argument("Size of the image cant be negative");
    }
    if (static_cast<int>((*img).size()) != width * height) {
        throw std::invalid_argument("Size of the image is not correct");
    }
    int count_comp{1};
    for (int i{0}; i < height; ++i) {
        for (int j{0}; j < width; ++j) {
            bfs(img, {i, j}, &count_comp, width, height);
        }
    }
    return count_comp - 1;
}

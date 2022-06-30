// Copyright 2021 Oganyan Robert

#include "../../modules/task_4/oganyan_r_mark_components_std/mark_components_std.h"
#include <iostream>
#include "../../3rdparty/unapproved/unapproved.h"

static const std::vector<std::pair<int, int>> directions{
        {-1, 0},
        {0,  -1},
        {0,  1},
        {1,  0},
};

int MarkComponentsParStd(std::vector<int> *img_new, int height, int width, int num_proc) {
    if ((*img_new).size() == 0) {
        throw std::invalid_argument("Size of the image cant be negative");
    }
    if (static_cast<int>((*img_new).size()) != width * height) {
        throw std::invalid_argument("Size of the image is not correct");
    }
    int count_comp{0};
    Disjoint_Set_Union<int> dsu(height * width);
    dsu.InitStd(num_proc);

    std::vector<std::thread> threads;
    threads.reserve(num_proc);


    int div = height / num_proc;
    int mod = height % num_proc;

    int last = 0;


    for (int proc = 0; proc < num_proc; ++proc) {
        threads.emplace_back([&img_new, width, height, &dsu, proc, div, mod, last]() {
            int balance = (proc < mod) ? 1 : 0;
            for (int i = last; i < last + div + balance; ++i) {
                for (int j = 0; j < width; ++j) {
                    if ((*img_new)[i * width + j]) {
                        for (auto &neighbor : directions) {
                            if (i + neighbor.first >= height || i + neighbor.first < 0
                                || j + neighbor.second >= width || j + neighbor.second < 0) {
                                continue;
                            }
                            int cur = (i + neighbor.first) * width + j + neighbor.second;
                            if ((*img_new)[cur] == 0) {
                                continue;
                            }
                            dsu.union_sets(i * width + j, cur);
                        }
                    }
                }
            }
        });
        last += div + ((proc < mod) ? 1 : 0);
    }

    for (auto &thread : threads) {
        thread.join();
    }


    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cur = i * width + j;
            if ((*img_new)[cur]) {
                (*img_new)[cur] = dsu.find_set(cur, cur) + 1;
            }
            if ((*img_new)[cur] == cur + 1) {
                count_comp++;
            }
        }
    }

    return count_comp;
}

int MarkComponentsSeq(std::vector<int> *img_new, int height, int width) {
    if ((*img_new).size() == 0) {
        throw std::invalid_argument("Size of the image cant be negative");
    }
    if (static_cast<int>((*img_new).size()) != width * height) {
        throw std::invalid_argument("Size of the image is not correct");
    }
    int count_comp{0};
    Disjoint_Set_Union<int> dsu(height * width);
    dsu.Init();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if ((*img_new)[i * width + j]) {
                for (auto &neighbor : directions) {
                    if (i + neighbor.first >= height || i + neighbor.first < 0
                        || j + neighbor.second >= width || j + neighbor.second < 0) {
                        continue;
                    }
                    int cur = (i + neighbor.first) * width + j + neighbor.second;
                    if ((*img_new)[cur] == 0) {
                        continue;
                    }
                    dsu.union_sets(i * width + j, cur);
                }
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cur = i * width + j;
            if ((*img_new)[cur]) {
                (*img_new)[cur] = dsu.find_set(cur, cur) + 1;
            }
            if ((*img_new)[cur] == cur + 1) {
                count_comp++;
            }
        }
    }

    return count_comp;
}

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



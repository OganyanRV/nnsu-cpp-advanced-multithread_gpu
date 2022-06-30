// Copyright 2021 Oganyan Robert


#ifndef MODULES_TASK_1_OGANYAN_R_MARK_COMPONENTS_MARK_COMPONENTS_H_
#define MODULES_TASK_1_OGANYAN_R_MARK_COMPONENTS_MARK_COMPONENTS_H_

#include <vector>
#include <cstdint>
#include <queue>
#include <stdexcept>
#include <utility>

int MarkComponentsSeqBfs(std::vector<int> *img,
                   int height, int width);


void bfs(std::vector<int> *img, std::pair<int, int> start,
         int *number, int width, int height);

#endif  //  MODULES_TASK_1_OGANYAN_R_MARK_COMPONENTS_MARK_COMPONENTS_H_

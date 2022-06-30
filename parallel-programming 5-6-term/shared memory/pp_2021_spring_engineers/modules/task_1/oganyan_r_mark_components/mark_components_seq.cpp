
#include "../../modules/task_1/oganyan_r_mark_components/mark_components_seq.h"

static const std::vector<std::pair<int, int>> directions{
        // {-1, -1},
        {-1, 0},
        //  {-1, 1},
        {0,  -1},
        {0,  1},
        // {1,  -1},
        {1,  0},
        // {1,  1},
};


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

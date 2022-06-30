// Copyright 2021 Oganyan Robert

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <chrono>
#include "../../modules/task_1/oganyan_r_mark_components/mark_components_seq_bfs.h"
#include "mark_components_seq.h"


TEST(Comparing_to_a_sample, rectangle_image) {
    uint16_t width = 3;
    uint16_t height = 5;
    std::vector<int> source_image{
            0, 1, 1,
            0, 0, 0,
            1, 1, 1,
            0, 1, 0,
            0, 0, 0,
    };
    std::vector<int> result_image{
            0, 2, 2,
            0, 0, 0,
            3, 3, 3,
            0, 3, 0,
            0, 0, 0,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeqBfs(&source_image, height, width));
    ASSERT_EQ(res_image, 2);
}

TEST(Wrong_data, wrong_img_2) {
    std::vector<int> source_image{};
    std::vector<int> result_image{};
    uint16_t width = 5;
    uint16_t height = 2;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}


TEST(Comparing_to_a_sample, square_image_1) {
    uint16_t width = 6;
    uint16_t height = 6;
    std::vector<int> source_image{
            0, 1, 0, 0, 0, 0,
            1, 1, 1, 0, 0, 1,
            0, 0, 0, 0, 1, 1,
            0, 1, 0, 1, 0, 0,
            1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1,
    };
    std::vector<int> result_image{
            0, 2, 0, 0, 0, 0,
            2, 2, 2, 0, 0, 3,
            0, 0, 0, 0, 3, 3,
            0, 4, 0, 5, 0, 0,
            4, 4, 4, 0, 0, 0,
            0, 0, 0, 0, 6, 6,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeqBfs(&source_image, height, width));
    ASSERT_EQ(res_image, 5);
}

TEST(Wrong_data, wrong_width_and_height_3) {
    std::vector<int> source_image{
            0, 0, 1,
            1, 0, 1,
            0, 0, 0,
    };
    std::vector<int> result_image{
            0, 0, 2,
            3, 0, 2,
            0, 0, 0,
    };
    uint16_t width = 1;
    uint16_t height = 3;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}

TEST(Comparing_to_a_big_sample, square_image_1) {
    uint16_t width = 15;
    uint16_t height = 15;
    std::vector<int> source_image{
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    std::vector<int> result_image{
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeqBfs(&source_image, height, width));
    ASSERT_EQ(res_image, 1);
}

TEST(Wrong_data, wrong_width_and_height_1) {
    std::vector<int> source_image = {
            0, 1, 1,
            1, 0, 0,
            0, 0, 0,
    };
    std::vector<int> result_image = {
            0, 2, 2,
            3, 0, 0,
            0, 0, 0,
    };
    uint16_t width = 4;
    uint16_t height = 3;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}

TEST(Comparing_to_a_big_sample, square_image_2) {
    uint16_t width = 10;
    uint16_t height = 10;
    std::vector<int> source_image{
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
            1, 0, 1, 0, 0, 0, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 0, 0, 0, 1, 0, 1,
            1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    std::vector<int> result_image{
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
            2, 0, 3, 3, 3, 3, 3, 3, 0, 2,
            2, 0, 3, 0, 0, 0, 0, 3, 0, 2,
            2, 0, 3, 0, 4, 4, 0, 3, 0, 2,
            2, 0, 3, 0, 4, 4, 0, 3, 0, 2,
            2, 0, 3, 0, 0, 0, 0, 3, 0, 2,
            2, 0, 3, 3, 3, 3, 3, 3, 0, 2,
            2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeqBfs(&source_image, height, width));
    ASSERT_EQ(res_image, 3);
}

TEST(Wrong_data, wrong_img) {
    std::vector<int> source_image;
    std::vector<int> result_image;
    uint16_t width = 0;
    uint16_t height = 0;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}


TEST(Comparing_to_a_sample, square_image_2) {
    uint16_t width = 5;
    uint16_t height = 5;
    std::vector<int> source_image{
            0, 1, 0, 1, 1,
            0, 0, 0, 1, 1,
            1, 0, 0, 0, 1,
            0, 0, 1, 1, 1,
            0, 0, 0, 0, 1,
    };
    std::vector<int> result_image{
            0, 2, 0, 3, 3,
            0, 0, 0, 3, 3,
            4, 0, 0, 0, 3,
            0, 0, 3, 3, 3,
            0, 0, 0, 0, 3,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeqBfs(&source_image, height, width));
    ASSERT_EQ(res_image, 3);
}

TEST(Wrong_data, wrong_img_1) {
    std::vector<int> source_image{};
    std::vector<int> result_image{};
    uint16_t width = 0;
    uint16_t height = 0;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}

TEST(Wrong_data, wrong_width_and_height_2) {
    std::vector<int> source_image{
            0, 1, 1,
            0, 0, 1,
            1, 1,
    };
    std::vector<int> result_image{
            0, 2, 2,
            0, 0, 2,
            2, 2,
    };
    uint16_t width = 3;
    uint16_t height = 3;
    ASSERT_ANY_THROW(MarkComponentsSeqBfs(&source_image, height, width));
}

TEST(Comparing_to_a_big_sample, square_image_3) {
    uint16_t width = 10;
    uint16_t height = 10;
    std::vector<int> source_image{
            0, 1, 0, 1, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 1, 1, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
            1, 0, 1, 1, 1, 1, 1, 0, 0, 1,
            0, 1, 1, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 1, 1, 0, 1, 0, 1, 1,
            1, 0, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 0, 1, 0, 1, 0, 1, 0, 1, 1,
    };
    std::vector<int> result_image{
            0, 2, 0, 3, 0, 4, 4, 0, 0, 0,
            0, 0, 0, 2, 2, 0, 4, 4, 0, 0,
            0, 0, 5, 5, 0, 6, 0, 0, 7, 0,
            8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            8, 0, 9, 0, 9, 9, 9, 9, 0, 0,
            8, 0, 9, 9, 9, 9, 9, 0, 0, 10,
            0, 9, 9, 0, 0, 0, 0, 11, 0, 0,
            0, 0, 0, 12, 12, 0, 13, 0, 14, 14,
            15, 0, 16, 0, 0, 0, 0, 0, 14, 14,
            15, 0, 16, 0, 17, 0, 18, 0, 14, 14,
    };
    int res_image;
    ASSERT_NO_THROW(res_image = MarkComponentsSeq(&source_image, height, width));
    ASSERT_EQ(res_image, 17);
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

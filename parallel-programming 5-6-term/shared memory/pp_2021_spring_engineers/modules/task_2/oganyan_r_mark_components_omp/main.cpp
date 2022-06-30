// Copyright 2021 Oganyan Robert


#include <gtest/gtest.h>
#include <omp.h>
#include <random>
#include "../../modules/task_2/oganyan_r_mark_components_omp/mark_components_omp.h"



void Create_Custom_Test(int height, int width) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<unsigned int> distribution(0, 1);
    std::vector<int> seq(width * height);
    for (int i = 0; i < width * height; ++i) {
        seq[i] = distribution(generator);
    }
    auto paral = seq;
    auto seq2 = seq;

    std::cout << height << "x" << width << ": \n";

    double start = omp_get_wtime();
    int ans = MarkComponentsSeqBfs(&seq, height, width);
    double end = omp_get_wtime();
    std::cout << "Sequential fast: " << (end - start) << "\n";
    std::cout << " \n";

    start = omp_get_wtime();
    auto ans2 = MarkComponentsSeq(&seq2, height, width);
    end = omp_get_wtime();
    std::cout << "Sequential normal: " << (end - start) << "\n";
    std::cout << " \n";

    start = omp_get_wtime();
    auto ans3 = MarkComponentsParOmp(&paral, height, width, 4);
    end = omp_get_wtime();
    std::cout << "Parallel: " << (end - start) << "\n";

    std::cout << " \n";
    ASSERT_EQ(ans3, ans2);
    ASSERT_EQ(ans, ans3);
}


TEST(Comparing_random_img, small_image) {
    Create_Custom_Test(10, 10);
}

TEST(Comparing_random_img, small_image_2) {
    Create_Custom_Test(100, 100);
}

TEST(Comparing_random_img, middle_image) {
    Create_Custom_Test(500, 500);
}

TEST(Comparing_random_img, middle_image_2) {
    Create_Custom_Test(750, 750);
}

TEST(Comparing_random_img, middle_image_3) {
    Create_Custom_Test(1000, 1000);
}

TEST(Comparing_random_img, big_image) {
    Create_Custom_Test(2000, 2000);
}

TEST(Comparing_random_img, big_image_2) {
    Create_Custom_Test(3000, 3000);
}

TEST(Comparing_random_img, big_image_3) {
    Create_Custom_Test(5000, 5000);
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

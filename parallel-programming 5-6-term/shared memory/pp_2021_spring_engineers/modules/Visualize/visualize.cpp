//// Copyright 2021 Oganyan Robert

#include <vector>
#include <iostream>
#include <map>
#include <random>
#include <iomanip>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <omp.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/task_scheduler_init.h>
#include <thread>
#include <queue>


#include "../../modules/task_1/oganyan_r_mark_components/mark_components_seq_bfs.h"
#include "../../modules/task_1/oganyan_r_mark_components/mark_components_seq.h"
#include "../../modules/task_2/oganyan_r_mark_components_omp/mark_components_omp.h"
#include "../../modules/task_3/oganyan_r_mark_components_tbb/mark_components_tbb.h"
#include "../../modules/task_4/oganyan_r_mark_components_std/mark_components_std.h"

std::vector<int> make_img(int height, int width) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<unsigned int> distribution(0, 800);
    std::vector<int> seq(width * height);
    for (int i = 0; i < width * height; ++i) {
        auto val = distribution(generator);
        //  So I want "0" (white) to be most likely
        if (val % 2 == 0) val = 0;
        seq[i] = val;
    }
    return seq;
}

void convert_to_zeroone(cv::Mat *img) {
    for (int i = 0; i < (*img).rows; ++i) {
        for (int j = 0; j < (*img).cols; ++j) {
            if ((*img).at<uchar>(i, j) < 255) {
                (*img).at<uchar>(i, j) = 1;
            }
            if ((*img).at<uchar>(i, j) == 255) {
                (*img).at<uchar>(i, j) = 0;
            }
        }
    }
}


void convert_to_rdm_color(std::vector<cv::Vec3b> *img) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<unsigned int> distribution(0, 255);
    std::map<uint, cv::Vec3b> colors;
    for (uint64_t i = 0; i < (*img).size(); ++i) {
        int cur_num = (*img)[i][0];
        if (cur_num >= 1) {
            if (colors.find(cur_num) == colors.end()) {
                cv::Vec3b result;
                for (int j = 0; j < 3; ++j) {
                    result[j] = distribution(generator);
                }
                colors[cur_num] = result;
                (*img)[i] = result;
            } else {
                (*img)[i] = colors[cur_num];
            }
        } else {
            (*img)[i] = {255, 255, 255};
        }
    }
    return;
}

int main(int argc, char *argv[]) {
    std::cout << "Printing arvg info: " << "\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << "\n";
    }

    if (argc < 3) {
        std::cout << "Wrong input" << "\n";
        return 0;
    }

//      cv::Mat img = imread(argv[1], cv::IMREAD_GRAYSCALE);

    //  or this
    int h = 5000;
    int w = h;
    auto img_vector = make_img(w, h);
    std::vector<cv::Vec3b> temp(img_vector.begin(), img_vector.end());
    convert_to_rdm_color(&temp);
    cv::Mat img(h, w, CV_8UC3, temp.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/curent_testing.png", img,
                {cv::IMWRITE_JPEG_QUALITY});

    img = imread("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/curent_testing.png",
                 cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        std::cout << "Wrong data" << "\n";
        return 0;
    }

    convert_to_zeroone(&img);
    // Sequential

    std::vector<int> array_seq(img.datastart, img.dataend);
    auto start = std::chrono::high_resolution_clock::now();
    MarkComponentsSeqBfs(&array_seq, img.rows, img.cols);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Fast Sequential: " << std::setw(9) << diff.count() << "\n";
    std::vector<cv::Vec3b> result_image_seq(array_seq.begin(), array_seq.end());
    convert_to_rdm_color(&result_image_seq);
    cv::Mat final_img_seq(img.rows, img.cols, CV_8UC3, result_image_seq.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/seq.png", final_img_seq,
                {cv::IMWRITE_JPEG_QUALITY});

    // Sequential normal
    std::vector<int> array_seq_0(img.datastart, img.dataend);
    start = std::chrono::high_resolution_clock::now();
    MarkComponentsSeq(&array_seq_0, img.rows, img.cols);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Normal Sequential: " << std::setw(9) << diff.count() << "\n";
    std::vector<cv::Vec3b> result_image_seq_0(array_seq_0.begin(), array_seq_0.end());
    convert_to_rdm_color(&result_image_seq_0);
    cv::Mat final_img_seq_0(img.rows, img.cols, CV_8UC3, result_image_seq_0.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/seq_normal.png", final_img_seq_0,
                {cv::IMWRITE_JPEG_QUALITY});

    // TBB

    std::vector<int> array_tbb(img.datastart, img.dataend);
    start = std::chrono::high_resolution_clock::now();
    MarkComponentsParTbb(&array_tbb, img.rows, img.cols, 4);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "TBB: " << std::setw(9) << diff.count() << "\n";
    std::vector<cv::Vec3b> result_image_tbb(array_tbb.begin(), array_tbb.end());
    convert_to_rdm_color(&result_image_tbb);
    cv::Mat final_img_tbb(img.rows, img.cols, CV_8UC3, result_image_tbb.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/tbb.png", final_img_tbb,
                {cv::IMWRITE_JPEG_QUALITY});

    // OMP

    std::vector<int> array_omp(img.datastart, img.dataend);
    start = std::chrono::high_resolution_clock::now();
    MarkComponentsParOmp(&array_omp, img.rows, img.cols, 4);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "OMP: " << std::setw(9) << diff.count() << "\n";
    std::vector<cv::Vec3b> result_image_omp(array_omp.begin(), array_omp.end());
    convert_to_rdm_color(&result_image_omp);
    cv::Mat final_img_omp(img.rows, img.cols, CV_8UC3, result_image_omp.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/omp.png", final_img_omp,
                {cv::IMWRITE_JPEG_QUALITY});



    // STD

    std::vector<int> array_std(img.datastart, img.dataend);
    start = std::chrono::high_resolution_clock::now();
    MarkComponentsParStd(&array_omp, img.rows, img.cols, 4);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "STD: " << std::setw(9) << diff.count() << "\n";
    std::vector<cv::Vec3b> result_image_std(array_std.begin(), array_std.end());
    convert_to_rdm_color(&result_image_std);
    cv::Mat final_img_std(img.rows, img.cols, CV_8UC3, result_image_std.data());
    cv::imwrite("/home/ogrob/pp_2021_spring_engineers/modules/Visualize/results/std.png", final_img_std,
                {cv::IMWRITE_JPEG_QUALITY});


    return 0;
}

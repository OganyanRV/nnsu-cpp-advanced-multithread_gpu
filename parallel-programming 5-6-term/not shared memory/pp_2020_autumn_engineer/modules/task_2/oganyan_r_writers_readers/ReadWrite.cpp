//  Copyright 2020 by Oganyan Robert

#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_2/oganyan_r_writers_readers/ReadWrite.h"

void sleep(int koef = 1) {
    auto start_time = MPI_Wtime();
    std::mt19937 gen;
    gen.seed(static_cast<int>(time(0)));
    double time = gen() % 5;
    time /= 50;
    time *= koef;
    while (time > MPI_Wtime() - start_time) {}
}

int randel() {
    std::random_device random_device;
    std::mt19937 random_generator(random_device());
    auto rdm = std::uniform_int_distribution<size_t>(0, 1000)(random_generator);
    return rdm % 1000;
}

int randop() {
    std::random_device random_device;
    std::mt19937 random_generator(random_device());
    auto rdm = std::uniform_int_distribution<size_t>(0, 1000)(random_generator);
    return rdm % 2;
}

int read_write(int op_cnt) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int res = 0;
        int koef = (op_cnt + 1) / 2;
        if (size > 10) {
            int optimise = (size + 9) / 10;
            koef /= size * optimise;
        }
        if (rank == 0) {
        res = library(koef * (size - 1));
    }
    sleep();
    if (rank != 0) {
        for (int i = 0; i < koef; ++i) {
            if (randop()) {
                writer();
            } else {
                reader();
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return res;
}

const int ANY_TAG = 0;
const int WRITE_TAG = 1;
const int WRITE_FINISH_TAG = 2;
const int READ_TAG = 3;
const int READ_FINISH_TAG = 4;

int library(int op_cnt) {
    int access_wr = 1;
    int access_rd = 1;
    int access = 0;
    int cur_op = 0;
    int data = 0;
    int arereading = 0;
    int arewriting = 0;
    int request;
    MPI_Status status;
    while (cur_op != op_cnt) {
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (arereading == 0 && arewriting == 0) {
            access_wr = 1;
        }
        if (status.MPI_TAG == WRITE_TAG) {
            access = 0;
            if (access_wr) {
                access = 1;
                access_wr = access_rd = 0;
                ++arewriting;
                MPI_Send(&access, 1, MPI_INT, status.MPI_SOURCE, ANY_TAG, MPI_COMM_WORLD);
            } else {
                MPI_Send(&access, 1, MPI_INT, status.MPI_SOURCE, ANY_TAG, MPI_COMM_WORLD);
            }
        } else if (status.MPI_TAG == WRITE_FINISH_TAG) {
            access = 1;
            data = request;
            //  std::cout << "The new data is" << data << std::endl;
            access_rd = access_wr = 1;
            --arewriting;
            ++cur_op;
        } else if (status.MPI_TAG == READ_TAG) {
            access = 0;
            if (access_rd) {
                arereading++;
                access_wr = 0;
                access = 1;
                MPI_Send(&access, 1, MPI_INT, status.MPI_SOURCE, ANY_TAG, MPI_COMM_WORLD);
                //  std::cout << "Sending data to reader " << data << std::endl;
                MPI_Send(&data, 1, MPI_INT, status.MPI_SOURCE, ANY_TAG, MPI_COMM_WORLD);
            } else {
                MPI_Send(&access, 1, MPI_INT, status.MPI_SOURCE, ANY_TAG, MPI_COMM_WORLD);
            }
        } else if (status.MPI_TAG == READ_FINISH_TAG) {
            arereading--;
            ++cur_op;
        }
    }
    return cur_op - op_cnt;
}

void writer() {
    int data, request = 0;
    MPI_Status status;
    while (!request) {
        sleep();
        MPI_Send(nullptr, 0, MPI_INT, 0, WRITE_TAG, MPI_COMM_WORLD);
        MPI_Recv(&request, 1, MPI_INT, 0, ANY_TAG, MPI_COMM_WORLD, &status);
    }
    data = randel();
    sleep(3);
    //  std::cout << "Writing data " << data << std::endl;
    MPI_Send(&data, 1, MPI_INT, 0, WRITE_FINISH_TAG, MPI_COMM_WORLD);
}

void reader() {
    int data, request = 0;
    MPI_Status status;
    while (!request) {
        sleep();
        MPI_Send(&request, 1, MPI_INT, 0, READ_TAG, MPI_COMM_WORLD);
        MPI_Recv(&request, 1, MPI_INT, 0, ANY_TAG, MPI_COMM_WORLD, &status);
    }
    MPI_Recv(&data, 1, MPI_INT, 0, ANY_TAG, MPI_COMM_WORLD, &status);
    sleep(2);
    //  std::cout << 'Received data " << data << std::endl;
    MPI_Send(nullptr, 0, MPI_INT, 0, READ_FINISH_TAG, MPI_COMM_WORLD);
}



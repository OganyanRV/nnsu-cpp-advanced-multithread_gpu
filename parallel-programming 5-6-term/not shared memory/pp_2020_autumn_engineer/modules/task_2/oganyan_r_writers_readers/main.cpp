// Copyright 2020 by Oganyan Robert

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "../../../modules/task_2/oganyan_r_writers_readers/ReadWrite.h"

TEST(Parallel_Operations_MPI, Test_tiny) {
    int count = 2;
    ASSERT_EQ(read_write(count), 0);
}

TEST(Parallel_Operations_MPI, Test_small) {
    int count = 5;
    ASSERT_EQ(read_write(count), 0);
}

TEST(Parallel_Operations_MPI, Test_medium) {
    int count = 12;
    ASSERT_EQ(read_write(count), 0);
}

TEST(Parallel_Operations_MPI, Test_big) {
    int count = 30;
    ASSERT_EQ(read_write(count), 0);
}

TEST(Parallel_Operations_MPI, Test_large) {
    int count = 80;
    ASSERT_EQ(read_write(count), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

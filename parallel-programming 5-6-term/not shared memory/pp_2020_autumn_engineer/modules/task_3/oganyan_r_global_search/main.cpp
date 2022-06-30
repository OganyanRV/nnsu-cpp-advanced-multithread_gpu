//  Copyright by Oganyan Robert 2020


#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include <iostream>

#include "../../../modules/task_3/oganyan_r_global_search/functions.h"
#include "../../../modules/task_3/oganyan_r_global_search/global_search.h"

using std::cout;
using std::endl;

void CreateTest(int num_fun,
                double x_left, double x_right, double y_left, double y_right,
                double ans_minimum, dpair ans_pos, int repeat = 500) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double time_start{MPI_Wtime()};
  double res_par{ParallelGlobalSearch(num_fun, x_left, x_right, y_left, y_right)};
  double time_par_end{MPI_Wtime()};
  auto par_time{time_par_end - time_start};
  cout << std::fixed << std::setprecision(10);
  if (rank == 0) {
    cout << " Test number: " << num_fun << endl;
    cout << "-#-#-#-#-#-# Parallel Time: " << par_time << " -#-#-#-#-#-#" << endl;
    cout << "-#-#-#-#-#-# Parallel Result: " << res_par << " -#-#-#-#-#-#" << endl;
    time_start = MPI_Wtime();
    double res_seq{SequentialGlobalSearch(num_fun, x_left, x_right, y_left, y_right, repeat)};
    const double time_end_s{MPI_Wtime()};
    cout << "-#-#-#-#-#-# Sequential Time :" << time_end_s - time_start << " -#-#-#-#-#-#" << endl;
    cout << "-#-#-#-#-#-# Sequential Result :" << res_seq << " -#-#-#-#-#-#" << endl;
    ASSERT_EQ(1, abs(ans_minimum - res_par) <= 0.15);
  }
}

TEST(Parallel_Operations_MPI, Test_first_fun) {
  const std::function<double(dpair)> func{fun_first};
  const std::function<dpair(dpair)> grad{grad_first};
  CreateTest(1, -200.0, 200.0, -200.0, 200.0, 0.0, {0.0, 0.0});
}

TEST(Parallel_Operations_MPI, Test_second_fun) {
  const std::function<double(dpair)> func {fun_second};
  const std::function<dpair(dpair)> grad { grad_second };
  CreateTest(2, -300.0, 300.0, -200.0, 600.0, -2.0, {0.0, 1.0});
}

TEST(Parallel_Operations_MPI, Test_third_fun) {
  const std::function<double(dpair)> func {fun_third};
  const std::function<dpair(dpair)> grad { grad_third };
  double anss = -8.0 / (2.71828 * 2.71828);
  CreateTest(3, -10.0, 10.0, -10.0, 10.0,
             anss, {-4.0, 2.0});
}

TEST(Parallel_Operations_MPI, Test_forth_fun) {
  const std::function<double(dpair)> func {fun_forth};
  const std::function<dpair(dpair)> grad { grad_forth };
  CreateTest(4, -200.0, 300.0, -200.0, 300.0, -0.125, {1.0, 0.5});
}

TEST(Parallel_Operations_MPI, Test_fifth_fun) {
  const std::function<double(dpair)> func {fun_fifth};
  const std::function<dpair(dpair)> grad { grad_fifth };
  CreateTest(5, -100.0, 100.0, -200.0, 300.0, 0.0, {3.0, 2.0});
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
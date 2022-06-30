//  Copyright by Oganyan Robert 2020

#ifndef MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#include <utility>
#include "../../../modules/task_3/oganyan_r_global_search/functions.h"

std::pair<std::function<double(dpair)>, std::function<dpair(dpair)>> Cur_fun(int num_fun);

//  Старт градиентного спуска
dpair GetStart(double x_left, double x_right,
                      double y_left, double y_right);

//  Проверка является ли точка экстремумом
bool IsExtremum(const dpair& last, const dpair& cur,
                const std::function<double(dpair)>& func, const double& eps);

//  Вышли ли за пределы
bool IsInside(dpair cur, double x_left, double x_right,
                     double y_left, double y_right);

//  Вычисление новой точки
dpair Calculate(dpair cur, dpair grad, double step);

//  Вычисление одномерной функции для нахождения шага методом золотого сечения
double MakeSimplefx(double x, dpair grad, dpair cur,
                const std::function<double(dpair)>& func);


//  Вычисление нового шага методом золотого сечения
double GoldenSelection(double a, double b, double eps, dpair gradient,
                            dpair cur, const std::function<double(dpair)>& func);

double SequentialGlobalSearch(int num_fun,
                double x_left, double x_right, double y_left, double y_right, int repeat = 2,
                double eps = 1e-5);

double ParallelGlobalSearch(int num_fun,
                double x_left, double x_right, double y_left, double y_right);



#endif  //  MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

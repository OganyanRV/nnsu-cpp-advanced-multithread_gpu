//  Copyright by Oganyan Robert 2020

#ifndef MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_
#define MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_

#include <cmath>
#include <functional>
#include <utility>

#define x first
#define y second

using dpair = std::pair<double, double>;

//  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

extern inline double fun_first(dpair coord) {
  return coord.x * coord.x + coord.y * coord.y;
}

extern inline dpair grad_first(dpair coord) {
  return {2 * coord.x, 2 * coord.y};
}

//  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

extern inline double fun_second(dpair coord) {
  return 3 * coord.x * coord.x + coord.x * coord.y + 2 * coord.y * coord.y - coord.x - 4 * coord.y;
}

extern inline dpair grad_second(dpair coord) {
  return {6 * coord.x + coord.y - 1, -3 + 4 * coord.y};
}

//  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

extern inline double fun_third(dpair coord) {
  return -exp(coord.x + coord.y) * (coord.x * coord.x - 2 * coord.y * coord.y);
}

extern inline dpair grad_third(dpair coord) {
  return {-2 * coord.x * exp(coord.x + coord.y) +
      (-coord.x * coord.x + 2 * coord.y * coord.y) *
          exp(coord.x + coord.y),
          4 * coord.y * exp(coord.x + coord.y) +
              (-coord.x * coord.x + 2 * coord.y * coord.y) *
                  exp(coord.x + coord.y)};
}

//  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

extern inline double fun_forth(dpair coord) {
  return coord.x * coord.x * coord.x * coord.x + coord.y * coord.y * coord.y * coord.y
      + coord.x * coord.y;
}

extern inline dpair grad_forth(dpair coord) {
  return { 4 *coord.x * coord.x * coord.x + coord.y, 4 * coord.y * coord.y * coord.y + coord.x  };
}

//  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

extern inline double fun_fifth(dpair coord) {
  return std::pow(coord.x * coord.x + coord.y - 11, 2) + std::pow(coord.x + coord.y * coord.y -7, 2);
}

extern inline dpair grad_fifth(dpair coord) {
  return { 4 * coord.x * (coord.x * coord.x + coord.y - 11) + 2 * coord.x + 2 * coord.y * coord.y -14,
           2 * coord.x * coord.x  + 4 * coord.y * (coord.x + coord.y * coord.y - 7) + 2 * coord.y - 22 };
}

#endif  //  MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_

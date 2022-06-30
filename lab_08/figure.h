#ifndef FIGURE_H
#define FIGURE_H

#include "structurs.h"

int add_point(const bool& is_cut, const point& p, content& data);
int check_convex(const polygon &cut);
bool check_cross(const polygon &cut);
int vector_product(const point &v1, const point &v2);

#endif // FIGURE_H

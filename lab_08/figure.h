#ifndef FIGURE_H
#define FIGURE_H

#include "structurs.h"

int add_point(const bool& is_cut, const point& p, content& data);
int check_cut(const cut_rect &cut);
int vector_product(const point &v1, const point &v2);

#endif // FIGURE_H

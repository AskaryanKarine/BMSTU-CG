#ifndef FILL_H
#define FILL_H

#include "structurs.h"
#include <map>

int fill_delay(const content &data, const int &delay, canvas_t &scene, gv_t &view, std::vector<double>& time);
int fill_fast(const content &data, canvas_t &scene, gv_t &view, std::vector<double>& time);
#endif // FILL_H

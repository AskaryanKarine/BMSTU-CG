#ifndef TABLE_H
#define TABLE_H

#include "structurs.h"
#include <QTableWidget>

int add_point(const point &p, const colors col, const bool &is_hole, QTableWidget *table, content &data);
void rewrite_table(const content &data, QTableWidget *table);
void del_row(const indexes &ind_data, content &data);
int change_point(const indexes &ind, const point &p, content &data);

#endif // TABLE_H

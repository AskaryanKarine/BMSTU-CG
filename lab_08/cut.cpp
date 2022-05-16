#include "cut.h"
#include "figure.h"
#include <cmath>

 bool cut_line(figure& res_line, const figure& src, const cut_rect& cut)
{
     bool flag = true;

     // нужны будут check_cross и vector cross
//    point_code_t T1, T2;
//    int S1 = find_point_code(T1, src.p1, cut);
//    int S2 = find_point_code(T2, src.p2, cut);

//    point R1 = { 0, 0 }, R2 = { 0, 0 };

//    if (S1 == 0 && S2 == 0) {
//        R1 = src.p1;
//        R2 = src.p2;
//    } else {
//        int p = log_mult(T1, T2);
//        if (p != 0)
//            flag = false;
//        else {
//            int i = 0;
//            point Q = { 0, 0 };
//            if (S1 == 0) {
//                R1 = src.p1;
//                Q = src.p2;
//                i = 1;
//            } else if (S2 == 0) {
//                R1 = src.p2;
//                Q = src.p1;
//                i = 1;
//            }
//            while (i < 2) {
//                i++;
//                if (i == 1) {
//                    Q = src.p1;
//                    R1 = find_r(src.p1, src.p2, Q, cut, flag);
//                    Q = src.p2;
//                } else
//                    R2 = find_r(src.p1, src.p2, Q, cut, flag);
//            }
//        }
//    }
//    if (flag) {
//        res_line.p1 = R1;
//        res_line.p2 = R2;
//        return true;
//    }

    return flag;
}

void cut(content& data)
{
     figure visible_line;
     for (figure line : data.lines)
         if (cut_line(visible_line, line, data.cut))
             data.visible_lines.push_back(visible_line);
 }

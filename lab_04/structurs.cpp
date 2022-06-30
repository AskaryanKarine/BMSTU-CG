#include "structurs.h"

void copy(struct content_t **a, struct content_t *b)
{
    for (size_t i = 0; i < b->figures.size(); i++)
        (*a)->figures.push_back(b->figures[i]);

    for (size_t i = 0; i < b->spectrums.size(); i++)
        (*a)->spectrums.push_back(b->spectrums[i]);

    (*a)->back_color = b->back_color;
}

#ifndef COLOR_PEAKER_H_INCLUDED
#define COLOR_PEAKER_H_INCLUDED

#include "component.h"

namespace be
{
    class color_peaker : component
    {
        vec2d color_vertex[3];// index 0 = red , index 1 = green and index 2 = blue
    };
}

#endif // COLOR_PEAKER_H_INCLUDED

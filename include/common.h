#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>



#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <cmath>
#include <map>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <functional>
#include <initializer_list>


#include "Error.h"
#include "random_vector.h"
#include "string_handler.h"


namespace be
{
    union vec2d
    {
        struct { int x,y; };
        struct { int w,h; };
    };

    union vec2df
    {
        struct { double x,y; };
        struct { double w,h; };
    };

    struct vec3d
    {
        int x,y,z;
    };

    union Line
    {
        struct {vec2d head, tail; };
        struct { int data[4];};
    };


    inline bool operator==(const vec2d& A, const vec2d& B)
    {
        return (A.x == B.x && A.y == B.y);
    }

    inline bool operator!=(const vec2d& A, const vec2d& B)
    {
        return !(A.x == B.x && A.y == B.y);
    }

    inline bool operator==(const Line& A, const Line& B)
    {
        return (A.head == B.head && A.tail == B.tail);
    }

    inline bool operator!=(const Line& A, const Line& B)
    {
        return !(A.head == B.head && A.tail == B.tail);
    }

    inline bool operator==(const SDL_Rect& A, const SDL_Rect& B)
    {
        return (A.x == B.x && A.y == B.y && A.w == B.w && A.h == B.h);
    }

    inline bool operator!=(const SDL_Rect& A, const SDL_Rect& B)
    {
        return !(A == B);
    }

    inline bool operator==(const SDL_Color& a, const SDL_Color& b)
    {
        return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a)? true : false;
    }

    inline bool operator!=(const SDL_Color& a, const SDL_Color& b)
    {
        return !(a == b);
    }



    inline SDL_Color int_to_color(uint32_t num)
    {
        SDL_Color cl;
        cl.a = (uint8_t)(num >> 24);
        cl.b = (uint8_t)(num >> 16) & 255;
        cl.g = (uint8_t)(num >> 8) & 255;
        cl.r = (uint8_t)(num & 255);
        return cl;
    }

}

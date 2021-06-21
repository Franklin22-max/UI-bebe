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
#include <cstdint>
#include <vector>
#include <algorithm>
#include <functional>
#include <initializer_list>


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


    inline std::string to_string(double num)
    {
        std::stringstream ss;
        ss << num;
        return ss.str();
    }

    inline double to_number(std::string str)
    {
        double num;
        std::stringstream ss;
        ss << str;
        ss >> num;
        return num;
    }

    inline std::string substr(const std::string& src, int s, int e)
    {
        std::string h("");
        for(; s < e; s++)
            h += src[s];
        return h;
    }

    inline std::string insert(const std::string& src, int i, std::string value)
    {
        return (substr(src,0,i) + value + substr(src,i,src.length()));
    }

    inline std::string remove(const std::string& src, int i, uint32_t length = 1)
    {
        return (substr(src,0,i) + substr(src,i + length ,src.length()));
    }

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

    inline bool is_point_inside_rect(const vec2d& p, const SDL_Rect& rect)
    {
        SDL_Point _p = {p.x, p.y};
        return SDL_PointInRect(&_p,&rect);
    }

    inline bool is_line_intersecting(const Line& A, const Line& B)
    {
        float h = (B.tail.x - B.head.x) * (A.head.y - A.tail.y) - (A.head.x - A.tail.x) * (B.tail.y - B.head.y);
        float t1 = ((B.head.y - B.tail.y) * (A.head.x - B.head.x) + (B.tail.x - B.head.x) * (A.head.y - B.head.y)) / h;
        float t2 = ((A.head.y - A.tail.y) * (A.head.x - B.head.x) + (A.tail.x - A.head.x) * (A.head.y - B.head.y)) / h;

        return (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f);
    }


    inline bool isPointInsidePolygon(vec2d p,std::vector<vec2d> vertices)
    {
        for (uint16_t i = 0; i < vertices.size(); i++)
        {
            uint16_t j = (i + 1) % vertices.size();
            vec2d m = {-(vertices[j].y - vertices[i].y) , vertices[j].x - vertices[i].x};
            //double distance = sqrtf(m.y * m.y + m.x * m.x);
            //m = {m.y/distance, m.x/distance};

            float min = INFINITY , max = -INFINITY;
            for (uint16_t i = 0; i < vertices.size(); i++)
            {
                float dot = (vertices[i].x * m.x + vertices[i].y * m.y);
                min = std::min(min, dot);
                max = std::max(max, dot);
            }

            float p_dot = (p.x * m.x + p.y * m.y);

            if(!(max >= p_dot && p_dot >= min))
            return false;
        }
        return true;
    }

    /** \brief
     * \param A first line
     * \param B second line
     * \param norm_A directional vector of line one must be  b/w -1.0 to 1.0
     * \param norm_B directional vector of line one must be b/w -1.0 to 1.0
     * \note function does not check if line is intersecting to check it use
     *y
     * \sa  is_line_intersecting()
     */

    inline vec2d get_line_intersection_point(const Line& A, const Line& B, const vec2d norm_A, const vec2d norm_B)
    {

    }



    inline SDL_Rect get_intersection_rect(const SDL_Rect& A, const SDL_Rect& B)
    {
        SDL_Rect R;

        if(A.x <= B.x && A.x+A.w > B.x)
        {
            R.x = B.x;
            R.w = (A.x+A.w > B.x+B.w)? (B.x+B.w) - R.x  :   (A.x+A.w) - R.x;
        }
        else if(A.x >= B.x && A.x <= B.x+B.w)
        {
            R.x = A.x;
            R.w = (A.x+A.w > B.x+B.w)? (B.x+B.w) - R.x  :   (A.x+A.w) - R.x;
        }
        else return {0,0,0,0};

        if(A.y <= B.y && A.y+A.h > B.y)
        {
            R.y = B.y;
            R.h = (A.y+A.h > B.y+B.h)? (B.y+B.h) - R.y  :   (A.y+A.h) - R.y;
            return R;
        }
        else if(A.y >= B.y && A.y <= B.y+B.h)
        {
            R.y = A.y;
            R.h = (A.y+A.h > B.y+B.h)? (B.y+B.h) - R.y  :   (A.y+A.h) - R.y;
            return R;
        }
        else return {0,0,0,0};
    }


    inline Line get_intersection_line(Line line, const SDL_Rect& rect)
    {
        Line R = {0,0,0,0};

        // make sure that the head of the line is at the left side
        if(line.head.x > line.tail.x)
            std::swap(line.head, line.tail);

        // if line is horizontal
        if(line.head.y == line.tail.y && (line.head.y > rect.y && line.head.y < rect.y + rect.h))
        {
            if(line.head.x <= rect.x && line.tail.x > rect.x)
            {
                R.head.y = R.tail.y = line.head.y;
                R.head.x = rect.x;
                R.tail.x = (line.tail.x > rect.x+rect.w)? rect.x+rect.w : line.tail.x;
                return R;
            }
            else if(line.head.x > rect.x && line.head.x < rect.x+rect.w)
            {
                R.head.y = R.tail.y = line.head.y;
                R.head.x = line.head.x;
                R.tail.x = (line.tail.x > rect.x+rect.w)? rect.x+rect.w : line.tail.x;
                return R;
            }
            else return R;
        }
        // if line is vertical
        else if(line.head.x == line.tail.x && (line.head.x > rect.x && line.head.x < rect.x + rect.w))
        {
            int min_y = std::min(line.head.y,line.tail.y);
            int max_y = std::max(line.head.y,line.tail.y);

            if(min_y <= rect.y && max_y > rect.y)
            {
                R.head.x = R.tail.x = line.head.x;
                R.head.y = rect.y;
                R.tail.y = (max_y > rect.y+rect.h)? rect.y+rect.h : max_y;
                return R;
            }
            else if(min_y > rect.y && min_y < rect.y+rect.h)
            {
                R.head.x = R.tail.x = line.head.x;
                R.head.y = min_y;
                R.tail.y = (max_y > rect.y+rect.h)? rect.y+rect.h : max_y;
                return R;
            }
            else {return R;}
        }
        // if line is diagonal
        else
        {
            bool head_in_rect = is_point_inside_rect(line.head,rect);
            bool tail_in_rect = is_point_inside_rect(line.tail,rect);

            // if the entire line is within rectangle
            if( head_in_rect && tail_in_rect)
                return line;
            else
            {
                bool head_set = false;
                if(head_in_rect)
                {
                    R.head = line.head;
                    head_set = true;
                }
                else if(tail_in_rect)
                {
                    R.head = line.tail;
                    head_set = true;
                }

                double slope = ((line.tail.y - line.head.y)*1.0) / (line.tail.x - line.head.x);
                // left side
                if(is_line_intersecting(line, {rect.x, rect.y, rect.x, rect.y+rect.h}))
                {
                    if(head_set)
                    {
                        R.tail = { rect.x, -slope * (line.tail.x - rect.x) + line.tail.y};
                        return R;
                    }
                    else
                    {
                        R.head = { rect.x, -slope * (line.tail.x - rect.x) + line.tail.y};
                        head_set = true;
                    }

                }
                // right
                if(is_line_intersecting(line, {rect.x+rect.w, rect.y, rect.x+rect.w, rect.y+rect.h}))
                {
                    if(head_set)
                    {
                        R.tail = { (rect.w + rect.x), slope * ((rect.w + rect.x) - line.head.x) + line.head.y};
                        return R;
                    }
                    else
                    {
                        R.head = { (rect.w + rect.x), slope * ((rect.w + rect.x) - line.head.x) + line.head.y};
                        head_set = true;
                    }
                }
                // up
                if(is_line_intersecting(line, {rect.x, rect.y, rect.x+rect.w, rect.y}))
                {
                    if(head_set)
                    {
                        R.tail = { line.head.x + (rect.y - line.head.y)/slope,   rect.y};
                        return R;
                    }
                    else
                    {
                        R.head = { line.head.x + (rect.y - line.head.y)/slope,   rect.y};
                        head_set = true;
                    }
                }
                // bottom
                if(is_line_intersecting(line, {rect.x, rect.y+rect.h, rect.x+rect.w, rect.y+rect.h}))
                {
                    if(head_set)
                    {
                        R.tail = { line.head.x + ((rect.y + rect.h) - line.head.y)/slope,   rect.y + rect.h};
                        return R;
                    }
                    else
                    {
                        R.head = { line.head.x + ((rect.y + rect.h) - line.head.y)/slope,   rect.y + rect.h};
                        head_set = true;
                    }
                }

                R = {0,0,0,0};
                return R;
            }
        }
    }
}

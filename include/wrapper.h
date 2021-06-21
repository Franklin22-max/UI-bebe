#pragma once

#include "Administrator.h"
#include "component.h"
#include <vector>


namespace be
{
    class wrapper
    {
    public:
        enum class GROWTH_DIRECTION { HORIZONTALLY, VERTICALLY};
    private:
        be::view* view;
        uint8_t pad;
        uint8_t grid_no;
        vec2d plane_size; // the size of the plane where components are place
        SDL_Rect display;
        GROWTH_DIRECTION dir;
        std::vector<component*> components;
    public:
        /** \brief
         * \param pan spacing b/w two components
         * \param dir this is the direction the wrapper is allowed to grow
         * \param grid_no : this is the number of component required to fill non growing direction
         */

        wrapper(be::view* view, int x, int y,int w,int h, uint8_t pad, uint8_t grid_no = 1, GROWTH_DIRECTION dir = GROWTH_DIRECTION::VERTICALLY)
        :display({x,y,w,h}), dir(dir), grid_no(grid_no), pad(pad), view(view)
        {
            plane_size = {0,0};
        }

        /** \brief
         * \return a constant pointer to inputed component or null pointer on failure
         * \note use make component to add component, the component must only exist in wrapper to avoid conflict
         */

        const component* add_component(component* comp)
        {
            if(comp)
            {
                comp->Enable();
                components.push_back(comp);
                auto ptr = comp->get_view();
                ptr = view;
                return comp;
            }
            else return nullptr;
        }

    public:
        void arrange()
        {
            if(dir == GROWTH_DIRECTION::VERTICALLY)
            {
                plane_size = {0,0};
                int max_grid_width[grid_no];
                for(int i =0; i < grid_no; i++)
                    max_grid_width[i] = 0;

                // get the largest width for each grid
                for(int i = 0; i < components.size(); i++)
                    max_grid_width[i % grid_no] = std::max(max_grid_width[i % grid_no], components[i]->get("w"));
                // set each grid start position
                for(int i = 0; i < grid_no; i++)
                    if(i > 0)   max_grid_width[i] += (pad + max_grid_width[i - 1]); else max_grid_width[i] += pad;

                plane_size.w = max_grid_width[grid_no - 1];

                int max_heigth = 0;
                for(int i = 0; i < components.size(); i++)
                {
                    if(i % grid_no == 0)
                    {
                        plane_size.h += max_heigth;
                        max_heigth = 0;

                        components[i]->set("x", display.x);
                        components[i]->set("y", display.y + plane_size.h);
                        max_heigth = components[i]->get("h");
                    }
                    else
                    {
                        components[i]->set("x", max_grid_width[(i % grid_no) - 1] + display.x);
                        components[i]->set("y", display.y + plane_size.h);
                        max_heigth = std::max(components[i]->get("h"), max_heigth);

                        if(i == components.size() - 1)
                            plane_size.h += max_heigth;

                    }
                    //std::cout<<components[i]->get("x")<<" : "<<components[i]->get("y")<<"\n";
                }
            }
            else if(dir == GROWTH_DIRECTION::HORIZONTALLY)
            {
                plane_size = {0,0};
                int max_grid_h[grid_no];
                for(int i =0; i < grid_no; i++)
                    max_grid_h[i] = 0;

                // get the largest height for each grid
                for(int i = 0; i < components.size(); i++)
                    max_grid_h[i % grid_no] = std::max(max_grid_h[i % grid_no], components[i]->get("h"));
                // set each grid start position
                for(int i = 0; i < grid_no; i++)
                    if(i > 0)   max_grid_h[i] += (pad + max_grid_h[i - 1]); else max_grid_h[i] += pad;

                plane_size.h = max_grid_h[grid_no - 1];

                int max_width = 0;
                for(int i = 0; i < components.size(); i++)
                {
                    if(i % grid_no == 0)
                    {
                        plane_size.w += max_width;
                        max_width = 0;

                        components[i]->set("y", display.y);
                        components[i]->set("x", display.x + plane_size.w);
                        max_width = components[i]->get("w");
                    }
                    else
                    {
                        components[i]->set("y", max_grid_h[(i % grid_no) - 1] + display.y);
                        components[i]->set("x", display.x + plane_size.w);
                        max_width = std::max(components[i]->get("w"), max_width);

                        if(i == components.size() - 1)
                            plane_size.w += max_width;

                    }
                    //std::cout<<components[i]->get("x")<<" : "<<components[i]->get("y")<<"\n";
                }
            }

        }

        void Render()
        {
            for(auto &i: components)
            {
                // use wrapper display as clip boarder
                i->Render(&display);
            }

        }

    };
}

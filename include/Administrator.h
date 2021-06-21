#ifndef ADMINISTRATOR_H_INCLUDED
#define ADMINISTRATOR_H_INCLUDED

#include "view.h"
#include "Event.h"
#include <SDL_syswm.h>
#include <algorithm>

namespace be
{

    enum VIEW_PLACEMENT {ALWAYS_ON_TOP, ANY_POSITION };

    class Administrator
    {
        Administrator() { event = Event::get_instance(); };
        Administrator(const Administrator&) = delete;

        // holds info about the z index of are view object
        struct VZI
        {
            VIEW_PLACEMENT p;
            view* view;
            uint16_t index;
        };

        SDL_Window* window;
        Event* event;
        VZI* active_view = NULL;
        std::vector<VZI> views;
        std::map<std::string,bool> text_components;


        VZI* find(be::view* view)
        {
            if(views.size() > 0)
            {
                for(auto &i: views)
                {
                    if(i.view == view)
                    {
                        return &i;
                        break;
                    }
                }
            }
            return nullptr;
        }

        bool is_view_infocus(view* v)
        {
            vec2d mouse = event->get_mouse_pos();
            return (mouse.x >= v->display.x && mouse.y >= v->display.y && mouse.x <= v->display.x+v->display.w && mouse.y <= v->display.y+v->display.h);
        }


        void resolve_index(int index)
        {
            for(int i = views.size()-1; i >= 0; i--)
            {
                if(i > index)
                    views[i].index -= 1;
                else if(i == index)
                {
                    views[i].index = views.size()-1;
                    std::sort(views.begin(), views.end(),[](VZI& f,VZI& s){ return (f.index < s.index); });
                    active_view = &views[views.size()-1];
                    break;
                }
            }
        }

        static Administrator* instance;
    public:
        static Administrator* get_instance(){ return instance = (instance != nullptr)? instance : new Administrator;};

        void Destroy()
        {
            for(auto &i : views)
                delete i.view;
            views.erase(views.begin(), views.end());
        }


        void SET_WINDOW(SDL_Window* window)
        {
            this->window = window;
        }


        HWND Get_Window_handler()
        {
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version);
            SDL_GetWindowWMInfo(window,&info);
            return info.info.win.window;
        }


        view* create_view(VIEW_PLACEMENT placement,SDL_Renderer* renderer,int x,int y,int w=350,int h=600,SDL_Color bg = {0,0,0,0},view::MODE adjust = view::MODE::DYNAMIC, vec2d texture_size = {350,150})
        {
            view* __view = new view(renderer,x,y,w,h,bg,adjust,texture_size);

            if(__view)
            {
                VZI vi;
                vi.p = placement;
                vi.view = __view;

                if(views.size() > 0 && vi.p != ALWAYS_ON_TOP)
                {
                    for(auto &i: views)
                    {
                        if(i.p == ALWAYS_ON_TOP)
                            i.index += 1;
                        else
                        {
                            vi.index = i.index + 1;
                            break;
                        }
                    }
                }
                else vi.index = views.size();

                views.push_back(vi);
                // sort views in other of focus
                std::sort(views.begin(), views.end(),[](VZI& f,VZI& s){ return (f.index < s.index); });
                return __view;
            }
            return nullptr;
        }


        void EVENT_POOL()
        {
            (*event)();

            if(event->mouse_left.get_state() == Event::key_state::click || event->mouse_right.get_state() == Event::key_state::click)
            {
                for(int i = views.size()-1; i >= 0; i--)
                {
                    if(is_view_infocus(views[i].view) && i != views.size()-1 && views[i].p != ALWAYS_ON_TOP)
                    {
                        resolve_index(i);
                        break;
                    }
                    else if(is_view_infocus(views[i].view) && (i == views.size()-1 || views[i].p == ALWAYS_ON_TOP))
                        break;
                    else if(views[i].p == ALWAYS_ON_TOP && (i != views.size()-1))
                    {
                        resolve_index(i);
                        break;
                    }
                }
            }
        }


        void RENDER_PRESENT()
        {
            for(auto v : views)
                v.view->RenderPresent();
        }


        /** \brief
         *  All functions below are encapsulations of event calls with a higher level control
         */

        Event::key_state get_special_key_state(std::string key)
        {
            return event->get_key_state(key);
        }

        vec2d get_mouse_pos(view* view)
        {
            VZI* vi = find(view);
            if(vi != nullptr)
            {
                if(vi->index  == views.size()-1)
                    return event->get_mouse_pos();
                else
                    return {-1,-1};
            }
        }

        Event::key_state get_key_state(view* view,std::string key)
        {
            VZI* vi = find(view);
            if(vi->index  == views.size()-1)
                return event->get_key_state(key);
            else
                return Event::key_state::none;
        }

        bool get_modstate(view* view, std::string key_mode_name)
        {
            VZI* vi = find(view);
            if(vi->index  == views.size()-1)
                return event->get_modstate(key_mode_name);
            return false;
        }

        void start_text_input(view* view,std::string id)
        {
            if(SDL_IsTextInputActive() == SDL_FALSE)
            {
                VZI* vi = find(view);
                if(vi->index  == views.size()-1)
                {
                    text_components[id] = true;
                    event->start_text_input();
                }
            }
        }

        void stop_text_input(view* view, std::string id)
        {
            bool skip = false;
            text_components[id] = false;
            for(auto &i : text_components)
            {
                if(i.second == true)// ignore stop text input if another component is using it
                {
                    skip = true;    break;
                }
            }

            if(!skip)
            {
                VZI* vi = find(view);
                if(vi->index  == views.size()-1)
                    event->stop_text_input();
            }
        }

        char* get_inputed_text(view* view)
        {
            VZI* vi = find(view);
            if(vi->index  == views.size()-1)
                return event->get_text_char();
            else
                return nullptr;
        }

        bool has_inputed_text(view* view)
        {
            VZI* vi = find(view);
            if(vi->index  == views.size()-1)
                return event->has_inputed_text();
            return false;
        }


    };
}

#endif // ADMINISTRATOR_H_INCLUDED

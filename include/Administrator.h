#ifndef ADMINISTRATOR_H_INCLUDED
#define ADMINISTRATOR_H_INCLUDED

#include "view.h"
#include "Event.h"
#include "Error.h"
#include <SDL_syswm.h>
#include <algorithm>

namespace be
{
    class Administrator
    {
    public:
        enum VIEW_PLACEMENT {ALWAYS_ON_TOP, ANY_POSITION, ALWAYS_ON_TOP_AND_GRAB_EVENT };

        Administrator() { event = Event::get_instance(); };
        Administrator(const Administrator&) = delete;

        SDL_Window* window;
        Event* event;
        // the view with the event
        be::view* active_view = NULL;
        // view which alway appears on top and grab event
        be::random_vector<be::view*> alpha_views;
        // view which alway appears on top but does not grab event
        be::random_vector<be::view*> beta_views;
        // view which occupies any position
        be::random_vector<be::view*> omega_views;
        // components that reqTESTres text inputs
        std::map<std::string,bool> text_components;


        bool isViewInFocus(view* v)
        {
            vec2d mouse = event->get_mouse_pos();
            return (mouse.x >= v->display.x && mouse.y >= v->display.y && mouse.x <= v->display.x+v->display.w && mouse.y <= v->display.y+v->display.h);
        }



        void rearrange_views(be::random_vector<be::view*>& store ,int i)
        {
            be::view* hold =  store[i];
            // remove the element from it position and place it at the back
            for(; i != store.size(); i++)
            {
                if(i + 1 < store.size())
                {
                    store[i] = store[i + 1];// relocate elements to fill the position of our current view
                }
                else if(store[i] != hold   &&  i == store.size() - 1)
                    store[i] = hold; // now place it at the very end
            }
        }

        /** \brief
         *
         * \param  vector to search
         * \param  view ptr to be found
         * \return returns an iterator to the view and end() of container if not found
         *
         */

        be::random_vector<be::view*>::iterator find(be::random_vector<be::view*>& store, be::view* view)
        {
            for(auto i = store.begin(); i != store.end(); ++i)
            {
                if(*i  == view) return i;
            }

            return store.end();
        }




        static Administrator* instance;
    public:
        static Administrator* get_instance(){ return instance = (instance != nullptr)? instance : new Administrator;};


        const be::view* get_active_view()
        {
            return active_view;
        }

        void INIT(SDL_Window* window)
        {
            this->window = window;
        }


        void Destroy()
        {
            for(auto &i : alpha_views)
                delete i;
            alpha_views.erase(alpha_views.begin(), alpha_views.end());

            for(auto &i : beta_views)
                delete i;
            beta_views.erase(beta_views.begin(), beta_views.end());

            for(auto &i : omega_views)
                delete i;
            omega_views.erase(omega_views.begin(), omega_views.end());
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
                if(placement == ALWAYS_ON_TOP_AND_GRAB_EVENT)
                {
                    alpha_views.push_back(__view);
                    __view = alpha_views[alpha_views.size() - 1];
                    active_view = __view;
                }
                else if(placement == ALWAYS_ON_TOP)
                {
                    beta_views.push_back(__view);
                    __view = beta_views[beta_views.size() - 1];

                    if(alpha_views.size() == 0)
                        active_view = __view;
                }
                else
                {
                    omega_views.push_back(__view);
                    __view = omega_views[omega_views.size() - 1];

                    if(alpha_views.size() == 0)
                        active_view = __view;
                }

                return __view;
            }
            return nullptr;
        }


        void EVENT_POOL()
        {   // run event via function operator
            (*event)();
            // set active view when mouse left is clicked
            if(event->mouse_left.get_state() == Event::key_state::click || event->mouse_right.get_state() == Event::key_state::click)
            {
                if(alpha_views.size() > 0)
                {
                    active_view = alpha_views[alpha_views.size() - 1];
                }
                else
                {
                    bool skip_omega = false;

                    for(int i = beta_views.size() - 1; i >= 0; i--)
                    {
                        if(isViewInFocus(beta_views[i]))
                        {
                            if(i == beta_views.size() - 1)// if its already the last one
                                active_view = beta_views[i];
                            else
                            {   // place it at the end
                                rearrange_views(beta_views, i );
                                active_view = beta_views[beta_views.size() - 1];
                            }
                            skip_omega = true;
                            break;
                        }
                    }

                    if(!skip_omega)
                    {
                        for(int i = omega_views.size() - 1; i >= 0; i--)
                        {
                            if(isViewInFocus(omega_views[i]))
                            {
                                if(i == omega_views.size() - 1)// if its already the last one
                                    active_view = omega_views[i];
                                else
                                {   // place it at the end
                                    rearrange_views(omega_views,  i);
                                    active_view = omega_views[omega_views.size() - 1];
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }


        void RENDER_PRESENT()
        {
            for(auto &v : omega_views)
                v->RenderPresent();

            for(auto &v : beta_views)
                v->RenderPresent();

            for(auto &v : alpha_views)
                v->RenderPresent();
        }


        void REMOVE_VIEW(be::view* view)
        {
            auto it = find(alpha_views, view);
            if(it != alpha_views.end())
            {
                if(*it == active_view)
                {
                    if(alpha_views.size() > 1)
                        active_view = *(--it);
                    else if(!beta_views.empty()  || !omega_views.empty())
                        active_view = (!beta_views.empty())? *(--beta_views.end()) : *(--omega_views.end());
                    else
                        active_view = NULL;
                }
                auto __v = *it;
                alpha_views.erase(it);
                delete __v;
            }
            else
            {
                it = find(beta_views, view);
                if(it != beta_views.end())
                {
                    if(*it == active_view)
                    {
                        if(beta_views.size() > 1)
                            active_view = *(--it);
                        else if(!omega_views.empty())
                            active_view = *(--omega_views.end());
                        else
                            active_view = NULL;
                    }
                    auto __v = *it;
                    beta_views.erase(it);
                    delete __v;
                }
                else
                {
                    it = find(omega_views, view);
                    if(it != omega_views.end())
                    {
                        if(*it == active_view)
                        {
                            if(omega_views.size() > 1)
                                active_view = *(--it);
                            else
                                active_view = NULL;
                        }
                        auto __v = *it;
                        omega_views.erase(it);
                        delete __v;
                    }
                }
            }
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
            if(view == active_view)
            {
                if(view == active_view)
                    return event->get_mouse_pos();
                else
                    return {-1,-1};
            }
        }

        Event::key_state get_key_state(view* view,std::string key)
        {
            if(view == active_view)
                return event->get_key_state(key);
            else
                return Event::key_state::none;
        }

        bool get_modstate(view* view, std::string key_mode_name)
        {
            if(view == active_view)
                return event->get_modstate(key_mode_name);
            return false;
        }

        void start_text_input(view* view,std::string id)
        {
            if(SDL_IsTextInputActive() == SDL_FALSE)
            {
                if(view == active_view)
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
                if(view == active_view)
                    event->stop_text_input();
            }
        }

        char* get_inputed_text(view* view)
        {
            if(view == active_view)
                return event->get_text_char();
            else
                return nullptr;
        }

        bool has_inputed_text(view* view)
        {
            if(view == active_view)
                return event->has_inputed_text();
            return false;
        }


    };
}

#endif // ADMINISTRATOR_H_INCLUDED

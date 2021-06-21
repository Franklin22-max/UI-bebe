#ifndef component_H_INCLUDED
#define component_H_INCLUDED

#include "Administrator.h"

namespace be
{
    class component
    {
    protected:
        vec2d pos;
        vec2d size;
        view* view;
        std::string id;
        bool is_active;
        bool on_hover;
    public:
        component(be::view* view,std::string id,int x,int y, int w, int h)
        :pos({x,y}),size({w,h}), id(id), is_active(false), on_hover(false)
        {
            this->view = view;
        }

        be::view* get_view()
        {
            return view;
        }

        virtual ~component() = default;

        virtual void Disable() = 0;
        virtual void Enable() = 0;
        virtual void Logic(vec2d _mouse) = 0;
        virtual void Update() = 0;
        virtual void Render(SDL_Rect* clip_boarder = NULL) = 0;

        virtual int get(std::string key) = 0;
        virtual void set(std::string key, int value) = 0;
    };



    /** \brief creates object of component sub-classes with ease, thanks to c++ compiler
     *
     * \param   arguments for sub component class
     * \return pointer to created component
     * \note components are created on the heap so manually deallocate
     */

    template<class T, class... Args>
    component* make_component(Args... args)
    {
        T* comp = new T(args...);
        return comp;
    }
}

#endif // componentx_H_INCLUDED

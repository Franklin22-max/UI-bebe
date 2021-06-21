#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "component.h"
#include "Renderer.h"

namespace be
{
    class button : public component
    {
        SDL_Color bg;
        SDL_Color fg;
        text_renderer* TR;
        std::string label;
    public:
        buttton(be::view* view,int x,int y,int w, int h,std::string label,SDL_Color bg,SDL_Color fg)
        :component(view,x,y,w,h), fg(fg), bg(bg), label(label)
        {
            TR = new text_renderer(view,label);
        }

        ~button()
        {
            delete TR;
        }


        void Logic(vec2d _mouse) override
        {
            vec2d m = _mouse;
            on_hover = (m.x >= pos.x && m.y >= pos.y && m.x <= pos.x+size.w && m.y <= pos.y+size.h)? true : false;
        }
    };
}


#endif // BUTTON_H_INCLUDED

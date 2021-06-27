#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "trigger.h"

namespace be
{
    class button : trigger
    {
        SDL_Rect frame;
        bool draw_frame;
    public:
        button(be::view* view, std::string uuid,std::string text, int x = 0, int y = 0, std::function<void( STATE&, unsigned int )> callback = NULL)
        :trigger(view,uuid,text,x,y,0,0,callback)
        {
            frame = {x,y,0,0};
        }

        void Update() override
        {
            if(is_active)
            {
                if(in_focus)
                {
                    frame.h = TR->get("h");
                    frame.w = TR->get("w") + TR->get("font_size")*2;
                    TR->set("x", x+TR->get("font_size"));
                    TR->set("y", y);
                    draw_frame = true;
                }
                else
                {
                    frame.h = frame.w = 0;
                    draw_frame = false;
                }
            }
        }

        void Render(SDL_Rect* clip_border = bull)
        {
            if(is_active)
            {
                TR->Render(clip_border);

                if(draw_frame)
                {
                    SDL_Color* cl = theme::get_instance()->get_color(4);
                    if(cl)
                        view->RenderDrawRect(clip_border,&frame,*cl,SDL_BLENDMODE_BLEND);
                    else
                        view->RenderDrawRect(clip_border,&frame,{0,0,0,255},SDL_BLENDMODE_BLEND);
                }
            }
        }
    };
}


#endif // BUTTON_H_INCLUDED

#ifndef TRIGGER_H_INCLUDED
#define TRIGGER_H_INCLUDED

#include "view.h"
#include "component.h"

namespace be
{

    class trigger : component
    {
    public:
        enum class STYLE { button, link , icon , dialog_icon };
        enum class STATE { click, held, release, none};

        /** \brief
         *  STATE -> state of the button { click , held or release }
         *  uint32_t -> length of time the button has been held
         */
        std::function<void( STATE&, unsigned int )> callback = NULL;

        image_renderer* IR;
        text_renderer* TR;
        unsigned int held_start = 0;
        STATE state = STATE::none;
        STYLE style;

    public:

        trigger(be::view* view, std::string uuid, int x = 0, int y = 0, int w = 0, int h = 0, std::string text = "",const theme::IMG_NODE* img = nullptr ,std::function<void( STATE&, unsigned int )> callback = NULL, STYLE style = STYLE::button)
        :component(view,uuid,x,y,w,h), callback(callback), style(style)
        {
            TR = new text_renderer(view,text);
            IR = new image_renderer(view, img);
        }

        ~trigger()
        {
            delete TR;
            delete IR;
        }

        void Disable()
        {
            is_active = false;
        }

        void Enable()
        {
            is_active = true;
        }

        void Logic(vec2d mouse)  override
        {
            if(is_active)
            {
                mouse = view->resolve_point(mouse);
                on_hover = (mouse.x >= pos.x && mouse.x <= pos.x + size.w && mouse.y >= pos.y && mouse.y <= pos.y + size.h);

                if(on_hover && Administrator::get_instance()->get_key_state(view,"mouse left") == Event::key_state::click)
                {
                    state = STATE::click;
                    held_start = SDL_GetTicks();
                }
                else if(on_hover && Administrator::get_instance()->get_key_state(view,"mouse left") == Event::key_state::held)
                    state = STATE::held;
                else if(on_hover && Administrator::get_instance()->get_key_state(view,"mouse left") == Event::key_state::released)
                    state = STATE::release;
                else
                    state = STATE::none;
            }
        }

        void Update() override
        {
            if(callback != NULL && (state == STATE::click || state == STATE::held || state == STATE::release))
                callback(state, (state == STATE::held)? SDL_GetTicks() - held_start  :   0 );
        }

        void Render(SDL_Rect* clip_boarder) override
        {

        }

        const STATE& get_state()
        {
            return state;
        }



        int get(std::string key) override
        {
            if(key == "x")
                return pos.x;
            else if(key == "y")
                return pos.y;
            else if(key == "w")
                return size.w;
            else if(key == "h")
                return size.h;
            else if(key == "font size" || key == "font_size")
                return TR->get("font_size");
            else return -1;

        }

        void set(std::string key, int value) override
        {
            if(key == "x")
                pos.x = value;
            else if(key == "y")
                pos.y = value;
            else if(key == "w")
                size.w = value;
            else if(key == "h")
                size.h = value;
            else if(key == "font size" || key == "font_size")
                TR->set("font_size",value);
        }



        void Design()
        {
            if(style == STYLE::button)
            {

            }
        }



    };

}

#endif // TRIGGER_H_INCLUDED

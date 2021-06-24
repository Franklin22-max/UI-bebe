#ifndef UI_INCLUDE_H
#define UI_INCLUDE_H


#include "common.h"
#include "Event.h"
#include "Renderer.h"
#include "f_time.h"
#include "Administrator.h"
#include "text_input.h"
#include "File.h"
#include "wrapper.h"
#include "SurfaceHandler.h"
#include "Theme.h"
#include "trigger.h"

#define RENDERER_SYNC           SDL_RENDERER_PRESENTVSYNC
#define RENDERER_ACCELERATED    SDL_RENDERER_ACCELERATED
#define RENDERER_SOFTWARE       SDL_RENDERER_SOFTWARE
#define RENDERER_TARGETTEXTURE  SDL_RENDERER_TARGETTEXTURE


namespace be
{
    static int num = 0;
    class UI
    {
        SDL_Window* window;
        SDL_Renderer* renderer;
        Administrator* administrator;
        Event* event;
        bool isRunning = false;
        int window_h,window_w;
        time timer;

        view* view1;
        view* view2;
        view* view3;

        wrapper* wrp;
        image_renderer* ir;
        image_renderer* ir2;
        text_renderer* tr;
        text_input* text_in;

        SDL_Surface* surf;
        SDL_Texture* texture;

        vec2d former_mouse;

    public:
        std::string INIT(const char* title,int window_w, int window_h, Uint32 renderer_flag)
        {
            this->window_h = window_h;
            this->window_w = window_w;

            //  initialize SDL2
            if(SDL_Init(SDL_INIT_EVERYTHING) > 0) return std::string("Error initializing SDL: ")+SDL_GetError();
            // initialize SDL_TTF
            if(TTF_Init()) return std::string("Error initializing TTF: ")+SDL_GetError();
            // initialize SDL_IMAGE
            if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP))  return std::string("Error initializing IMG: ");
            // initialize mixer
            if(!Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS)) return std::string("Error initializing MIXER: ");

            // create window
            window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,window_w,window_h,SDL_WINDOW_RESIZABLE);
            if(!window) return std::string("Error creating window: ")+SDL_GetError();

            // create renderer
            renderer = SDL_CreateRenderer(window,-1,renderer_flag);
            if(!renderer)  return std::string("Error creating renderer: ")+SDL_GetError();

            std::string err;
            theme::get_instance()->INIT(renderer);
            theme::get_instance()->load_font("consolaz", R"(C:\Users\USER\Documents\CODE BLOCK PROJECT\UI-bebe\fonts\consolaz.ttf)",14,&err);
            theme::get_instance()->set_default_font("consolaz",14);

            // create event object
            event = Event::get_instance();
            administrator = Administrator::get_instance();

            // insert window
            administrator->SET_WINDOW(window);

            // if all passed enable
            isRunning = true;
            SDL_Color miled_black = {60,66,61,255};
            SDL_Color white = {225,230,230,250};
            SDL_Color black = {0,0,0,0};
            vec2d size = {1400,1000};
            timer.set_max_FPS(240);

            view1 = administrator->create_view(ANY_POSITION,renderer,300,300,600,400,white,view::MODE::STATIC,size);

            //auto img = theme::get_instance()->Load_image("narayana",R"(images/Narayana.png)");
            //ir = new image_renderer(view1,img,0,0,600,360);
            tr = new text_renderer(view1,"Whenever dharma declines, oh son of Bharata and evil prevails, I incarnate myself. To destroy the evil-doers and restore dharma in the hearth of men, I am born from ages to ages.",miled_black,200,0,be::text_renderer::WRAPPED);
            wrp = new wrapper(view1,20,20,200,200,0,1);
            text_in = new text_input(view1,"text input 1",200,300,250);
            text_in->Enable();

            return "";
        }


        void RUN()
        {
            num++;
            timer.frame_sync();
            UPDATE();

            vec2d mouse = Administrator::get_instance()->get_mouse_pos(view1);

            if(Administrator::get_instance()->get_special_key_state("f5") == Event::key_state::held)
                view1->zoom_in();
            else if(Administrator::get_instance()->get_special_key_state("f6") == Event::key_state::held)
                view1->zoom_out();
            else if(Administrator::get_instance()->get_special_key_state("mouse right") == Event::key_state::held && (former_mouse.x != mouse.x || former_mouse.y != mouse.y))
            {
                view1->pan_horizontally(former_mouse.x - mouse.x);
                view1->pan_vertically(former_mouse.y - mouse.y);
            }


            //ir->Render();
            tr->Render();

            text_in->Logic(mouse);
            text_in->Update();
            text_in->Render();

            administrator->RENDER_PRESENT();
            SDL_RenderPresent(renderer);
            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderClear(renderer);

            former_mouse = mouse;
        }

        void TERMINATE()
        {
            TTF_Quit();
            IMG_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }

        void UPDATE()
        {
            administrator->EVENT_POOL();

            if(administrator->get_special_key_state("quit") == Event::key_state::click || administrator->get_special_key_state("esc") == Event::key_state::click)
                isRunning = false;
        }

        bool is_running()
        {
            return isRunning;
        }
    };
}
#endif // UI_INCLUDE_H

#ifndef TEST_INCLUDE_H
#define TEST_INCLUDE_H


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
#include "value_adjuster.h"

#define RENDERER_SYNC           SDL_RENDERER_PRESENTVSYNC
#define RENDERER_ACCELERATED    SDL_RENDERER_ACCELERATED
#define RENDERER_SOFTWARE       SDL_RENDERER_SOFTWARE
#define RENDERER_TARGETTEXTURE  SDL_RENDERER_TARGETTEXTURE


namespace be
{
    static int num = 0;
    class TEST
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
        text_renderer* tr2;
        text_input* text_in;
        text_input* text_in2;

        value_adjustment* va;

        SDL_Surface* surf;
        SDL_Texture* texture = NULL;

        vec2d former_mouse;
        vec2d mouse_offset;

    public:
        std::string INIT(const char* title,int window_w, int window_h, UINT32 renderer_flag)
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
            administrator->INIT(window);

            // if all passed enable
            isRunning = true;
            SDL_Color bebe = {250,255,255,255};
            SDL_Color white = {225,230,230,250};
            SDL_Color black = {0,0,0,0};
            vec2d size = {1400,1000};
            timer.set_max_FPS(240);

            view1 = administrator->create_view(be::Administrator::ANY_POSITION,renderer,300,300,600,400,white,view::MODE::STATIC,size);
            //view2 = administrator->create_view(be::Administrator::ALWAYS_ON_TOP,renderer,300,250,400,250,white,view::MODE::STATIC,size);
            view3 = administrator->create_view(be::Administrator::ANY_POSITION,renderer,700,250,500,400,bebe,view::MODE::STATIC,size);

            //auto img = theme::get_instance()->Load_image("narayana",R"(images/Narayana.png)");
            //ir = new image_renderer(view2,img,0,0,600,360);
            tr = new text_renderer(view1,"Whenever dharma declines, oh son of Bharata and evil prevails, i incarnate myself, to destroy the evil-doers and restore dharma in the hearth of men. I am born from ages to ages.",black,200,20,be::text_renderer::WRAPPED);
            tr2 = new text_renderer(view3,"press f5 to zoom in, press f6 to zoom out, hold the mouse right key and move mouse to pan,",black,200,20,be::text_renderer::WRAPPED);
            wrp = new wrapper(view1,20,20,200,200,0,1);
            text_in = new text_input(view1,"text input 1",200,300,250);
            text_in2 = new text_input(view3,"text input 2",200,200,250);
            va = new value_adjustment(view3,"va1",-100,100, 200,100);

            va->Enable();
            text_in->Enable();
            text_in2->Enable();


            SDL_Surface* narayana = LOAD_IMAGE(R"(images/Narayana.png)");

            if(narayana)
            {
                //uint32_t* p = (uint32_t*)narayana->pixels;
                SDL_Surface* round_vishnu = make_round_edges(narayana,26.60);
                texture = SDL_CreateTextureFromSurface(renderer,round_vishnu);

                SDL_FreeSurface(round_vishnu);
                SDL_FreeSurface(narayana);
            }

            mouse_offset = {0,0};

            return "";
        }


        void RUN()
        {
            num++;
            timer.frame_sync();
            UPDATE();


            vec2d mouse;
            if(Administrator::get_instance()->get_active_view() == view1)
                mouse = Administrator::get_instance()->get_mouse_pos(view1);
            else if(Administrator::get_instance()->get_active_view() == view3)
                mouse = Administrator::get_instance()->get_mouse_pos(view3);

            be::view* v  = (Administrator::get_instance()->get_active_view() == view1)? view1 : view3;

            if(Administrator::get_instance()->get_open_key_state("f5") == Event::key_state::held)
                v->zoom_in();
            else if(Administrator::get_instance()->get_open_key_state("f6") == Event::key_state::held)
                v->zoom_out();
            else if(Administrator::get_instance()->get_open_key_state("mouse right") == Event::key_state::held && (former_mouse.x != mouse.x || former_mouse.y != mouse.y))
            {
                v->pan_horizontally(former_mouse.x - mouse.x);
                v->pan_vertically(former_mouse.y - mouse.y);
            }
            else if(Administrator::get_instance()->get_open_modstate("LSHIFT") == true)
            {
                if(mouse_offset.x == 0 && mouse_offset.y == 0)
                {
                    mouse_offset.x = v->get("x") - mouse.x;
                    mouse_offset.y = v->get("y") - mouse.y;
                }
                else
                {
                    v->set("x", mouse.x + mouse_offset.x);
                    v->set("y", mouse.y + mouse_offset.y);
                }
            }
            else
            {
                mouse_offset = {0,0};
            }

            tr2->clear();

            be::Line A = {400,0,va->get_current_value(),400}, B = {200,0,200,400};

            vec2d I = be::get_line_intersection_point(A,B);
            vec2d rm = view1->resolve_point(mouse);
            std::string strr = be::to_string(I.x) + " , " + be::to_string(I.y) + " Mouse ( "+be::to_string(rm.x) + " , " + be::to_string(rm.y) + " )";

            tr2 << strr;

            //ir->Render();
            tr->Render();
            tr2->Render();

            text_in->Logic(mouse);
            text_in->Update();
            text_in->Render();

            text_in2->Logic(mouse);
            text_in2->Update();
            text_in2->Render();

            va->Logic(mouse);
            va->Update();
            va->Render();

            SDL_Rect dis = { 20,20,100,100};

            view1->RenderDrawLine(NULL,A, {0,0,0,255});
            view1->RenderDrawLine(NULL,B, {0,0,0,255});

            if(texture)
                view1->RenderCopy(texture,NULL,NULL,&dis);
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

            if(administrator->get_open_key_state("quit") == Event::key_state::click || administrator->get_open_key_state("esc") == Event::key_state::click)
                isRunning = false;
        }

        bool is_running()
        {
            return isRunning;
        }
    };
}
#endif // TEST_INCLUDE_H10

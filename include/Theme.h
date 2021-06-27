#ifndef THEME_MANAGER_H_INCLUDED
#define THEME_MANAGER_H_INCLUDED

#include "common.h"
#include "File.h"

namespace be
{
     /* \note while deleting font theme does not reassign default font
     * \sa   class be::Renderer
     */
    class theme
    {
        theme() = default;
    public:
        struct FONT_NODE { int font_size; TTF_Font* font;};
        struct IMG_NODE{ std::string path; SDL_Texture* img = nullptr; };
        struct MUSIC_NODE{ std::string path; Mix_Music* music; };
        struct DEFAULT_FONT{ FONT_NODE* node = nullptr; std::string font_name; };
        struct COLOR_NODE{ SDL_Color color; };
    private:
        struct FONT_STACK{ std::string path; std::list<FONT_NODE> fonts; };

        std::unordered_map<std::string, IMG_NODE> images;
        std::unordered_map<std::string, FONT_STACK> fonts;
        std::unordered_map<std::string, MUSIC_NODE> musics;
        std::vector<COLOR_NODE> colors;

        SDL_Renderer* renderer = nullptr;
        DEFAULT_FONT default_font;
        static theme* instance;
        bool has_defualt_font_changed = false;
    public:
        static theme* get_instance(){ return instance = (instance == nullptr)? new theme() : instance; };


        void Destroy_theme()
        {
            clear();
            if(instance)
                delete instance;
        }


        void INIT(SDL_Renderer* renderer)
        {
            this->renderer = renderer;
        }

        /* \note order of adding colors indicates importance
         *    index (0 -> primary background color, 1 -> primary foreground color, 2 -> secondary background color,  3 -> secondary background color and so on).
         */
        const COLOR_NODE* add_color(const SDL_Color cl)
        {
            COLOR_NODE c;
            c.color = cl;
            colors.push_back(c);
            return &(colors[colors.size() - 1]);
        }

        const IMG_NODE* Load_image(std::string uuid, std::string path, std::string* err = NULL)
        {
            if(renderer)
            {
                // free former texture if id already exists
                for(auto &i : images)
                {
                    if(i.first == uuid)
                    {
                        if(i.second.img)
                            SDL_DestroyTexture(i.second.img);
                        break;
                    }
                }
                IMG_NODE N;
                N.path = path;
                N.img = IMG_LoadTexture(renderer,path.c_str());

                if(N.img)
                {
                    // save image with id
                    images[uuid] = N;
                    return &(images[uuid]);
                }

                if(err)
                    (*err) = SDL_GetError();
                return nullptr;
            }
            if(err)
                (*err) = "theme not initilized";
            return nullptr;
        }
    private:
        FONT_NODE* __load_font(std::string& font_name, std::string& path, int& font_size, std::string* err = NULL)
        {
            TTF_Font* font = TTF_OpenFont(path.c_str(),font_size);

            if(font)
            {
                FONT_NODE f;
                f.font_size = font_size;
                f.font = font;

                // check if font name node already exist
                for(auto &i : fonts)
                {
                    if(i.first == font_name)
                    {
                        // return it if the specific font size already exists
                        for(auto &n : i.second.fonts)
                            if(font_size == n.font_size) return &n;

                        // if size doesn't exist then add it and return
                        i.second.fonts.push_back(f);
                        return &(*(--i.second.fonts.end()));
                    }
                }

                std::list<FONT_NODE> ft;
                ft.push_back(f);

                FONT_STACK N;
                N.path = path;
                N.fonts = ft;
                fonts[font_name] = N;

                // set font as default if no other font exist
                if(fonts.size() == 1 && !default_font.node )
                {
                    default_font.node = &(*(fonts[font_name].fonts.begin()));
                    default_font.font_name = font_name;
                }

                return &(*(--fonts[font_name].fonts.end()));
            }
            else
            {
                if(err)
                    (*err) = SDL_GetError();
                return nullptr;
            }
        }
    public:
        const FONT_NODE* load_font(std::string font_name, std::string path, int font_size, std::string* err = NULL)
        {
            return __load_font(font_name, path, font_size, err);
        }


        const MUSIC_NODE* load_music(std::string uuid, std::string path,std::string* err = NULL)
        {
            Mix_Music* m = Mix_LoadMUS(path.c_str());

            if(m)
            {
                for(auto &i : musics)
                {
                    if(i.first == uuid)
                    {
                        Mix_FreeMusic(i.second.music); break;
                    }
                }
                MUSIC_NODE N;
                N.music = m;
                N.path = path;

                musics[uuid] = N;
                return &(musics[uuid]);
            }
            else
            {
                if(err)
                    (*err) = "Couldn't load "+path;
                return nullptr;
            }
        }

        bool set_default_font(std::string font_name, int fontsize)
        {
            auto f_node = __get_font(font_name,fontsize);
            if(f_node)
            {
                default_font.node = f_node;
                default_font.font_name = font_name;
                has_defualt_font_changed = true;
                return true;
            }
            else return false;
        }


        const DEFAULT_FONT* get_default_font()
        {
            return &default_font;
        }


        const IMG_NODE* get_image(const std::string id)
        {
            for(auto &i : images)
            {
                if(i.first == id)
                {
                    return &(i.second);
                }
            }
            return nullptr;
        }

    private:
        FONT_NODE* __get_font(std::string& font_name, int& font_size)
        {
            std::string path = "";
            for(auto &i : fonts)
            {
                if(font_name == i.first)
                {
                    path = i.second.path;
                    for(auto &f : i.second.fonts)
                    {
                        if(f.font_size == font_size)
                            return &f;
                    }
                    break;
                }
            }
            // trial loading font
            return __load_font(font_name,path,font_size);
    }
    public:
        const FONT_NODE* get_font(std::string font_name, int font_size)
        {
            return  __get_font(font_name, font_size);
        }


        MUSIC_NODE* get_music(const std::string id)
        {
            for(auto &m : musics)
            {
                if(m.first == id)
                {
                    return &(m.second);
                }
            }
            return nullptr;
        }


        const COLOR_NODE* get_color(unsigned int index)
        {
            if(index < colors.size())
                return &colors[index];
            else
                return nullptr;
        }


        void remove_image(const std::string id)
        {
            for(auto i = images.begin(); i != images.end(); ++i)
            {
                if(i->first == id)
                {
                    SDL_DestroyTexture(i->second.img);
                    images.erase(i);
                    break;
                }
            }
        }


        void remove_music(const std::string id)
        {
            for(auto i = musics.begin(); i != musics.end(); ++i)
            {
                if(i->first == id)
                {
                    Mix_FreeMusic(i->second.music);
                    musics.erase(i);
                    break;
                }
            }
        }



        void remove_font(const std::string font_name)
        {
            for(auto i = fonts.begin(); i != fonts.end(); ++i)
            {
                if(i->first == font_name)
                {
                    for(auto f = i->second.fonts.begin(); f != i->second.fonts.end(); ++i)
                    {
                        TTF_CloseFont(f->font);
                    }
                    fonts.erase(i);
                    break;
                }
            }
        }


        void remove_font(const std::string id, int fontsize)
        {
            for(auto i = fonts.begin(); i != fonts.end(); ++i)
            {
                if(i->first == id)
                {
                    for(auto f = i->second.fonts.begin(); f != i->second.fonts.end(); ++i)
                    {
                        if(f->font_size == fontsize)
                        {
                            TTF_CloseFont(f->font);
                            i->second.fonts.erase(f);
                            break;
                        }
                    }

                    if(i->second.fonts.size() == 0)
                        fonts.erase(i);
                    break;
                }
            }
        }

        void clear()
        {
            // clear font
            for(auto & store : fonts)
            {
                for(auto& node : store.second.fonts)
                {
                    TTF_CloseFont(node.font);
                }
            }
            fonts.erase(std::begin(fonts), std::end(fonts));

            // clear images
            for(auto& node : images)
            {
                SDL_DestroyTexture(node.second.img);
            }
            images.erase(std::begin(images), std::end(images));

            // clear musics
            for(auto& node : musics)
            {
                Mix_FreeMusic(node.second.music);
            }
            musics.erase(std::begin(musics), std::end(musics));
        }

    };
}

#endif // THEME_MANAGER_H_INCLUDED

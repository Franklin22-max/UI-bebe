#ifndef T_PARSER_H_INCLUDED
#define T_PARSER_H_INCLUDED

#include "File.h"
#include "common.h"
#include "Theme.h"
#include "string_handler.h"




/**
  Helps set theme default values based of user defined values in a text file
Parser syntax


N.B any line not starting with # is considered as a comment

A Basic Example of a typical script


COLOR OPTIONS:
TEXT
VIEW_PRIMARY
VIEW_SECONDARY
COMPONENT_BG
LINK
LINK_HOVER
LINK_CLICK
BUTTON_BG
BUTTON_FG



FONT OPTIONS:
COMPONENT
TEXT
LINK



IMAGE OPTION:
BG_VIEW
BG_COMPONENT

SOUND OPTIONS:
NOTIFICATION
ERROR



<COLOR>

# TEXT , LINK , BUTTON_FG
# ID "white"
# VALUE "255,255,255,255"


# VIEW
# ID "black"
# VALUE 0x000000

</COLOR>




<FONT>

# TEXT , LINK
# ID "white"
# VALUE "fonts/arial.ttf" , 11

</FONT>

<IMAGE>


</IMAGE>







this is best if extra values are given but are not to be used as defaults, possibly to be called by the process at some point

*/

namespace be
{
    class theme_parser
    {
        be::File* file;
        enum class obj   { image, color, font, sound, none} m_obj = obj::none;
        enum class tag   { open , closed} m_tag = tag::closed;
        enum class stage  { option, id, value} m_stage = stage::option;
        std::vector<std::string> options;
        std::string ID;

    public:
        parser(std::string path)
        {
            file = new File(path);
            this->interprete();
        }
    private:


        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */

        void link_colors(std::string& command, const int& line)
        {
            std::stringstream ss;
            std::string hold;
            bool success = true;

            if(m_stage == stage::id)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "ID")
                {
                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        ID = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid ID at line ") + to_string(line+1));
                        success = false;
                    }
                }
                else
                {
                    Error::write_error(std::string("THEME PARSER ERROR: expected ID ") + to_string(line+1));
                    success = false;
                }

                m_stage = stage::value;
            }
            else if(m_stage == stage::value)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "VALUE")
                {
                    SDL_Color color;

                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        std::vector<std::string> color_values = be::get_seperated_words(hold,",");
                        if(color_values.size() == 4) // individual values was given for each color value
                        {
                            Uint8* color_ptr = (Uint8*)&color;
                            for(int i = 0; i < color_values.size(); i++)
                            {
                                Uint8 num = -1;
                                num = to_number(color_values[i]);
                                if(num > -1)
                                    *(color_ptr + i) = num;
                                else
                                {
                                    Error::write_error(std::string("THEME PARSER ERROR: Invalid value for color, in line ") + to_string(line+1));
                                    success = false;
                                }
                            }
                        }
                        else
                        {
                            Error::write_error(std::string("THEME PARSER ERROR: Too many argument for color, in line ") + to_string(line+1));
                            success = false;
                        }

                    }
                    else
                    {
                        ss >> hold;
                        if(hold != "")// integer value was given for the color
                        {
                            uint32_t icolor = to_number(hold);
                            color = int_to_color(icolor);
                        }
                        else
                        {
                            Error::write_error(std::string("THEME PARSER ERROR: invalid argument for color, in line ") + to_string(line+1));
                            success = false;
                        }
                    }

                    if(success && ID != "")
                    {
                        theme::COLOR_NODE* ptr = theme::get_instance()->add_color(ID,color);
                        if(ptr)
                        {
                            auto node = &theme::defaults[theme::Theme_object::COLOR];
                            for(auto &option : options)
                            {
                                if(option == "TEXT")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::TEXT , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "VIEW_PRIMARY")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::VIEW_PRIMARY , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "VIEW_SECONDARY")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::VIEW_SECONDARY , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "COMPONENT_BG")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::COMPONENT_BG , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "LINK")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::LINK , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "LINK_CLICK")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::LINK_CLICK , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "LINK_HOVER")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::LINK_HOVER , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "BUTTON_BG")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::BUTTON_BG , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "BUTTON_FG")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_COLOR_OPTION::BUTTON_FG , theme::DEFAULT_NODE(ID, ptr)));
                                else
                                    Error::write_error(std::string("THEME PARSER ERROR: invalid option for color, before line ") + to_string(line+1));
                            }
                        }
                    }

                }
                else
                    Error::write_error(std::string("THEME PARSER ERROR: invalid identifier in line, ") + to_string(line+1));

                m_stage = stage::option;
                if(options.size() > 0)
                    options.erase(options.begin(),options.end());
                ID = "";

            }
        }





        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */

        int link_images(std::string& command, const int& line)
        {
            std::stringstream ss;
            std::string hold;
            bool success = true;

            if(m_stage == stage::id)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "ID")
                {
                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        ID = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid ID at line ") + to_string(line+1));
                        success = false;
                    }
                }
                else
                {
                    Error::write_error(std::string("THEME PARSER ERROR: expected ID ") + to_string(line+1));
                    success = false;
                }
                m_stage = stage::value;
            }
            else if(m_stage == stage::value)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "VALUE")
                {
                    std::string path = "";

                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        path = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid value for image : line ") + to_string(line+1));
                        success = false;
                    }

                    if(success && ID != "")
                    {
                        std::string err;
                        auto ptr = theme::get_instance()->Load_image(ID,path,&err);
                        if(ptr != nullptr)
                        {

                            std::map<int, theme::DEFAULT_NODE>* node = &theme::get_instance()->defaults[theme::Theme_object::IMAGE];
                            for(auto &option : options)
                            {
                                if(option == "BG_VIEW")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_IMAGE_OPTION::BG_VIEW , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "BG_COMPONENT")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_IMAGE_OPTION::BG_COMPONENT , theme::DEFAULT_NODE(ID, ptr)));
                                else
                                    Error::write_error(std::string("THEME PARSER ERROR: invalid option for image, before line ") + to_string(line+1));
                            }
                        }
                        else
                            Error::write_error(std::string("THEME PARSER ERROR: unable to load image: ") + err + ": line " + to_string(line+1));
                    }
                }
                else
                    Error::write_error(std::string("THEME PARSER ERROR: invalid identifier in line, ") + to_string(line+1));

                m_stage = stage::option;
                if(options.size() > 0)
                    options.erase(options.begin(),options.end());
                ID = "";

            }
        }




        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */

        void link_fonts(std::string& command, const int& line)
        {
            std::stringstream ss;
            std::string hold;
            bool success = true;

            if(m_stage == stage::id)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "ID")
                {
                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        ID = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid ID at line ") + to_string(line+1));
                        success = false;
                    }
                }
                else
                {
                    Error::write_error(std::string("THEME PARSER ERROR: expected ID ") + to_string(line+1));
                    success = false;
                }

                m_stage = stage::value;
            }
            else if(m_stage == stage::value)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "VALUE")
                {
                    std::string path = "";
                    int fontsize = -1;

                    path = be::get_quoted_string(command);
                    hold = be::replace_string_with(command,path,"");
                    auto words = be::get_seperated_words();

                    if(words.size() > 1)
                        fontsize = words[1];

                    if(path  == "" || fontsize <= -1)
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid value for font, expects eg \"path\font.ttf\" , fontsize     :line ") + to_string(line+1));
                        success = false;
                    }

                    if(success && ID != "")
                    {
                        std::string err;
                        auto ptr = theme::get_instance()->load_font(ID,path,fontsize,&err);
                        if(ptr != nullptr)
                        {

                            std::map<int, theme::DEFAULT_NODE>* node = &theme::get_instance()->defaults[theme::Theme_object::FONT];
                            for(auto &option : options)
                            {
                                if(option == "COMPONENT")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_FONT_OPTION::COMPONENT , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "LINK")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_FONT_OPTION::LINK , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "TEXT")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_FONT_OPTION::TEXT , theme::DEFAULT_NODE(ID, ptr)));
                                else
                                    Error::write_error(std::string("THEME PARSER ERROR: invalid option for font, before line ") + to_string(line+1));
                            }
                        }
                        else
                            Error::write_error(std::string("THEME PARSER ERROR: unable to load font: ") + err + ": line " + to_string(line+1));
                    }
                }
                else
                    Error::write_error(std::string("THEME PARSER ERROR: invalid identifier in line, ") + to_string(line+1));

                m_stage = stage::option;
                if(options.size() > 0)
                    options.erase(options.begin(),options.end());
                ID = "";

            }
        }



        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */


        void link_sounds(std::string& command, const int& line)
        {
                       std::stringstream ss;
            std::string hold;
            bool success = true;

            if(m_stage == stage::id)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "ID")
                {
                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        ID = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid ID at line ") + to_string(line+1));
                        success = false;
                    }
                }
                else
                {
                    Error::write_error(std::string("THEME PARSER ERROR: expected ID ") + to_string(line+1));
                    success = false;
                }
                m_stage = stage::value;
            }
            else if(m_stage == stage::value)
            {
                ss.str(command);
                ss >> hold;
                if(hold == "VALUE")
                {
                    std::string path = "";

                    hold = be::get_quoted_string(command);
                    if(hold != "")
                    {
                        path = hold;
                    }
                    else
                    {
                        Error::write_error(std::string("THEME PARSER ERROR: invalid value for sound : line ") + to_string(line+1));
                        success = false;
                    }

                    if(success && ID != "")
                    {
                        std::string err;
                        auto ptr = theme::get_instance()->load_music(ID,path,&err);

                        if(ptr != nullptr)
                        {
                            std::map<int, theme::DEFAULT_NODE>* node = &theme::get_instance()->defaults[theme::Theme_object::SOUND];
                            for(auto &option : options)
                            {
                                if(option == "BG_VIEW")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_SOUND_OPTION::NOTIFICATION , theme::DEFAULT_NODE(ID, ptr)));
                                else if(option == "BG_COMPONENT")
                                    node->insert(node->begin(),std::pair<int,theme::DEFAULT_NODE>((int)theme::DEFAULT_SOUND_OPTION::ERROR , theme::DEFAULT_NODE(ID, ptr)));
                                else
                                    Error::write_error(std::string("THEME PARSER ERROR: invalid option for sounds, before line ") + to_string(line+1));
                            }
                        }
                        else
                            Error::write_error(std::string("THEME PARSER ERROR: unable to load sound: ") + err + ": line " + to_string(line+1));
                    }
                }
                else
                    Error::write_error(std::string("THEME PARSER ERROR: invalid identifier in line, ") + to_string(line+1));

                m_stage = stage::option;
                if(options.size() > 0)
                    options.erase(options.begin(),options.end());
                ID = "";

            }
        }




        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */


        int interprete()
        {
            if(!file->empty())
            {

                std::string command = "";
                std::stringstream ss;
                auto script = file->get_file_buffer();
                for(int line = 0; line < script.size(); line++)
                {
                    if((script[line]).length() > 4)
                    {
                        command = be::strip_edges(script[line],' ');
                        command = be::strip_edges(command, '\t');

                        std::stringstream ss;
                        std::string word;

                        if(command[0] != '<' || command[0] != '#')// probably a comment
                            continue;
                        else if((command[0] == '<') && (command[1] != '/') && (m_tag == tag::closed))// open a tag
                        {
                            command = strip(command,"<");
                            command = strip(command,">");
                            ss.str(command);

                            ss >> word;

                            if(word == "IMAGE")
                                m_obj = obj::image;
                            else if(word == "COLOR" || word == "COLOUR")
                                m_obj = obj::color;
                            else if(word == "FONT")
                                m_obj = obj::font;
                            else if(word == "SOUND")
                                m_obj = obj::sound;
                            else
                            {
                                m_obj = obj::none;
                                Error::write_error(std::string("THEME PARSER ERROR: invalid tag in line ") + to_string(line+1));
                                return -1;
                            }

                            m_tag = tag::open;

                        }
                        else if(command[0] == '#' && m_tag == tag::open && m_obj != obj::none)// command
                        {
                            command = strip_edges(command,'#');
                            ss.str(command);
                            std::string hold = "";
                            ss >> hold;

                            if(m_stage == stage::option && hold != "VALUE")
                            {
                                if(m_stage == stage::option)
                                options = get_seperated_words(command,",");
                                if(options.size() == 0)
                                {
                                    ss.str(command);
                                    ss >> hold;
                                    if(hold != "")
                                        options.push_back(hold);
                                }
                                m_stage = stage::id;
                            }
                            else
                            {
                                m_stage = stage::id;
                                if(m_obj == obj::color)
                                    link_colors(command,line);
                                else if(m_obj == obj::font)
                                    link_fonts(command,line);
                                else if(m_obj == obj::image)
                                    link_images(command,line);
                                else if(m_obj == obj::sound)
                                    link_sounds(command,line);
                            }

                        }
                        else if(command[0] == '<' && command[1] == '/' && m_tag == tag::open && m_stage == stage::value)// for closing tage
                        {
                            m_tag = tag::closed;
                            m_stage = stage::option;
                            m_obj = obj::none;
                            // reset to default values
                            if(options.size() > 0)
                                options.erase(options.begin(),options.end());
                            ID = "";
                        }
                        else
                        {
                            Error::write_error(std::string("THEME PARSER ERROR: invalid command in line ") + to_string(line+1));
                            return -1;
                        }

                    }
                }
            }
        }




    };
}

#endif // PARSER_H_INCLUDED

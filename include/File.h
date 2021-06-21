#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED


#include <boost/filesystem.hpp>
#include "Administrator.h"
#include <string>
#include <commdlg.h>

namespace sys = boost::system;

namespace be
{
    std::string savefilename(char *filter , HWND owner);
    std::string openfilename(char *filter , HWND owner);
    SDL_Color ChooseColor();

    class FileReadder
    {
        struct LINE_DATA
        {
            LINE_DATA(uint16_t line, boost::filesystem::fstream::pos_type pos)
            :pos(pos), line(line){};

            uint16_t line;
            boost::filesystem::fstream::pos_type pos;
        };

        int current_line;
        boost::filesystem::fstream::pos_type end_pos = 0;// stores the next position to be read by fstream file
        boost::filesystem::path path;
        boost::filesystem::fstream file;
        std::vector<LINE_DATA> lines;

    public:
        FileReadder() = default;

        FileReadder(boost::filesystem:: path)
        {
            open_file(path);
        }

        bool open_file(boost::filesystem::path _path)
        {
            if(boost::filesystem::exists(_path))
            {
                if(file.is_open())
                    close();
                path = _path;
                current_line = 1;
                file.open(path.string(), std::ios::in | std::ios::binary);
                return (file.is_open());
            }
            else
                return 0;
        }

        // return 0 if successful and -1 otherwise
        std::string read_next_line()
        {
            std::string dest = "";
            if(current_line < lines.size() && current_line > 0)// line has been loaded before
            {
                std::getline(file,dest);
                current_line;
                goto_line(current_line + 1);
            }
            else if(file.is_open() && file.peek() != EOF)
            {
                // get starting line position
                auto pos = file.tellg();
                std::getline(file,dest);

                auto record_line_data = [&,this]()
                {
                    // store line info
                    lines.emplace_back(lines.size()+1, pos);
                    current_line = lines.size();
                    end_pos = file.tellg();
                };

                if(lines.size() == 0)
                    record_line_data();
                else if(pos != lines[current_line - 1].pos)
                    record_line_data();
            }
            return dest;
        }

        void goto_line(uint16_t line)
        {
            if(line > lines.size())
            {
                goto_end();
                for(int i=0; i < line - lines.size(); i++)
                    read_next_line();
            }
            else if(lines.size() > 0 && line > 0 && line <= lines.size())
            {
                current_line = line;
                file.seekg(lines[current_line - 1].pos);
            }
        }

        bool at_end_of_file()
        {
            if(!file.is_open()) return true;    else return(file.peek() == EOF);
        }

        void goto_end()
        {
            if(lines.size() > 0)
            {
                current_line = lines.size();
                file.seekg(end_pos);
            }
        }

        uint16_t get_current_line()
        {
            return current_line;
        }

        // clear data and reset the file position to the beginning
        void reset()
        {
            lines.erase(lines.begin(), lines.end());
            current_line = 0;
            end_pos = 0;
            file.seekg(0);
        }

        // closes file and path object and clean data
        void close()
        {
            if(file.is_open())
                file.close();
            path.clear();
            lines.erase(lines.begin(), lines.end());
        }

        boost::filesystem::path get_path()
        {
            return path;
        }

        uintmax_t get_file_size()
        {
            return boost::filesystem::file_size(path);
        }
    };






   // Returns an empty string if dialog is canceled
    inline std::string savefilename(char *filter = "All Files (*.*)\0*.*\0", HWND owner = Administrator::get_instance()->Get_Window_handler())
    {
        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = owner;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;

        if (GetSaveFileName(&ofn))
            fileNameStr = fileName;

        return fileNameStr;
    }


    // Returns an empty string if dialog is canceled
    inline std::string openfilename(char *filter = "All Files (*.*)\0*.*\0", HWND owner = Administrator::get_instance()->Get_Window_handler())
    {
        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = owner;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;

        if( GetOpenFileName(&ofn) )
            fileNameStr = fileName;

        return fileNameStr;
    }


    /** \brief  Opens a Color chooser dialog and let user pick colors dynamically
     *
     * \return  SDL_Color  filled with color data
     * \note the alpha value for return color is always 255
     */

    inline SDL_Color ChooseColor()
    {
        CHOOSECOLOR cc;                 // common dialog box structure
        static COLORREF acrCustClr[16]; // array of custom colors
        //HBRUSH hbrush;                  // brush handle
        static DWORD rgbCurrent;        // initial color selection

        // Initialize CHOOSECOLOR
        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner =  Administrator::get_instance()->Get_Window_handler();
        cc.lpCustColors = (LPDWORD) acrCustClr;
        cc.rgbResult = rgbCurrent;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

        if (ChooseColor(&cc)==TRUE)
        {
            //hbrush = CreateSolidBrush(cc.rgbResult);
            rgbCurrent = cc.rgbResult;

            SDL_Color cl; cl.a = 255;/*(BYTE)(rgbCurrent >> 24);*/ cl.b = (BYTE)(rgbCurrent >> 16) & 255; cl.g = (BYTE)(rgbCurrent >> 8) & 255; cl.r = (BYTE)(rgbCurrent & 255);
            return cl;
        }
    }

}


#endif // FILE_H_INCLUDED

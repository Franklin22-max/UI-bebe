#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED


#include "Administrator.h"
#include <string>
#include <commdlg.h>


namespace be
{

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

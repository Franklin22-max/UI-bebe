#include "include/UI.h"
#include "include/Event.h"
#include "include/text_input.h"



int main(int argc, char* argv[])
{
    std::string ec("");
    be::UI MY_UI;
    ec = MY_UI.INIT("UI TEST",1200,800,RENDERER_ACCELERATED | RENDERER_SYNC);

    if(!ec.empty())
        std::cout<<ec<<"\n";

    while(MY_UI.is_running())
    {
        MY_UI.RUN();
    }

    MY_UI.TERMINATE();
    return 0;
}

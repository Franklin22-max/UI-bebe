#include "include/TEST.h"
#include "include/Event.h"
#include "include/text_input.h"



int main(int argc, char* argv[])
{

    std::string src = "\"the golden broom ,rope ,bucket ,stream ,ocean\" ,see andis with the value\"";
    auto words = be::get_seperated_words(src,",");

    std::cout<<be::get_quoted_string(src)<<"\n";

    for(auto &i : words)
    {
        std::cout<<i<<"\n";
    }

    std::string ec("");
    be::TEST MY_TEST;
    ec = MY_TEST.INIT("TEST TEST",1200,800,RENDERER_ACCELERATED | RENDERER_SYNC);

    if(!ec.empty())
        std::cout<<ec<<"\n";

    while(MY_TEST.is_running())
    {
        MY_TEST.RUN();
    }

    MY_TEST.TERMINATE();
    return 0;
}

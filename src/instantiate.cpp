#include "../include/TEST.h"
#include "../include/Theme.h"


namespace be
{
    vec2d view::max_texture_size;

    Event* Event::instance = nullptr;

    Administrator* Administrator::instance = nullptr;

    theme* theme::instance = nullptr;

    tsqueue<std::string> Error::error_messages;
}

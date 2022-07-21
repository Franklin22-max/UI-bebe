#ifndef STRING_HANDLER_H_INCLUDED
#define STRING_HANDLER_H_INCLUDED

#include <string>
#include <vector>

namespace be
{

    inline std::string to_string(double num)
    {
        std::stringstream ss;
        ss << num;
        return ss.str();
    }



    inline long double to_number(std::string str)
    {
        long double num;
        std::stringstream ss;
        ss << str;
        ss >> num;
        return num;
    }



    inline std::string substr(const std::string src, int s, int e)
    {
        std::string h("");
        for(; s < e; s++)
            h += src[s];
        return h;
    }



    inline std::string insert(const std::string src, int i, std::string value)
    {
        return (substr(src,0,i) + value + substr(src,i,src.length()));
    }



    inline std::string remove(const std::string src, int i, uint32_t length = 1)
    {
        return (substr(src,0,i) + substr(src,i + length ,src.length()));
    }



    // strips a specific word or sentence from through out a string
    inline std::string strip(const std::string src,const std::string _char)
    {
        if(_char.length() <= src.length())
        {
            bool skipLastFill = false;
            std::string hold = "";
            for(int i = 0; i < src.length() - _char.length(); i++)
            {
                if(be::substr(src, i , i + _char.length())  == _char)
                {
                    if(src.length() - _char.length()-1 == i)
                        skipLastFill = true;
                    i += (_char.length()-1);
                }
                else
                    hold += src[i];

            }

            std::string str = be::substr(src, src.length()-_char.length() , src.length());
            // add the remaining values to the string
            if(!skipLastFill)
                hold += (str == _char)? "" : str;

            return hold;
        }
        else
            return "";
    }


    inline std::string strip_edges(const std::string src,const char _char)
    {
        if(src.length() > 0)
        {
            int start = 0;
            int end = src.length();

            for(int i = 0; i < src.length(); i++)
            {
                if(src[i] != _char)
                {
                    start = i;
                    break;
                }
            }

            for(int i = src.length()-1; i >= 0; i--)
            {
                if(src[i] != _char)
                {
                    end = i + 1;
                    break;
                }
            }
            return be::substr(src,start, end);
        }
        return "";
    }

    // replace all the specified character in the src with another character
    inline std::string replace_char_with(const std::string src, char toBeReplaced, char replacement)
    {
        std::string hold = "";
        for(int i = 0; i < src.length(); i++)
        {
            if(src[i] == toBeReplaced)
                hold += replacement;
            else
                hold += src[i];
        }
        return hold;
    }


    // replaces a given sentence or word in the source with another sentence or word specified
    inline std::string replace_string_with(const std::string src, std::string toBeReplaced, std::string replacement)
    {
        if(src.length() >= toBeReplaced.length())
        {
            bool skipLastFill = false;
            std::string hold = "";
            for(int i = 0; i < src.length() - toBeReplaced.length(); i++)
            {
                if(be::substr(src, i , i + toBeReplaced.length())  == toBeReplaced)
                {
                    hold += replacement;

                    if( i == src.length() - toBeReplaced.length()-1)
                        skipLastFill = true;

                    i += (toBeReplaced.length() -1);
                }
                else
                    hold += src[i];
            }

            std::string str = be::substr(src, src.length() - toBeReplaced.length()  ,  src.length());
            // add the remaining values to the string
            if(!skipLastFill)
                hold += (str == toBeReplaced)? replacement : str;
            return hold;
        }
    }


    // returns all words that is separated by some character or word     eg home , ball
    inline std::vector<std::string> get_seperated_words(const std::string src, std::string separator)
    {
        std::string beta = " ";
        beta += (separator + " ");
        std::string hold = replace_string_with(src,separator,beta);
        std::stringstream ss;
        ss.str(hold);
        std::string prev = "";
        std::vector<std::string> words;

        while(ss.peek() != EOF)
        {
            ss >> hold;
            if(hold == separator && prev != "" && prev != separator)
            {
                if(words.size() > 0 )
                {
                    if(prev != words[words.size() - 1])
                        words.push_back(prev);
                }
                else
                    words.push_back(prev);
            }
            else if(prev == separator && hold != "" && hold != separator && words.size() > 0)
                    words.push_back(hold);
            prev = hold;
        }
        return words;
    }


    // returns the string characters enclosed by string quotes
    inline std::string get_quoted_string(const std::string src)
    {
        char prev = NULL;
        int start = -1, end = -1;

        for(int i=0; i < src.length(); i++)
        {
            if(src[i] == '"')
            {
                if(start < 0)
                    start = i;
                else if(start >= 0 && end < 0 && prev != '\\' && prev != NULL)
                {
                    end = i;
                    break;
                }
            }
            prev = src[i];
        }

        if(start >= 0 && end >= 1 && start < end)
            return strip_edges(be::substr(src, start, end),'"');
        else
            return "";
    }


}

#endif // STRING_HANDLER_H_INCLUDED

#ifndef RANDOM_VECTOR_H_INCLUDED
#define RANDOM_VECTOR_H_INCLUDED

#include <vector>

namespace be
{
    template <typename TYPE, typename... Args>
    class random_vector
    {
        std::vector<TYPE> vect;
    public:
        using iterator = typename std::vector<TYPE>::iterator;

        iterator begin()
        {
            return vect.begin();
        }

        iterator end()
        {
            return vect.end();
        }

        uint32_t size()
        {
            return vect.size();
        }

        void push_back(TYPE data)
        {
            vect.push_back(data);
        }

        TYPE pop_back()
        {
            TYPE hold = vect[vect.size()-1];
            vect.pop_back();
            return hold;
        }

        const TYPE* data()
        {
            return vect.data();
        }

        std::vector<TYPE> subvect(int start, int end)
        {
            std::vector<TYPE> s;
            for(; start < end; start++)
                s.push_back(vect[start]);
            return s;
        }

        void insert(int i, TYPE data) noexcept
        {
            std::vector<TYPE> s = subvect(0,i);

            s.push_back(data);

            for(; i < vect.size(); i++)
                s.push_back(vect[i]);
            vect = s;
        }

        void erase(int i, uint32_t length = 1) noexcept
        {
            std::vector<TYPE> s = subvect(0,i);

            for(int p = i+length; p < vect.size(); p++)
                s.push_back(vect[p]);
            vect = s;
        }

        void erase(iterator it) noexcept
        {
            for(; it != vect.end(); ++it)
            {
                auto j = it;     ++j;
                if(j != (--vect.end())  && j != vect.end())
                {
                    *it = *j;// copy back to replace the removed element
                }
                else if(it == (--vect.end()) && j != vect.end())
                {
                    vect.pop_back();// remove the last element
                }
            }
        }

        void erase(const iterator __1,  const iterator __2)
        {
            vect.erase(__1,__2);
        }


        void insert(const iterator it, TYPE data)
        {
            int index = 0;
            for(auto i = vect.begin(); i != vect.end(); ++i)
            {
                if(i == it) break;
                index++;
            }
            insert(index,data);
        }

        void emplace_back(Args&&... args)
        {
            vect.emplace_back(args...);
        }


        void emplace(const iterator it, Args&&... args)
        {
            vect.emplace(it, args...);
        }

        bool empty()
        {
            return vect.empty();
        }

        TYPE& operator[](uint32_t index)
        {
            return vect[index];
        }

    };
}

#endif // RANDOM_VECTOR_H_INCLUDED

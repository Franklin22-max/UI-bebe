#ifndef F_TIME_H_INCLUDED
#define F_TIME_H_INCLUDED

#include <chrono>
#include <cstdint>
#include <thread>


namespace be
{
    class time
    {
        using HRC_time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

        tm     time_holder;
        double FPS;
        double presumed_FPS;
        double current_FPS;
        double min_time;
        double frame_time;
        double start_time = 0;
        double delta_time;
    public:
        time(){ FPS = 1000;}
        time(double FPS)
        :FPS(FPS)
        {}

        tm get_time_book()
        {
            // get time from system
            HRC_time_point _ctime = std::chrono::high_resolution_clock::now();
            // convert to time we can output
            time_t local_time = std::chrono::high_resolution_clock::to_time_t(_ctime);
            // convert time to current time zone
            time_holder = *localtime(&local_time);
        }
        /** \brief set maximum frame rate of program
         */

        void set_max_FPS(double fps = 1000)
        {
            FPS = (fps <= 0)? 60 : fps;
            min_time = 1000000 / FPS;
        }

        /** \brief returns what frame rate  could have been if frame was not synced to fps
         */

        double get_presumed_FPS()
        {
            return presumed_FPS;
        }

        double get_current_FPS()
        {
            return current_FPS;
        }

        double get_delta_time()
        {
            return delta_time;
        }

        void frame_sync()
        {
            // get time from system
            HRC_time_point _ctime = std::chrono::high_resolution_clock::now();
            frame_time = std::chrono::time_point_cast<std::chrono::microseconds>(_ctime).time_since_epoch().count() - start_time;
            presumed_FPS = 1000000/frame_time;

            if(min_time > frame_time)
            {
              std::this_thread::sleep_for(std::chrono::duration<std::chrono::microseconds::rep,std::chrono::microseconds::period>(std::chrono::microseconds::rep(min_time - frame_time)));
              delta_time = min_time/1000000;
              current_FPS = FPS;
            }
            else
            {
              current_FPS = presumed_FPS;
              delta_time = frame_time/1000000;
            }

            if(delta_time > 1)
              delta_time = delta_time = min_time/1000000;

            HRC_time_point _time = std::chrono::high_resolution_clock::now();
            start_time = std::chrono::time_point_cast<std::chrono::microseconds>(_time).time_since_epoch().count();
        }
    };


    // switches process ON and OFF for specified time periods
    class Timer
    {
        uint32_t prev_time = 0;
    public:
        uint16_t interval = 0;
        Timer(){}

        Timer(uint16_t milisec)
        : interval(milisec){}

        bool Gate()
        {
            if(SDL_GetTicks() - prev_time > interval)
            {
                prev_time = SDL_GetTicks();
                return true;
            }
            else return false;
        }
    };
}

#endif // F_TIME_H_INCLUDED

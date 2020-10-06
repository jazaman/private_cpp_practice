/*
 * time_wrapper.h
 *
 *  Created on: May 27, 2020
 *      Author: jamil.zaman
 */

#ifndef TIME_WRAPPER_H_
#define TIME_WRAPPER_H_
// put_time example

// put_time example
#include <iostream>     // std::cout
#include <iomanip>      // std::put_time
#include <ctime>        // std::time_t, struct std::tm, std::localtime
#include <chrono>       // std::chrono::system_clock
#include <sys/time.h>
#include <string>

class time_wrapper {
public:
    friend std::ostream &operator<<( std::ostream &output, const time_wrapper &tr ) {
        std::time_t tt {std::time(nullptr)};
        timeval milsec {};

        char foo[60];
        if(0 < strftime(foo, sizeof(foo), "%F_%X", localtime(&tt))) {
            //output << std::put_time(std::localtime(&tt),"[%F_%X] ");
            gettimeofday(&milsec, nullptr);
            output << "[" << std::string(foo) + "." +  std::to_string(milsec.tv_usec/1000) << "]" ;
        }
        return output;
    }

    static time_wrapper& get_timer() {
        static time_wrapper tw;
        return tw;
    }

private:
    //time_wrapper();
    //total_response.length()

    //TODO make all default, copy, assignment private
};



#endif /* TIME_WRAPPER_H_ */

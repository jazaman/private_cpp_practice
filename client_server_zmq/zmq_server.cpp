#include <iostream>
#include <zmq.hpp>
#include <error.h>

#ifndef _WIN32
#include <unistd.h>
#endif

int main()
{
    std::cout << "Hello ZMQ!!" << std::endl;
    //void* hZmq = 0;
    //if (( hZmq = zmq_ctx_new()) == 0 )
    //{
    //    std::cerr << "Failed to create ZMQ context:" << errno << std::endl;
    //}

    zmq::context_t zmq_ctx(1);
    zmq::socket_t socket(zmq_ctx, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while(true)
    {
        zmq::message_t request;
	socket.recv(&request); 
        std::cout << "Received Hello " << std::endl;
        sleep(1);
        zmq::message_t reply(5);
        memcpy((void*) reply.data(), "World", 5);
        socket.send(reply);
        
    }
    





    //if(zmq_ctx_destroy(hZmq)) //no zero return
    //{
    //    std::cerr << "Failed to cleanly terminate ZMQ context:" << errno  << std::endl;
    //}
    
    return 0;
}

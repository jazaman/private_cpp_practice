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
    zmq::socket_t socket(zmq_ctx, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    for(int i = 0; i < 5; i++)
    {
        zmq::message_t reply(5);
        memcpy((void*) reply.data(), "Hello", 5); 
        socket.send(reply);
        zmq::message_t request;
	socket.recv(&request); 
        std::cout << "Received World" << std::endl;
        sleep(1);
        
    }
    





    //if(zmq_ctx_destroy(hZmq)) //no zero return
    //{
    //    std::cerr << "Failed to cleanly terminate ZMQ context:" << errno  << std::endl;
    //}
    
    return 0;
}

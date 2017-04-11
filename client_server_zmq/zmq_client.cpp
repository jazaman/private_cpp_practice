#include <iostream>
#include <zmq.hpp>
#include <error.h>
#include <string>

//local header
#include "fileprocessor.h"
#include "template_utilities.h"

#ifndef _WIN32
#include <unistd.h>
#endif

int main()
{
    std::cout << "Hello ZMQ - CLIENT!!" << std::endl;
    FileProcessor fp;
    //void* hZmq = 0;
    //if (( hZmq = zmq_ctx_new()) == 0 )
    //{
    //    std::cerr << "Failed to create ZMQ context:" << errno << std::endl;
    //}
    std::cout << "Start reading file" << std::endl;

    process_data(&FileProcessor::read_file, fp, "/home/jamil/deployment/member.zip");

    std::cout << "Start reading file" << std::endl;

    std::cout << "Client Application Starting ..." << std::endl;

    zmq::context_t zmq_ctx(1);
    zmq::socket_t socket(zmq_ctx, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");
    std::cout << "Client connected waiting for 2 seconds ..." << std::endl;
    sleep(2);

    for(int i = 0; i < 15; i++)
    {
        zmq::message_t reply(5);
        memcpy((void*) reply.data(), "Hello", 5); 
        socket.send(reply);
        zmq::message_t request;
	    socket.recv(&request);
	    std::string data = (char*)request.data();
        std::cout << "Received "<< data << std::endl;
        sleep(1);
    }
    
    //if(zmq_ctx_destroy(hZmq)) //no zero return
    //{
    //    std::cerr << "Failed to cleanly terminate ZMQ context:" << errno  << std::endl;
    //}
    
    return 0;
}

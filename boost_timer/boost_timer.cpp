/*
 * boost_timer.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: jamil.zaman
 */

//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
//#include <json/reader.h>
//#include <json/writer.h>

/*void print(const boost::system::error_code& e)
{
  std::cout << "Hello, world!" << std::endl;
}*/

class printer
{
public:
  printer(boost::asio::io_context& io, size_t _seconds)
    : timer_(io, boost::posix_time::seconds(1)),
      count_(0),max_(_seconds)
  {
    timer_.async_wait(boost::bind(&printer::print, this));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print()
  {
    if (count_ < max_)
    {
      std::cout << count_ << std::endl;
      ++count_;

      timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
      timer_.async_wait(boost::bind(&printer::print, this));
    }
  }

private:
  boost::asio::deadline_timer timer_;
  int count_;
  int max_;
};

void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << std::endl;
    ++(*count);

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
    t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    std::cout <<"after: "<<*count << std::endl;
  }
}

/*
int main()
{
    boost::asio::io_context io;
    //int count = 0;
    printer(io, 3);
    //boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
    //t.async_wait(&print);
    //t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));

    io.run();

    //std::cout << "Final count is " << count << std::endl;

    return 0;
}
*/

//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//



using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {

      boost::asio::read_until(socket_, rmessage_, '\0');
      std::string message = boost::asio::buffer_cast<const char*>(rmessage_.data());
      std::cout << "client sent ->" << message << std::endl;
      /*boost::asio::async_read(socket_, rmessage_,
                    boost::bind(&tcp_connection::handle_read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    */

            wmessage_ = make_daytime_string();
    boost::asio::async_write(socket_, boost::asio::buffer(wmessage_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

  }

private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
      {
          std::cout << "Handle Read of connection\n";

          if (error && error != boost::asio::error::eof) {
              std::cout << "Error: " << error.message() << "\n";
              return;
          }

          std::string messageP;
          {
              std::stringstream ss;
              ss << &rmessage_;
              ss.flush();
              messageP = ss.str();
          }

          std::cout << messageP << std::endl;

          //Json::Value root      = to_json(messageP);
          //std::string isHello   = root["hello"].asString();
          //std::string isMessage = root["message"].asString();
          //if(mes)
          {
              std::string messageTemp = "{\"Hello\":\"Server\"}";
              boost::asio::async_write(socket_, boost::asio::buffer(messageTemp),
                      boost::bind(&tcp_connection::handle_write, shared_from_this(),
                          boost::asio::placeholders::error,
                          boost::asio::placeholders::bytes_transferred));
          }

          //if(!isMessage.empty())
          {
              std::string messageTemp = "{\"response\":\"Fine\"}";
              boost::asio::async_write(socket_, boost::asio::buffer(messageTemp),
                      boost::bind(&tcp_connection::handle_write, shared_from_this(),
                          boost::asio::placeholders::error,
                          boost::asio::placeholders::bytes_transferred));
          }

          {
              wmessage_ = make_daytime_string();
                  boost::asio::async_write(socket_, boost::asio::buffer(wmessage_),
                      boost::bind(&tcp_connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
          }
      }

  tcp::socket socket_;
  std::string wmessage_;
  boost::asio::streambuf rmessage_;

};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context)
: acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
{
        start_accept();
}

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_executor().context());

        acceptor_.async_accept(new_connection->socket(),
                boost::bind(&tcp_server::handle_accept, this, new_connection,
                        boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
            const boost::system::error_code& error)
    {
        if (!error)
        {
            std::cout << "A Client Connected" << std::endl;
            new_connection->start();
        }

        start_accept();
    }

    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

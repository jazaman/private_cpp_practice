#include <sys/socket.h>
#include <boost/program_options.hpp>
#include <string>
//#include <stdint>

using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {

    uint32_t portnum; 
    po::options_description desc("Allowed Option");
    desc.add_options()
        ("server,s", "server side")
        ("client,c", "client side")
        ("servername", "hostname to connect to")
        ("port,p",   po::value<uint32_t>(&portnum)->default_value(100000), "port number")
        ("help,?", "usage help")
    ;

    po::variables_map vm;
    po::positional_options_description pos;
    pos.add("servername", -1);
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);

    po::notify(vm);

    if(vm.count("help")) {
        std::cout << "Usage:\n";
        std::cout << desc;
    }

    return 0;
}

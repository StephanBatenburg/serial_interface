#include <boost/asio.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#define ERR_LOG(x) fprintf(stderr, x)
enum OS{
    LINUX,
    WINDOWS
};

std::vector<std::string> get_comport_list(OS operating_system){
    std::vector<std::string> comport_list;

    if(operating_system == WINDOWS){
        for(int i = 1; i < 100; i++){
            try{
                boost::asio::io_context io;
                boost::asio::serial_port port(io, "COM" + std::to_string(i));
                comport_list.push_back("COM" + std::to_string(i));
                port.close();
            }catch (boost::system::error_code & ec){
                #ifdef IO_DEBUG
                    ERR_LOG("COMport couldn't open");
                #endif
            }
        }

    }else if(operating_system == LINUX){

    }

    if(comport_list.empty()){
        comport_list.push_back("None");
    }

    return comport_list;
}

int main(){
    fprintf(stdout, "Hello world!\n");
}
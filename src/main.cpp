#include <boost/asio.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#define ERR_LOG(x) fprintf(stderr, "%s\n", x)

std::vector<std::string> get_comport_list(){
    std::vector<std::string> comport_list;

    for(int i = 1; i < 100; i++){
        try{
            boost::asio::io_context io;
            boost::asio::serial_port port(io, "\\\\.\\COM" + std::to_string(i));
            comport_list.push_back("COM" + std::to_string(i));
            port.close();
        }catch (const boost::system::system_error & e){
            #ifdef IO_DEBUG
                ERR_LOG(e);
            #endif
        }
    }

    if(comport_list.empty()){
        comport_list.push_back("None");
    }

    return comport_list;
}

int main(){

    auto comport_list = get_comport_list();
    int selectPort = 0;
    
    auto topBar = ftxui::Container::Horizontal(
            {ftxui::Dropdown(&comport_list, &selectPort) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 40),
             ftxui::Button("Search", [&] {comport_list = get_comport_list();})
            });

    std::vector<std::string> message_history;
    std::string chat_history_string = "";
    auto chat_history_pane = ftxui::paragraph(chat_history_string) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 10);
    
    std::string input = "";
    std::string placeholder = "Enter message...";
    auto bottomBar = ftxui::Container::Horizontal(
            {ftxui::Input(&input, &placeholder),
             ftxui::Button("Send", [&] {/*TODO - add function to send message*/})
            });

    auto topPane = ftxui::Renderer(topBar, [&] {
        return ftxui::vbox({
            topBar->Render() | ftxui::flex,
            chat_history_pane | ftxui::flex,
            bottomBar->Render() | ftxui::flex
        }) | ftxui::border;
    });

    

    auto composition = ftxui::Container::Vertical({topPane});
    auto screen = ftxui::ScreenInteractive::FitComponent();
    screen.Clear();
    screen.Loop(composition);
}
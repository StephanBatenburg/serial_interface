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

void get_comport_messages(std::vector<std::string>* message_history, std::vector<std::string>* comport_list, int selectPort){
    std::vector<char> read_buffer(1024);
    size_t message_size = 0;
    try{
    boost::asio::io_context io_context;
    boost::asio::serial_port port(io_context, comport_list->at(selectPort));
    message_size = port.read_some(boost::asio::buffer(read_buffer));
    port.close();
    }catch (const boost::system::system_error & e){
        #ifdef IO_DEBUG
            ERR_LOG(e);
        #endif
    }
    if(message_size > 0){
        std::string final_message = "";
        for(size_t i = 0; i < message_size; i++){
            final_message += read_buffer[i];
        }
        message_history->push_back(final_message);
    }
}

int main(){

    auto comport_list = get_comport_list();
    int selectPort = 0;
    
    auto topBar = ftxui::Container::Horizontal(
            {ftxui::Dropdown(&comport_list, &selectPort) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 40),
             ftxui::Button("Search", [&] {comport_list = get_comport_list();}) | ftxui::color(ftxui::Color::DarkViolet)
            });

    std::vector<std::string> message_history;
    std::string chat_history_string = "";
    auto chat_history_pane = ftxui::paragraph(chat_history_string) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 10);
    
    std::string input = "";
    std::string placeholder = "Enter message...";
    auto bottomBar = ftxui::Container::Horizontal(
            {ftxui::Input(&input, &placeholder) | ftxui::border,
             ftxui::Button("Send", [&] {
                message_history.push_back(input);
                input = "";}) | ftxui::color(ftxui::Color::DarkViolet)
            });

    auto topPane = ftxui::Container::Vertical({
        topBar,
        ftxui::Renderer([&] (){
            get_comport_messages(&message_history, &comport_list, selectPort);
            chat_history_string = "";
            if(message_history.size() > 10){
                message_history.erase(message_history.begin());
            }
            for(auto chat_message: message_history){
                chat_history_string += chat_message + "\n";
            }
            chat_history_pane = ftxui::paragraph(chat_history_string) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 8) | ftxui::border ;
            return chat_history_pane;
        }),
        bottomBar
    }) | ftxui::border;

    auto composition = ftxui::Container::Vertical({topPane});
    auto screen = ftxui::ScreenInteractive::FitComponent();
    system("cls");
    screen.Loop(composition);
}
#include <memory>
#include <thread>
#include <mutex>


struct UIThreadArgs{
    double start_time;
    double* simulation_time;
    double end_time;
    uint8_t* state_ptr;
    char* pipe_name;
    int _fd;
    bool isService = false;
    std::shared_ptr<std::mutex> ui_mutex;
    std::shared_ptr<std::mutex> state_mutex;
};


class UIHandler{
    
    std::shared_ptr<std::thread> ui_handler_thread;
    UIThreadArgs ui_thread_args;

    bool isService;
public:
    void SetUIThreadArgs(UIThreadArgs args);
    void DeclareService(std::string pipe_name);

    void StartUIHandler();

    void StopUIHandler();

};
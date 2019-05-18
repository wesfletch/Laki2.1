#include <ros/ros.h>
#include <mavros/mavros.h>

#include <iostream>
#include <thread>

class RC_Controller {

    public:

    void exec(){
        
    }
        
};

class Commander {

    public:

    void exec(RC_Controller* rc_cont){

        RC_Controller cmd_rc = *rc_cont;

        std::string cmd;
        // std::vector<std::string> cmd_args;

        while (ros::ok()) {

            std::cout << "(Laki2.1): ";
            getline(std::cin, cmd);

            std::istringstream iss(cmd);
            std::vector<std::string> cmd_args(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());


            if (cmd_args[0] == "RC")
                cmd_rc.exec();

        }
    }

};

int main(int argc, char** argv) {

    ros::init(argc, argv, "sim_shell");

    RC_Controller* rc_cont;
    std::thread rc_thread(&RC_Controller::exec, rc_cont);

    Commander* cmd;
    std::thread cmd_thread(&Commander::exec, cmd, rc_cont);

    rc_thread.join();
    cmd_thread.join();


}
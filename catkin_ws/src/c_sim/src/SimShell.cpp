#include <ros/ros.h>
#include <mavros/mavros.h>
#include <mavros_msgs/RCIn.h>

#include <iostream>
#include <thread>

class RC_Controller {

    public:

        ros::NodeHandle n;
        ros::Publisher rc_pub = n.advertise<mavros_msgs::RCIn>("/mavros/rc/in", 100);
        std::string cmd_msg = "";
        std::vector<uint16_t> channels = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        mavros_msgs::RCIn rc_msg;

        void exec(){
            
            ros::Rate rate(1);

            while (ros::ok()){

                rate.sleep();

                std::cout << cmd_msg;

                if (cmd_msg == "") {
                    std::cout << "no cmd_msg";
                    continue;
                }
                else if(cmd_msg == "takeoff"){
                    std::cout << 'got here';
                    channels[4] = 2067;
                    rc_msg.channels = channels;
                    rc_pub.publish(rc_msg); 
                }
                   
   
            }

        }

        void set_msg(const std::string& arg){ 
            // std::cout << "eat shit";
            cmd_msg = arg;
            std::cout << cmd_msg;
        }

};

class Commander {

    public:

        void exec(RC_Controller* rc_cont){

            RC_Controller cmd_rc = *rc_cont;
            std::string cmd;

            while (ros::ok()) {

                std::cout << "(Laki2.1): ";
                getline(std::cin, cmd);

                if (!cmd.empty()) {
                    std::istringstream iss(cmd);
                    std::vector<std::string> cmd_args(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());

                    if (cmd_args[0] == "RC" && cmd_args.size() > 1)
                        cmd_rc.set_msg(cmd_args[1]);
                }
            
            }
        }

};

int main(int argc, char** argv) {

    ros::init(argc, argv, "sim_shell");

    // calling a member function DOES NOT instantiate the object, you have to actually do it yourself
    RC_Controller* rc_cont = new RC_Controller();
    Commander* commander = new Commander();

    
    std::thread cmd_thread(&Commander::exec, commander, rc_cont);
    std::thread rc_thread(&RC_Controller::exec, rc_cont);
    

    rc_thread.join();
    cmd_thread.join();

}
#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "ros/console.h" //for logs


// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO("INFO: drive_robot(%f, %f)\n", lin_x, ang_z);
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget svc;
    svc.request.linear_x = lin_x;
    svc.request.angular_z = ang_z;
    client.call(svc);
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    float lin_x = 0.0;
    float ang_z = 0.0;
    bool found = false;
    int nchanels = img.step / img.width;
    /*for(int i = 0; i < img.height * img.step; i += nchanels){
        f
    }
    for(int row = 0; row < img.height; ++row){
        for()
    }*/
    for(int i = 0; i < img.height*img.step; i+=nchanels){
        if(img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel){
            ROS_INFO("INFO: i can see ball!\n");
            lin_x = 0.2;
            switch((i%img.width)/(img.width/3)){
            case 0: ang_z = +0.5; break;
            case 2: ang_z = -0.5; break;
            }
            break;
        }
    }

    drive_robot(lin_x, ang_z);
    
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}

# **Rhino DC Motor Position Control with RMCS 2303**
## **Introduction:**
The repository consist of steps and guide for controlling a Rhino DC Motor with RMCS 2303 Driver based on ROS topics. 

&nbsp;
## **Components Needed**:
- Rhino DC Motor - 1
- RMCS 2303 Driver -1
- 12 V DC Power supply
- BreadBoard
- Jumper Wire
- Teensy 4.1 or Arduino Mega

    - Male-Male: 
    - Male-Female:
    - Female-Female:


&nbsp;

## **Pre-Requisite:**
- ROS Noetic
- VS Code with PlatformIO 
&nbsp;
## **Circuit Diagram**
![fig 1](./rmcs_connection.png)

&nbsp;
## **Procedure**:

1. Create New Project ![fig 1](./platformio.png)
&nbsp;
2. Name the project, Select Board teensey 4.1


3. Go to src/main.cpp, paste the following code:
&nbsp;
&nbsp;

4. Go to Include folder and make a file named RMCS2303.h and paste the following code:


&nbsp;

5. Go to src folder and create a file named RMCS2303.cpp and paste the below code:


6. Go to libraries section and search for ros. Download the Rosserial Arduino Library by Michael Ferguson. Select the project name and add project dependency. ![fig 1](./platformio_lib.png)
&nbsp;
7. Compile the Code and upload to teensey.
8. Open Terminal 1, run roscore
9. Open Terminal 2, run rosrun rosserial_python serial_node.py /tty

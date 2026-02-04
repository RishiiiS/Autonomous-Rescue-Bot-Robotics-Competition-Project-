#Autonomous Rescue Bot
A robotics competition project focused on building a four-wheel rescue robot capable of operating in danger-prone environments and retrieving hostages/objects using a gripper mechanism. The system combines ESP32-based motion control with Raspberry Pi camera streaming for real-time visual monitoring.

Project Overview
The Autonomous Rescue Bot is designed to simulate rescue operations in hazardous areas where human access is limited. The robot is remotely operated, features a gripping arm for object retrieval, and provides live video feedback using a Raspberry Pi webcam setup.

This project emphasizes:
Embedded systems integration
Robotics prototyping
Hardware–software coordination
Real-time monitoring

Tech Stack & Components
Hardware
ESP32 (main controller)
Raspberry Pi (camera + video streaming)
DC Motors + Motor Drivers
Gripper Mechanism
Buck Converter (power regulation)
Webcam
Sensors (as required)
Custom chassis (self-designed)
Software
Python (Raspberry Pi)
ESP32 firmware (Arduino framework)
Basic IoT concepts for remote control
Serial/GPIO communication

Key Features
Four-wheel drive robotic platform
Object retrieval using gripper mechanism
Real-time video feed via Raspberry Pi + webcam
Stable power management using buck converter
Fully custom circuit + mechanical design
Embedded control using ESP32

How It Works
ESP32 controls motor movement and gripper operations.
Motor drivers handle wheel actuation.
Buck converter regulates voltage for stable operation.
Raspberry Pi streams live video from webcam.
Operator uses video feedback to remotely navigate and rescue objects.

System Architecture (High Level)
Controller (ESP32)
        |
   Motor Drivers
        |
     DC Motors
        |
     Robot Base

Raspberry Pi → Webcam → Live Video Feed

Learning Outcomes
Embedded systems programming
Robotics hardware integration
Power management in robotic systems
Real-world debugging
Team-based engineering workflow
Applied computer vision concepts

Future Improvements

Autonomous navigation (line following / obstacle avoidance)
Computer vision–based object detection
Wireless control dashboard
Sensor fusion (ultrasonic / IMU)
Full IoT-based control interface

License
This project is for educational and demonstration purposes.

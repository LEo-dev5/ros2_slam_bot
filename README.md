# ROS2 SLAM Bot - Position Estimator with Kalman Filter

[한국어](README_KR.md) | English

## Overview
A ROS2-Based 2D position estimation system implented in C++.  
Simulates noisy sensor data and corrects it in real-time using a Kalman Filter node.  
Results are visualized using Foxglove Studio.

## Tech Stack
- C++ / ROS2 Jazzy
- Eigen3 (Linear Algebra)
- Foxglove Studio (Real-time Visualization)
- Raspberry Pi 5 (Runtime Environment)

## System Architecture
```
[Position_estimator node]
⬇/sensor_data topic (noisy position)
[kalman_filter node]
⬇/estimated_pos topic (corrected position)
[Foxglove Studio]
↪Real-time graph visualization
```

## Key Features
- ROS2 Publisher/Subscriber pipeline between nodes
- Kalman Filter Predict/Update cycle implemented in C++
- Matrix operations using Eigen3
- Real-time monitoring via Foxlove Bridge

## Results
![Kalman Filter Visualization](assets/foxglove_result.png)

- Blue ling : Raw sensor data (noisy)
- Orange line : Kalman Filter estimated position (smoothed)

## How to Run
```bash
# Terminal 1 - Publish sensor data
ros2 run slam_bot position_estimator

# Terminal 2 - Kalman Filter correction
ros2 run slam_bot kalman_filter

# Terminal 3 - Foxglove_bridge
ros2 launch foxglove_bridge foxglove_bridge_launch.xml port:=8765
```

## Environment
- OS : Ubuntu 24.04 LTS (Raspberry Pi 5)
- ROS2 : Jazzy
- Language : C++17
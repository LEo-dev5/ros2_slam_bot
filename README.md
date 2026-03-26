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
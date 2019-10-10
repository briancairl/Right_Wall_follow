# Right_Wall_follow

This repo contains the source for a really rough right wall follow algorithm.

The algorithm can be simulated by using this fork of the stdr_simulator branch:
https://github.com/bgill92/stdr_simulator

1) To get started, clone this repo and the stdr_simulator repo to the src directory of a catkin workspace (make sure ROS is installed. I have used this with ROS Indigo and Kinetic)

2) Build the catkin workspace

3) Have two terminals open, navigate to the root directory of the catkin workspace and in the first terminal, run:

    $ roslaunch stdr_launchers trin_bot_maze1_xacro.launch

   and in the second terminal run:

    $ rosrun right_wall_follow rwf_sonar

Hopefully everything works and happy simulating!

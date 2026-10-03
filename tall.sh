[1mdiff --git a/04_Sim/ghost_sim_examples/launch/start_sim.launch.py b/04_Sim/ghost_sim_examples/launch/start_sim.launch.py[m
[1mindex 61277a1a..269a02dd 100644[m
[1m--- a/04_Sim/ghost_sim_examples/launch/start_sim.launch.py[m
[1m+++ b/04_Sim/ghost_sim_examples/launch/start_sim.launch.py[m
[36m@@ -76,6 +76,18 @@[m [mdef generate_launch_description():[m
         arguments=["-d", rviz_config_path],[m
     )[m
 [m
[32m+[m[41m    [m
[32m+[m[32m    # Foxglove bridge[m
[32m+[m[32m    foxglove_bridge = Node([m
[32m+[m[32m        package="foxglove_bridge",[m
[32m+[m[32m        executable="foxglove_bridge",[m
[32m+[m[32m        name="foxglove_bridge",[m
[32m+[m[32m        output="screen",[m
[32m+[m[32m        parameters=[[m
[32m+[m[32m            {"address": "0.0.0.0", "port": 8765},[m
[32m+[m[32m        ],[m
[32m+[m[32m    )[m
[32m+[m[41m    [m
     return LaunchDescription([m
         [[m
             DeclareLaunchArgument(name="use_joy", default_value="true"),[m
[36m@@ -84,6 +96,7 @@[m [mdef generate_launch_description():[m
             DeclareLaunchArgument("verbose", default_value="true"),[m
             # simulation,[m
             rviz_node,[m
[32m+[m[32m            foxglove_bridge, #tells ROS launch to start as the bridge as another process[m
             OpaqueFunction(function=launch_setup),[m
         ][m
     )[m

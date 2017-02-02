# QoS
Quality of Service for MPEG Packets



INSTALL : Installation instructions can be found in this file.

config_scripts: Configuration Scripts required to configure the nodes.

marking_module: Kernel Module which marks the video packet.

test_video: MPEG Videos used for testing.

## System Test Topology 

![solarized dualmode](https://github.com/kkushagra/QoS/blob/master/topology.png)

The dangling numbered text is IP address assigned to interfaces of nodes, where nodes can have multiple interfaces.

##Package Information
###config_scripts
This folder contains all the scripts required for configuration.
<node>.sh scripts assign the IP address to each node, and install
VLC  package. 
<node_route>.sh  scripts  add  all  the  routes  and  enable forwarding. 

###marking_module
The marking module contains the kernel module used for marking I packets.

###test_video
There  are  two  test  MPEG  videos  that  are  provided  to  test system.

##Steps to stream video using VLC:

###Step 1: Start VLC to recieve stream at Client

````
$ vlc rtp://@:<port_number_video>
````
###Step 2: Start streaming at Server

````
$ vlc -vvv <filename> --sout '#rtp{dst=<destination_node>,port-video=<port>, port-audio=<port>,sout=keep}'
````

Thats it! You should be seeing the video stream on the client.

# QoS
Quality of Service for MPEG Packets



INSTALL : Installation instructions can be found in this file.

config_scripts: Configuration Scripts required to configure the nodes.

marking_module: Kernel Module which marks the video packet.

test_video: MPEG Videos used for testing.

## System Test Topology 

[![solarized dualmode](https://github.com/kkushagra/QoS/blob/master/topology.png)]

The dangling numbered text is IP address assigned to interfaces of nodes, where nodes can have multiple interfaces.

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

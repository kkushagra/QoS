# QoS
##Quality of Service for MPEG Packets

MPEG video frames contain 4 types of pictures, I, P, B and D. Loss of I and P pictures affect all other pictures within the same frame, causing more severe consequences than losing B or D picture packets. However, these priority levels of video picture packets are not known to the network forwarding engines.  If congestion happen at  the  routers  where  there  are  video  packets  being  forwarded,  they  will  not honor the importance of I picture packets and drop them with the same rule as with P and B picture packets.

The aim of this project is to improve video quality by prioritizing I picture
packets.  The importance of I picture packets is made known to edge routers, so the edge routers will drop such packets with lower probability than other video packets when edge routers get congested.  The rationale to enable this  feature  on  edge  routers  but  not  core  routers  is  that  the  core  routers  are busy forwarding packets all the time, so they can not do any extra work other than forwarding.

The overall architecture of the system we plan to work on is shown in the figure below.  The video server generates video streams to the client.  Edge router 1 is responsible for tagging I picture packets at the IP header.  The core routers forward packets.  Since the DSCP bits of I picture packets are marked, the edge router 2 gives them appropriate higher priority when the network is congested. The video client receives the videos and plays them.



## System Test Topology 

![solarized dualmode](https://github.com/kkushagra/QoS/blob/master/topology.png)

The dangling numbered texts are IP address assigned to an interface of a node, where a node can have multiple interfaces.

## Marking I-MPEG packets flow logic

![solarized dualmode](https://github.com/kkushagra/QoS/blob/master/moduleflow.png)

## Testing Scenario
####Configuration:
The marking kernel module is loaded at edge router E1 and congestion is induced using a congestion inducing mechanism(iperf, packet generator etc.).


##Package Information
####config_scripts
This folder contains all the scripts required for configuration.
\<node>.sh scripts assign the IP address to each node, and install
VLC  package. 
\<node_route>.sh  scripts  add  all  the  routes  and  enable forwarding. 

####marking_module
The marking module contains the kernel module used for marking I packets.

####test_video
There  are  two  test  MPEG  videos  that  are  provided  to  test system.

####INSTALL
Details on setting up the system test enviroment.

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

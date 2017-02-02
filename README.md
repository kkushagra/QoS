# QoS
Quality of Service for MPEG Packets

Installation instructions can be found in INSTALL file.

Directory Structure: 
.
├── INSTALL
├── README.md
├── config_scripts
│   ├── client.sh
│   ├── client_route.sh
│   ├── core1.sh
│   ├── core1_route.sh
│   ├── core2.sh
│   ├── core2_route.sh
│   ├── e1.sh
│   ├── e1_route.sh
│   ├── e2.sh
│   ├── e2_route.sh
│   ├── e3.sh
│   ├── e3_route.sh
│   ├── qos.sh
│   ├── s1.sh
│   ├── s1_route.sh
│   ├── s2.sh
│   └── s2_route.sh
├── marking_module
│   ├── Makefile
│   └── packet_classifier.c
├── test_video
│   ├── centaur_1.mpg
│   └── m84_1.mpg
├── topology.png
└── vlc pkg.tar.gz

INSTALL : Installation instructions can be found in this file.

config_scripts: Configuration Scripts required to configure the nodes.

marking_module: Kernel Module which marks the video packet.

test_video: MPEG Videos used for testing.

topology.png : The topology (internetwork of nodes) on which the project is
based on.

Steps to stream video using VLC:

Step 1: Start VLC to recieve stream at Client

On terminal:
    vlc rtp://@:<port_number_video>

Step 2: Start streaming at Server

On terminal:
    vlc -vvv <filename> --sout '#rtp{dst=<destination_node>,
	port-video=<port>, port-audio=<port>, sout=keep}'

Thats it! You should be seeing the video stream on the client.

For further information and unit tests refer the project report.

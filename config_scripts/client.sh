#!/bin/bash
service network-manager stop
sudo sysctl -w net.ipv4.ip_forward=1
sudo ifconfig eno1 192.168.5.2/24
tar -xvf vlc\ pkg.tar.gz
sudo dpkg -i ~/vlc\ pkg/*.deb

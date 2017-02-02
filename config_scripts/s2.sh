#!/bin/bash
service network-manager stop
sudo sysctl -w net.ipv4.ip_forward=1
sudo ifconfig enp4s0 192.168.6.1/24
tar -xvf vlc\ pkg.tar.gz
sudo dpkg -i ~/vlc\ pkg/*.deb

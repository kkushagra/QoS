#!/bin/bash
service network-manager stop
sudo sysctl -w net.ipv4.ip_forward=1
sudo ifconfig eno1 192.168.6.2/24
sudo ifconfig eno2 192.168.7.1/24


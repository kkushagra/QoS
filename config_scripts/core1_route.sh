#!/bin/sh

sudo route add -net 192.168.1.0/24 gw 192.168.2.1
sudo route add -net 192.168.2.0/24 gw 192.168.2.1
sudo route add -net 192.168.6.0/23 gw 192.168.7.1
sudo route add default gw 192.168.3.2

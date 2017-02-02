#!/bin/sh

sudo route add -net 192.168.1.0/24 gw 192.168.1.1
sudo route add default gw 192.168.2.2

#!/bin/sh

sudo route add -net 192.168.4.0/23 gw 192.168.4.2
sudo route add default gw 192.168.3.1

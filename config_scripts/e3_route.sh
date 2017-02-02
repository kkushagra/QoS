#!/bin/sh

sudo route add -net 192.168.5.0/24 gw 192.168.5.2
sudo route add default gw 192.168.4.1

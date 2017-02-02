#!/bin/sh

sudo route add -net 192.168.6.0/24 gw 192.168.6.1
sudo route add default gw 192.168.7.2

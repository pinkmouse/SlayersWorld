#!/bin/sh
echo Launch Script
ps auxw | grep SWServer | grep -v grep > /dev/null

if [ $? != 0 ]
then
    echo Go into loop
    ./SlayersWorld/Server/build/SWServer > /dev/null
fi
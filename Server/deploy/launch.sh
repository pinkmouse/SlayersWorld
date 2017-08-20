#!/bin/sh
echo Launch Script
ps auxw | grep SWServer | grep -v grep > /dev/null

if [ $? != 0 ]
then
    ./SlayersWorld/Server/build/SWServer > /dev/null
fi
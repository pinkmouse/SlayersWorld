#!/bin/sh

ps auxw | grep SWServer | grep -v grep > /dev/null

if [ $? != 0 ]
    then
            ../build/SWServer > /dev/null
    fi
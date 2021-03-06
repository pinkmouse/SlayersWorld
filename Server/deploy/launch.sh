#!/bin/bash

echo "#=============================#"
echo "#   Chargement du restarter   #"
echo "#=============================#"
echo
sleep 1

cd /SlayersWorld/Server/build/

while true
do
    echo "Demarrage du Serveur"
    git pull
    cmake ../
    make
    ./SWServer
    
    echo
    echo "Crash du serveur !"
    killall SWServer
    sleep 5
    date=$(date "+%Y-%m-%d %H:%M:%S")
        echo Crash : $date >> restarter.log

    echo "Redemarage du Serveur"
    echo
done

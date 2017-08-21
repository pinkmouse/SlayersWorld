#!/bin/bash

echo "#=============================#"
echo "#   Chargement du restarter   #"
echo "#=============================#"
echo
sleep 1

cd /SlayersWorld/Server/build/SWServer

while [ 1 == 1 ]
do
    echo "Demarrage du Serveur"
    ./SWServer
    
    echo
    echo "Crash du serveur !"
    killall SWServer
    sleep 2
    date=$(date "+%Y-%m-%d %H:%M:%S")
        echo Crash : $date >> restarter.log

    echo "Redemarage du Serveur"
    echo
done

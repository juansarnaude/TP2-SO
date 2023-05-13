#!/bin/bash

printf "\033[0;36m############################### COMPILING ###############################\033[0m\n"
docker start tpe > /dev/null
docker exec -it tpe make -C /root/Toolchain clean
docker exec -it tpe make -C /root clean
docker exec -it tpe make -C /root/Toolchain all
docker exec -it tpe make -C /root all
docker stop tpe > /dev/null
printf "\033[0;36m############################### END COMPILE ###############################\033[0m\n"

if [ $# -eq 1 ] && [ $1 == "run" ]
then
    cd /home/cane/tpe-castro/TP2-SO
    sudo ./run.sh
elif [ $# -eq 2 ] && [ $1 == "run" ] && [ $2 == "gdb" ]
then
    printf "\033[0;36m############################### START DEBUG ###############################\033[0m\n"
    printf "\033[0;35mAbrí una nueva terminal y corré \033[0;31m$> ./run.sh gdb\n\033[0;35mEn esta terminal corré \033[0;31m$> gdb\033[0m\n"
    docker start tpe > /dev/null
    docker exec -w /root -ti tpe /bin/bash
    docker stop tpe > /dev/null
    printf "\033[0;36m############################### END DEBUG ###############################\033[0m\n"
fi
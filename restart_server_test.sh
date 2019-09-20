#!/bin/bash
# TEST SCRIPT: Continuously restart the mosquitto server every COUNT seconds.
# This tests the robustness of the cloud task to disconnections. Run on the test
# server machine. When the test is terminated, the mosquitto server will still
# be running.

function usage
{
    echo "Usage: $0 [--start-first] [-T <period>]"
    echo "  --start-first   : Normally the server is off for the first period."
    echo "                    Use this flag to turn it on."
    echo "  -T <period>     : Set the on/off period of the server."
    echo "                    Default = 60 seconds."
    exit
}

function delay
{
    count=$1
    for i in `seq 1 $count`
    do
        echo -n "."
        sleep 1
    done
    echo
}

function finish
{
    echo -e "\n#####"
    echo "Quitting now..."
    killall mosquitto 2> /dev/null
    sleep 2
    mosquitto -d -v &
    p=`ps -C mosquitto | cut -f1 -d" "`
    if [ $p ]
    then
        echo "Mosquitto server is still running! [ pid $((p)) ]"
    fi
    exit
}

while [ $# -ge 1 ]
do
    case $1 in
        "--start-first" )
            start_first=1
            ;;

        "-T" )
            shift
            if [ $1 -gt 0 ]
            then
                period=$1
                echo "Using a period of $period seconds"
            else
                usage
            fi
            ;;

        *)
            usage
            ;;
    esac
    shift
done

trap finish EXIT

if [ -z $period ]
then
    period=60
    echo "Using a default period of $period seconds"
fi

echo "#####"

if [ $start_first ]
then
    echo "Start mosquitto server..."
    mosquitto -d -v
    delay $period
fi

while true
do
    echo "Kill mosquitto server..."
    killall mosquitto 2> /dev/null
    delay $period
    echo "Start mosquitto server..."
    mosquitto -d -v
    delay $period
done

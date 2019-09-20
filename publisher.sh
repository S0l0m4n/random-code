#!/bin/sh
# Constantly publish a message with an incrementing count to the server on the
# specified topic and with the given period (in seconds).

host=172.31.9.59 
port=1883
topic=Hub1

period=6

count=0

while true
do
    message=`printf "%08d. PUBLISH" $count`
    mosquitto_pub -h $host -p $port -t $topic -m "$message"
    count=$((count+1))
    sleep $period
done

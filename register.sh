#!/bin/bash
declare -A dev
echo -n 'Admin username: '; read username
echo -n 'Password: '
stty -echo
read password
stty echo
echo ''
echo -ne 'cid: '; read dev[cid]
echo -ne 'ES support: '; read dev[ES]
dev[usb_port]="/dev/ttyACM0"
dev[username]="term${dev[cid]}"
#Todo Generate better password
dev[password]=$(( ($RANDOM*$RANDOM)%1000000 ))
dev[macs]=`ifconfig -a | sed 's/.*HWaddr \([^ ]*\).*/\1/p' -n | tr '\n' ',' | head -c -1`
buff='{'
for key in "${!dev[@]}"; do
    buff="$buff\"$key\":\"${dev[$key]}\","
done
buff="${buff:0:-1}}"
echo -ne "$buff" > device_config.json
server=`cat server_config.json | sed -n "s/\"hs_ip\": \"\([^\"]*\)\".*/\1/p"`
server="$server""register_device"
wget -O .out --post-data "username=$username&password=$password&dev=`cat device_config.json`" ${server} 2> /dev/null
cat .out
echo ''
rm .out

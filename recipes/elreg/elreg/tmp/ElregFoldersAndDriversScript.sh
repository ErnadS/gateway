#!/bin/sh

echo "Starter Elreg script."

echo "create directory"

mkdir /gateway
mkdir /gateway/alarm
mkdir /gateway/dynamic
mkdir /gateway/Config
mkdir /srv/www/images

echo "mount PTS (needs for SSH and Eclipse)"
mkdir /dev/pts
mount /dev/pts

echo "mount Elreg driver"
cd /tmp
mknod /dev/SJA1000driver c 98 0
insmod ./SJA1000driver.ko

echo "mount RTC"
mknod /dev/rtc c 254 0

echo "mount WATCHDOG"
mknod /dev/watchdog c 10 130

echo "mount COM0"
mknod /dev/ttyS1 c 4 65



echo "Elreg script end"


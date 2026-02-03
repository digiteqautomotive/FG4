# Triggers

This example shows how to use the fg4 triggers in the "triggered buffer" mode
on Linux. It continuously reads from the IIO device and prints the timestamp
(in nanoseconds) and the active trigger signal(s) of every trigger event.

## IIO Device Setup

```shell
echo 1 > /sys/bus/iio/devices/iio:device0/scan_elements/in_activity_en
echo 1 > /sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en
cat /sys/bus/iio/devices/trigger0/name > /sys/bus/iio/devices/iio:device0/trigger/current_trigger
echo 1 > /sys/bus/iio/devices/iio\:device0/buffer/enable
```

## Build

```shell
gcc -o iio iio.c
```

## Run

```shell
./iio /dev/iio:device0
```

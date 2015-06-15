# RFCOMM

rfcomm client and server are exemple code based on the one provide in the book [An Introduction to Bluetooth Programming](http://people.csail.mit.edu/albert/bluez-intro/index.html) to communicate between two computer using Bluetooth RFCOMM protocol.

## How to make it works

In order to have those example working, there are a few preparation to do.

Everything has been detailed on my blog : [RaspberryPi in Bluez](http://absimiliard.art122-5.net/post/2015/06/12/Raspberry-in-bluez)

## Build

```gcc rfcomm_server.c -lbluetooth -o server```
```gcc rfcomm_client.c -lbluetooth -o client```



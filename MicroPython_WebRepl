import network, time

ssid = "YOUR_WIFI_NAME"
pw   = "YOUR_WIFI_PASSWORD"

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
if not wlan.isconnected():
    wlan.connect(ssid, pw)
    while not wlan.isconnected():
        time.sleep(0.2)

print("IP:", wlan.ifconfig()[0])


## afer running code run this in shell
## import webrepl_setup
## import machine
## machine.reset()
## ws://<device_ip>:8266/



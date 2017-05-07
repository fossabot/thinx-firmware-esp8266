# thinx-esp8266-firmware

Firmware for automatic device registration and OTA updates.

Provides example implementations in Arduino C, LUA and Micropython.

* This is a work in progress.
* 100% functionality is not guaranteed for all the time.
* Contents of thinx-lib-esp is not working yet.

# Requirements

### Arduino C development

- Arduino IDE or Platform.io
- Arduino libraries: ArduinoJSON, EAVManager, ESP8266httpUpdate (to be replaced)
- Open this folder using Atom with installed Platform.io or thinx-firmware-esp8266/thinx-firmware-esp8266.ino using Arduino IDE.
- Run prerelease.sh to bake your commit ID into the Thinx.h file.

### Micropython/LUA development

- ESPlorer
- ESPTool

# Usage

1. Create account on the [http://rtm.thinx.cloud/](http://rtm.thinx.cloud/) site
2. Create an API Key
3. Clone [vanilla NodeMCU app repository](https://github.com/suculent/thinx-firmware-esp8266) 
4. Run the bash ./prerelease.sh to create Thinx.h file; you can edit this with your custom information but the file will be overwritten when building on the server
5. You can store API Key in Thinx.h file in case your project is not stored in public repository.
6. Build and upload the code to your device.
7. After restart, connect with some device to WiFi AP 'AP-THiNX' with password 'PASSWORD' and enter the API Key
8. Device will connect to WiFi and register itself. Check your thinx.cloud dashboard for new device.

... Then you can theoretically add own git source, add ssh-keys to access those sources if not public, attach the source to device to dashboard and click the last icon in row to build/update the device. But that's not tested at time of updating this readme.


Note: In case you'll build/upload your project (e.g. the library) using thinx.cloud, API key will be injected automatically and you should not need to set it up anymore.

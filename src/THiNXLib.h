#include <Arduino.h>

#define __DEBUG__
#define __DEBUG_JSON__

#include <stdio.h>
#include "ArduinoJson/ArduinoJson.h"

#include <FS.h>
#include <EEPROM.h>
//#include "EAVManager/EAVManager.h"
//#include <EAVManager.h>
#include <WiFiManager.h>

// Using better than Arduino-bundled version of MQTT https://github.com/Imroy/pubsubclient
#include "PubSubClient/PubSubClient.h" // Local checkout
//#include <PubSubClient.h> // Arduino Library

// TODO: Add UDP AT&U= responder like in EAV? Considered unsafe. Device will notify available update and download/install it on its own (possibly throught THiNX Security Gateway (THiNX )
// IN PROGRESS: Add MQTT client (target IP defined using Thinx.h) and forced firmware update responder (will update on force or save in-memory state from new or retained mqtt notification)
// TODO: Add UDP responder AT&U only to update to next available firmware (from save in-memory state)

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define MQTT_BUFFER_SIZE 512

//#define __USE_WIFI_MANAGER__

class THiNX {

  public:

    THiNX();
    THiNX(const char *);

    enum payload_type {
      Unknown = 0,
      UPDATE = 1,		                         // Firmware Update Response Payload
      REGISTRATION = 2,		                   // Registration Response Payload
      NOTIFICATION = 3,                      // Notification/Interaction Response Payload
      Reserved = 255,		                     // Reserved
    };

    // Public API
    void initWithAPIKey(const char *);
    void publish();
    void loop();

    String checkin_body();                  // TODO: Refactor to C-string

#ifdef __USE_WIFI_MANAGER__
    WiFiManager *manager;
    WiFiManagerParameter *api_key_param;
#endif

    // MQTT
    PubSubClient *mqtt_client;

    uint8_t buf[MQTT_BUFFER_SIZE];

    const char* thinx_mqtt_channel();
    const char* thinx_mqtt_status_channel();

    // Import build-time values from thinx.h
    const char* app_version;                  // max 80 bytes
    const char* available_update_url;         // up to 1k
    const char* thinx_cloud_url;              // up to 1k but generally something where FQDN fits
    const char* thinx_commit_id;              // 40 bytes + 1
    const char* thinx_firmware_version_short; // 14 bytes
    const char* thinx_firmware_version;       // max 80 bytes
    const char* thinx_mqtt_url;               // up to 1k but generally something where FQDN fits
    const char* thinx_version_id;             // max 80 bytes (DEPRECATED?)

    bool thinx_auto_update;
    bool thinx_forced_update;

    long thinx_mqtt_port;
    long thinx_api_port;

    // dynamic variables
    char* thinx_alias;
    char* thinx_owner;
    char* thinx_udid;
    char* thinx_api_key;

    bool connected;                         // WiFi connected in station mode

    private:

      // WiFi Manager
      WiFiClient *thx_wifi_client;
      int status;                             // global WiFi status
      bool once;                              // once token for initialization      
      void saveConfigCallback();              // when user sets new API Key in AP mode

      // THiNX API
      char thx_api_key[64];                   // for EAVManager/WiFiManager callback
      char mac_string[16] = {0};
      const char * thinx_mac();

      StaticJsonBuffer<1024> jsonBuffer;
      StaticJsonBuffer<1280> wrapperBuffer;

      // In order of appearance
      bool fsck();                            // check filesystem if using SPIFFS
      void connect();                         // start the connect loop
      void connect_wifi();                    // start connecting
      void checkin();                         // checkin when connected
      void senddata(String);                  // TODO: Refactor to C-string
      void parse(String);                     // TODO: Refactor to C-string
      void update_and_reboot(String);         // TODO: Refactor to C-string

      // MQTT
      bool start_mqtt();                      // connect to broker and subscribe
      bool mqtt_result;                       // success or failure on connection
      String mqtt_payload;                    // mqtt_payload store for parsing
      int last_mqtt_reconnect;                // interval

      // Data Storage
      bool should_save_config;                // after autoconnect, may provide new API Key
      void import_build_time_constants();     // sets variables from thinx.h file
      void save_device_info();                // saves variables to SPIFFS or EEPROM
      bool restore_device_info();             // reads variables from SPIFFS or EEPROM
      String deviceInfo();                    // TODO: Refactor to C-string

      // Updates
      void notify_on_successful_update();     // send a MQTT notification back to Web UI

      // Event Queue / States
      bool checked_in;
      bool mqtt_started;
      bool connection_in_progress;
      bool complete;
      void evt_save_api_key();

      // Local WiFi Impl
      bool wifi_wait_for_connect;
      unsigned long wifi_wait_start;
      unsigned long wifi_wait_timeout;
      int wifi_retry;
      uint8_t wifi_status;
      bool wifi_connection_in_progress;
};

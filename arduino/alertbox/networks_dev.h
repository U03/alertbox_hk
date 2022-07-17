


// MQTT details
//
#define MQTT_KEEPALIVE  240
#define MQTT_PORT       8883

const char* MQTT_SERVER = "192.168.0.179";         // 192.168.0.179 : sony wifi
//const char* MQTT_SERVER = "192.168.1.155";         // 192.168.1.155 : sony eth

#ifdef CARD_TINYGSM
// APN
//
const char apn[] = "iot.1nce.net";
const char gprsUser[] = "";
const char gprsPass[] = "";
#endif

#ifdef CARD_WIFI

WiFiMulti wifiMultiple;
#define WIFIMULTI_APPLIST \
    wifiMultiple.addAP("yurikamome", "2212cledelolo"); \
    wifiMultiple.addAP("Takeshiba",  "2212cledelolo"); \
    wifiMultiple.addAP("402",        "MonMotDePasse1");


#endif

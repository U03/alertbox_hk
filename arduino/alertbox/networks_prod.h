


// MQTT details
//
#define MQTT_KEEPALIVE  240
#define MQTT_PORT       8844

const char* MQTT_SERVER = "151.80.47.195";         // 151.80.47.195: traccarpp.paris

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
    wifiMultiple.addAP("yurikamome_IOT", "2212cledelolo"); \
    wifiMultiple.addAP("Takeshiba",      "2212cledelolo"); \
    wifiMultiple.addAP("402",            "MonMotDePasse1");
#endif

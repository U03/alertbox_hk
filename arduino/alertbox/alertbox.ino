/*
    Si des messages "Discarded unread data to favor a write operation" apparaissent alors:
    Dans le fichier SSLClient.h remplacer:
      unsigned char m_iobuf[2048];
    par 
      unsigned char m_iobuf[BR_SSL_BUFSIZE_BIDI];
    Source: https://github.com/OPEnSLab-OSU/SSLClient/issues/46
*/

#include <SSLClient.h>

// INCLURE ICI LE FICHIER DECRIVANT LE BOITIER
//
#include "box_DCIIT.h"

#include "time.h"

// Gestion du bandeau de LEDS
//
#include <FastLED.h>
CRGB leds[LEDS_NUM_LEDS];

// Gestion des boutons
// PCF_00 à PCF_03 sont des #define donnant l'adresse des composants 
//
#include "PCF8574.h"
PCF8574 PCF[] = {
  PCF8574(PCF_00),
  PCF8574(PCF_01),
  PCF8574(PCF_02),
  PCF8574(PCF_03)
};
uint32_t clavierPrecedent = 0x0000;

// Gestion du clignottement
//
uint32_t blinkCourant = 254;
uint32_t blinkMillis  = 0;


// Client MQTT
//
#include <PubSubClient.h>

// Console série pour le debug
//
#define SerialMon Serial

// Surcouche TLS
//
SSLClientParameters mTLS = SSLClientParameters::fromPEM(client_cert, sizeof client_cert, client_key, sizeof client_key);

    
#ifdef CARD_TINYGSM
TinyGsmClient client(modem);
#endif

#ifdef CARD_WIFI
WiFiClient client;
#endif
    
#ifdef CARD_ETH
EthernetClient client;
#endif



SSLClient clientSSL(client, TAs, (size_t)TAs_NUM, A5);
PubSubClient mqtt(clientSSL);

#define MQTT_ACQ_TOPIC_SIZE (16 + sizeof(ID_BOITIER))   
#define MQTT_BUTTON_TOPIC_SIZE (20 + sizeof(ID_BOITIER))

char *mqtt_topic_push   = "alert/"ID_TOPIC"/push/#";
char *mqtt_topic_ping   = "alert/"ID_BOITIER"/ping";
char *mqtt_topic_pong   = "alert/"ID_BOITIER"/pong";
char *mqtt_topic_acq    = "alert/"ID_BOITIER"/acq";
char *mqtt_topic_err    = "alert/"ID_BOITIER"/err";
char *mqtt_topic_button = "alert/"ID_BOITIER"/button";

// Gestion des reconnexion et du ping
//
uint32_t dernierEssaiConnexion = 0;
uint32_t dernierPing = 0;
uint32_t dernierPong = 0;


void mqttCallback(char* topic, byte* message, unsigned int len) {
  Serial.print("Message recu sur le topic: '");
  Serial.print(topic);
  Serial.print("' Message: '");
  for (int i = 0; i < len; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println("'");

  // Gestion de la reception d'un message PONG
  //
  if (!strcmp(mqtt_topic_pong, topic )&& len == 4) {
    dernierPong = millis();
    return;
  }
  
  // Gestion de la reception d'un message MQTT d'alerte principale
  //
  if (strlen(topic) == strlen(mqtt_topic_push) -2 && !strncmp(mqtt_topic_push, topic, strlen(mqtt_topic_push) - 2) && len == 1) {
    switch (*message) {
      case '1':
        Serial.println("On active le relais");
        digitalWrite(GPIO_LED, HIGH);
        publierMqtt(mqtt_topic_acq, "1");
        break;
      case '0':
        Serial.println("On desactive le relais");
        digitalWrite(GPIO_LED, LOW);
        publierMqtt(mqtt_topic_acq, "0");
        break;
      default:
        publierMqtt(mqtt_topic_err, "4");
    }

    
    return;
  }


   
  // Gestion de la reception d'un message MQTT d'alerte multiple
  //
  char led[3];
  char couleur[4];
  
  if (strlen(topic) >= strlen(mqtt_topic_push) && strlen(topic) <= strlen(mqtt_topic_push) +1 && !strncmp(mqtt_topic_push, topic, strlen(mqtt_topic_push) - 1) && len == 3) {
    Serial.println("Alerte Multiple");
    strcpy(led, topic+strlen(mqtt_topic_push)-1);
    strncpy(couleur, (char *)message, 3);
    couleur[3] = '\0';
    Serial.println(" LED:");
    Serial.println(led);
    Serial.println(" Couleur:");
    Serial.println(couleur);

    ledSetColor(led, couleur);
  }
}

// On positionne la couleur d'une LED à partir de son numéro de la chaîne de caractères représentant la couleur demandée
// La chaine est composée de 3 lettres qui représentent les couleurs RGB et valent "A", "E" ou "C"
// "A": Allumée
// "E": Eteinte
// "C": Clignottante
//
void ledSetColor(char *ledstr, char *couleur) {

  int led;

  // On ajoute 1 car la LED 0 est utilisée pour afficher l'état du boitier (heartbeat)
  //
  led = atoi(ledstr) + 1;

  // On controle que le numéro de la LED ne dépasse pas la fin du ruban
  //
  if (led >= LEDS_NUM_LEDS || led < 0) {
    publierMqtt(mqtt_topic_err, "1");
    return;
  }

  // Controle de la longueur de la chaine de couleur
  //
  if (strlen(couleur) != 3) {
    publierMqtt(mqtt_topic_err, "2");
    return; 
  }
    
  // Controle de la valeur de la couleur
  //
  for (int i = 0; i < 3; i++) {
    if (couleur[i] != 'A' && couleur[i] != 'E' && couleur[i] != 'C') {
      publierMqtt(mqtt_topic_err, "3");
      return; 
    }
  }

  char topicTmp[MQTT_ACQ_TOPIC_SIZE];
  strcpy(topicTmp, mqtt_topic_acq);
  strcat(topicTmp, "/");
  strcat(topicTmp, ledstr);
  publierMqtt(topicTmp, couleur);

    
  switch (couleur[0]) {
    case 'A': leds[led].r = 255; break;
    case 'E': leds[led].r =   0; break;
    case 'C': leds[led].r = blinkCourant; break;
  }
  switch (couleur[1]) {
    case 'A': leds[led].g = 255; break;
    case 'E': leds[led].g =   0; break;
    case 'C': leds[led].g = blinkCourant; break;
  }
  switch (couleur[2]) {
    case 'A': leds[led].b = 255; break;
    case 'E': leds[led].b =   0; break;
    case 'C': leds[led].b = blinkCourant; break;
  }
  FastLED.show();
}


// Lecture du clavier
// On génère un entier sur 32 bits représentant chacun des boutons
// Les entrées avec les touches sont des PULL_UP que le bouton met à la masse
// Du coup on on place les 8  boutons à 1 (0xFF) pour les PCF qui ne sont pas présents
// On inverse le masque à la fin pour que les boutons actifs soient à 1
//
uint32_t lireClavier() {
  uint32_t clavier = 0x0000;
  for (int i = 3; i >= 0; i--) {
    clavier = clavier << 8;
    if (PCF[i].isConnected()) {
      clavier = clavier | PCF[i].read8();
    } else {
      clavier = clavier | 0xFF;
    }
  }
  clavier = ~clavier;
  return(clavier);
}

// Publication d'un message 
//
void publierMqtt(char *topic, char *payload) {
  SerialMon.print("Envoi MQTT  ");
  SerialMon.print(topic);
  SerialMon.print("  ");
  SerialMon.println(payload);
  if (mqtt.publish(topic, payload)) {
    SerialMon.println("Envoi MQTT OK");
  } else {
    SerialMon.println("Envoi MQTT KO");
  }
}

// Envoi d'un message PING au serveur avec le uptime de la carte
//
void envoyerPing() {
  char uptime[16];
  sprintf(uptime, "%d", millis()/1000);
  
  SerialMon.print("Envoi PING ");
  SerialMon.println(uptime);
  if (mqtt.publish(mqtt_topic_ping, uptime)) {
    SerialMon.println("Envoi PING OK");
    dernierPing = millis();
  } else {
    SerialMon.println("Envoi PING KO");
  }
}

// Envoi de l'appui d'un bouton au serveur
// S'il s'agit du bouton de l'alerte principale on passe une chaine vide
// Sinon on passe le numéro du bouton appuyé
//
void envoyerBouton(char *button) {
  char topicTmp[MQTT_BUTTON_TOPIC_SIZE];

  strcpy(topicTmp, mqtt_topic_button);
  if (*button != '\0') {
    strcat(topicTmp, "/");
    strcat(topicTmp, button);
  }
  SerialMon.print("Envoi BUTTON ");
  SerialMon.println(button);
  publierMqtt(topicTmp, "BUTTON");
}
 

boolean mqttConnect() {
  SerialMon.print("Connexion au serveur MQTT ");
  SerialMon.println(MQTT_SERVER);

  // Connection au broker MQTT
  // Pas de user/password on est en authentification mutuelle via TLS
  // l'ID du Boitier sert de ClientID MQTT
  //
  boolean status = mqtt.connect(ID_BOITIER);

  if (status == false) {
    SerialMon.println("   Echec de connexion au serveur MQTT");
    ESP.restart();
    return false;
  }
  
  SerialMon.println("   Connexion au serveur MQTT OK");
  
  SerialMon.print("Souscription a ");
  SerialMon.println(mqtt_topic_push);
  mqtt.subscribe(mqtt_topic_push);
  SerialMon.println("   Souscription OK");
  
  SerialMon.print("Souscription a ");
  SerialMon.println(mqtt_topic_pong);
  mqtt.subscribe(mqtt_topic_pong);
  SerialMon.println("   Souscription OK");

  // On commence par envoyer un message PING au serveur
  //
  envoyerPing();

  return mqtt.connected();
}


void setup() {

  // Console serie
  //
  SerialMon.begin(115200);
  delay(10);

  // LED - Relais d'alerte
  //
  pinMode(GPIO_LED, OUTPUT);
  digitalWrite(GPIO_LED, LOW);

  // BUTTON - Le bouton d'acquittement
  //
  pinMode(GPIO_BUTTON, INPUT_PULLUP);

  // Bandeau de LEDS
  //
  FastLED.addLeds<LEDS_LED_TYPE, LEDS_DATA_PIN, LEDS_COLOR_ORDER>(leds, LEDS_NUM_LEDS);
  FastLED.setBrightness(LEDS_BRIGHTNESS);
 
  FastLED.clear();     // On éteind toutes les LED
  leds[0] = CRGB::Red; // On affiche en rouge le voyant de heartbeat
  FastLED.show();      // On affiche le bandeau

  // On initialise les composants PCF
  //
  for (int i = 0; i <=3; i++) {
    Serial.print("PCF #");
    Serial.print(i);
    
    PCF[i].begin(PCF_SDA, PCF_SCL, 0xFF);
    if (PCF[i].isConnected()) {
      Serial.println(" => présent");
    } else {
      Serial.println(" => absent");
    }
  }


#ifdef CARD_TINYGSM 
  // GPIO correspondant au modem GSM
  //
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Paramétrage du port série du modem
  //
  SerialMon.println("Attendez...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(6000);

  // Initialisation du modem et affichage des informations du modem
  //
  SerialMon.println("Initialisation du modem...");
  modem.restart();

  String modem_info = modem.getModemInfo();
  SerialMon.print("Modem info : ");
  SerialMon.println(modem_info);

  SerialMon.print("Connexion a l'APN ");
  SerialMon.println(apn);
  
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" Echec");
    ESP.restart();
  }  else {
    SerialMon.println(" OK");
  }
  
  if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connecte");
  }
#endif

#ifdef CARD_WIFI
    Serial.println("\nConnexion en cours");
    
    WIFIMULTI_APPLIST;

  int nbTentatives = 0;
  
    while(wifiMultiple.run() != WL_CONNECTED) {
        Serial.println(WiFi.SSID());
        Serial.println(WiFi.status());

        // Si on arribe pas à connecte en 20 essais on force le reboot
        //
        if (nbTentatives++ > 20) {
          ESP.restart();
        }
        delay(500);
    }

    Serial.println("\nConnecté au réseau WIFI");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Adresse MAC: ");
    Serial.println(WiFi.macAddress());
#endif

#ifdef CARD_ETH
  //ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
   Ethernet.init(10)
   Ethernet.begin();
#endif

  clientSSL.setMutualAuthParams(mTLS);
  
  // Paramètres du broker MQTT
  //
  mqtt.setKeepAlive(MQTT_KEEPALIVE);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
}

void loop() {

  uint32_t t = millis();
  uint32_t clavier;
  uint32_t clavierCourant;
  char     strTemp[8];

  
  // Tentative de connexion ou de econnexion
  //
  if (!mqtt.connected()) {
    SerialMon.println("=== MQTT NON CONNECTE ===");

    // Tentative de connexion toutes les 10 secondes
    if (t - dernierEssaiConnexion > 10000L) {
      dernierEssaiConnexion = t;
      if (mqttConnect()) {
        dernierEssaiConnexion = 0;
      }
    }
    delay(100);
    return;
  }

  // Gestion du bouton poussoir
  //
  bool appuiBouton = true;
  for (int i = 0; i < 40; i++) {
    if (digitalRead(GPIO_LED) == HIGH && digitalRead(GPIO_BUTTON) == LOW) {
      delay(7);
    } else {
      appuiBouton = false;
      break;
    }
  }
  if (appuiBouton) {
    SerialMon.println("Appui bouton alors que l'alarme est activee");
    digitalWrite(GPIO_LED, LOW);
    envoyerBouton("");
  }


  // Gestion du clavier
  //
  clavier = lireClavier();
  clavierCourant = clavier;
  
  for (int i = 0; i <= 31; i++) {
    if (clavier & 0x0001 && !(clavierPrecedent & 0x0001)) { 
      Serial.print("Bouton ");
      Serial.println(i);
      sprintf(strTemp, "%d", i);
      envoyerBouton(strTemp);
    }
    clavier = clavier >> 1;
    clavierPrecedent = clavierPrecedent >> 1;
  }
  clavierPrecedent = clavierCourant;

  // Gestion de la présence ou de l'absence de PONG pour afficher le heartbeat
  // Note: Pas besoin de FastLED.show() il sera effectué par la gestion de clignotement au plus tard dans 500ms
  //
  if (millis() > dernierPong + 65000) {
    leds[0].r = blinkCourant;
    leds[0].g = 0;
    leds[0].b = 0;
  } else {
    leds[0].r = 0;
    leds[0].g = blinkCourant;
    leds[0].b = 0;
  }
  
  // Gestion du clignottement
  //
  if (millis() > blinkMillis + 500) {

    // On parcourt toutes les LED et on change le niveau de clignottement de celles qui le doivent
    //
    for (int i = 0; i < LEDS_NUM_LEDS; i++) {
      if (leds[i].r == 1 || leds[i].r == 254) { leds[i].r = 255 - leds[i].r; } 
      if (leds[i].g == 1 || leds[i].g == 254) { leds[i].g = 255 - leds[i].g; } 
      if (leds[i].b == 1 || leds[i].b == 254) { leds[i].b = 255 - leds[i].b; } 
    }
    
    // On change le niveau de clignottement courant
    // Et on positionne le timestamp courant pour le clignottement
    //
    blinkCourant = 255 - blinkCourant;
    blinkMillis = millis();

    FastLED.show();
  }

  mqtt.loop();
    
  // Gestion du ping toutes les minutes
  //
  if (abs(t -dernierPing > 59900L)) {       // abs car millis() boucle tous les 49 jours.
    envoyerPing();
  }

  mqtt.loop();
}

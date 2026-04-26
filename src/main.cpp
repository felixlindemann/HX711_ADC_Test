#include <Arduino.h>
#include <HX711_ADC.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"
#include "secrets.h"

// --- KONFIGURATION ---
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Pins für HX711 (DT, SCK)
const int HX711_dout = 33;
const int HX711_sck = 32;

// --- OBJEKTE ---
HX711_ADC LoadCell(HX711_dout, HX711_sck);
AsyncWebServer server(80);
AsyncEventSource events("/events");

// --- VARIABLEN ---
unsigned long lastUpdate = 0;
const int updateInterval = 100; // Update-Intervall in ms (10Hz)
float currentWeight = 0.0;
float calibrationValue = 696.0; // Diesen Wert nach Kalibrierung anpassen!

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("HX711 Scale Dashboard gestartet");

  // --- HX711 INITIALISIERUNG ---
  LoadCell.begin();
  unsigned long stabilizingTime = 2000; // Zeit zum Stabilisieren (ms)
  boolean _tare = true; // Automatisch tarieren beim Start
  LoadCell.start(stabilizingTime, _tare);
  
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Fehler: HX711 nicht gefunden. Verkabelung prüfen!");
  } else {
    LoadCell.setCalFactor(calibrationValue); 
    Serial.println("HX711 erfolgreich initialisiert");
  }

  // --- WIFI INITIALISIERUNG ---
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WiFi...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi verbunden!");
    Serial.print("IP Adresse: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Verbindung fehlgeschlagen. Starte Access Point...");
    WiFi.softAP("SmartScale-Setup");
    Serial.print("AP IP Adresse: ");
    Serial.println(WiFi.softAPIP());
  }

  // --- WEB SERVER ROUTEN ---
  
  // Hauptseite serving
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Tare Endpoint
  server.on("/tare", HTTP_GET, [](AsyncWebServerRequest *request){
    LoadCell.tareNoDelay();
    request->send(200, "text/plain", "OK");
  });

  // SSE Event Source
  server.addHandler(&events);

  server.begin();
}

void loop() {
  static boolean newDataReady = false;

  // HX711 dauerhaft updaten (Wichtig!)
  if (LoadCell.update()) newDataReady = true;

  // Daten in Intervallen an den Browser senden
  if (newDataReady) {
    if (millis() > lastUpdate + updateInterval) {
      currentWeight = LoadCell.getData();
      
      // Nur senden, wenn Clients verbunden sind
      if (events.count() > 0) {
        events.send(String(currentWeight).c_str(), "weight_update", millis());
      }
      
      newDataReady = false;
      lastUpdate = millis();
    }
  }

  // Serielles Debugging (Optional)
  static unsigned long lastSerial = 0;
  if (millis() - lastSerial > 1000) {
    Serial.print("Gewicht: ");
    Serial.println(currentWeight);
    lastSerial = millis();
  }
}

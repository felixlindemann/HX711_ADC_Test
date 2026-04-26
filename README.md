# HX711 Smart Scale with LOLIN32 LITE

Dieses Projekt ermöglicht es, einen HX711 ADC mit einem ESP32 (LOLIN32 LITE) zu betreiben und die Messwerte in Echtzeit auf einer modernen Website anzuzeigen.

## Features
- **Modernes Dashboard**: Glassmorphism Design mit Dunkelmodus.
- **SSE (Server-Sent Events)**: Live-Updates ohne Neuladen der Seite.
- **Tare-Funktion**: Nullstellen der Waage über die Website.
- **Fallback AP**: Startet einen Access Point, falls kein WLAN gefunden wird.

## Hardware-Setup
- **Board**: LOLIN32 LITE (oder ein anderer ESP32)
- **HX711 DT**: GPIO 33
- **HX711 SCK**: GPIO 32
- **VCC**: 3.3V
- **GND**: GND

## Software-Anforderungen

Folgende Bibliotheken müssen in der Arduino IDE installiert sein:
1. `HX711_ADC` von Olav Kallhovd
2. `ESPAsyncWebServer` von me-no-dev
3. `AsyncTCP` von me-no-dev

## Installation & Kalibrierung
1. Kopiere die Datei `secrets.template.h` zu `secrets.h`.
2. Trage in `secrets.h` deine WLAN-Daten (`SECRET_SSID`, `SECRET_PASS`) ein.
3. Öffne `HX711_ADC_Test.ino` in der Arduino IDE.
4. Lade den Code hoch.
4. **Kalibrierung**:
   - Um den korrekten Gewichtswert zu erhalten, musst du den `calibrationValue` anpassen.
   - Nutze am besten das Beispiel "Calibration.ino" aus der `HX711_ADC` Bibliothek, um deinen Faktor zu ermitteln, und trage ihn im Sketch ein.
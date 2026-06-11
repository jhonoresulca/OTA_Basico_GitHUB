#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define VERSION_URL "https://raw.githubusercontent.com/jhonoresulca/OTA_Basico_GitHUB/master/ota/version.txt.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/jhonoresulca/OTA_Basico_GitHUB/master/ota/firmware.bin"
#define CURRENT_VERSION "1.2"

unsigned long lastCheck = 0;

void checkAndUpdate() {
    Serial.println("[GitHub] Consultando versión...");
    
    HTTPClient http;
    http.begin(VERSION_URL);
    int code = http.GET();
    
    Serial.print("[GitHub] HTTP code: ");
    Serial.println(code);  // ← VER QUÉ DEVUELVE
    
    if (code == 200) {
        String newVersion = http.getString();
        newVersion.trim();
        Serial.print("GitHub versión: ");
        Serial.println(newVersion);
        
        if (newVersion != CURRENT_VERSION) {
            Serial.println("✓ NUEVA VERSIÓN");
        }
    } else {
        Serial.println("[GitHub] Error al conectar");
    }
    
    http.end();
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(2, OUTPUT);
    
    Serial.println("\n=== OTA desde GitHub ===");
    
    WiFi.begin("NAVITRACK -2.4G", "N@V1TR4CK");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    Serial.print("\nIP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // LED
    digitalWrite(2, HIGH);
    Serial.println("LED ON");
    delay(100);
    
    digitalWrite(2, LOW);
    Serial.println("LED OFF");
    delay(100);
    
    // Consultar cada 30 segundos
    if (millis() - lastCheck > 30000) {
        lastCheck = millis();
        checkAndUpdate();
    }
}
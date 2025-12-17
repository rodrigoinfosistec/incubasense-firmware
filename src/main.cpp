#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <time.h>

#include "config/app_config.h"
#include "config/wifi_config.h"
#include "config/seeder_config.h"
#include "core/plano_enum.h"
#include "services/firebase_service.h"
#include "services/dht_service.h"
#include "services/time_service.h"
#include "services/hc_service.h"
#include "services/display_service.h"
#include "services/button_service.h"
#include "services/buzzer_service.h"
#include "services/led_service.h"

FirebaseService firebaseService;
DhtService dht;
HcService hc;
DisplayService oled;
Led ledWifi(LED_WIFI);
Led ledFirebase(LED_FIREBASE);
Led ledAlerta(LED_ALERTA);
Led ledBuzzerAtivo(LED_BUZZER_ATIVO);
ButtonService button;
BuzzerService buzzer;
ButtonService buttonBuzzerAtivo;

// MÁQUINA DE ESTADOS
enum class AppState {
    WIFI,
    FIREBASE,
    ESCOLHA_PLANO,
    OPERANDO
};
AppState currentState = AppState::WIFI;
bool resetStarted = false;

// Controle conexão Wi-Fi
bool wifiStarted   = false;
bool wifiConnected = false;

// Delay proposital antes do Wi-Fi
unsigned long wifiStartAt = 0;
const unsigned long WIFI_DELAY_MS = 3000;

// Controle conexão Firebase
static unsigned long lastFirebaseCheck = 0;
const unsigned long FIREBASE_CHECK_INTERVAL = 5000;
bool firebaseConnected = false;

// Intervalo de coleta de dados
static unsigned long lastExec = 0;
const unsigned long REGISTRO_DELAY = 10000;

// Alertas
bool temperaturaAlerta = false;
bool umidadeAlerta = false;
bool nivelAlerta = false;
String tempMsg = "";
String umidadeMsg = "";
String nivelMsg = "";
bool buzzerAtivo = true;

PlanoDetalhado planoAtual;

void setup() {
    #if SIMULATION
        Serial.begin(115200);
    #endif

    ledWifi.begin();
    ledFirebase.begin();
    ledAlerta.begin();

    ledBuzzerAtivo.begin();

    button.begin(BOTAO_RESET);

    buttonBuzzerAtivo.begin(BOTAO_BUZZER_ATIVO);

    buzzer.begin(BUZZER_ALERTA);

    oled.begin();

    // NTP (continua normal)
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

    // Mensagem inicial
    Serial.println("Wi-Fi conectando...");
    oled.show("Wi-Fi conectando...");

    // LED Wi-Fi começa piscando
    ledWifi.blink(200);

    // Marca o tempo de início
    wifiStartAt = millis();

    // Firebase
    firebaseService.begin();

    // DHT - Temp. e umidade
    dht.begin();

    // HC - Distância (Nível)
    hc.begin();
}

void loop() {
    ledWifi.update();
    ledFirebase.update();
    ledAlerta.update();

    ledBuzzerAtivo.update();

    buzzer.update();

    // Escuta longo clique (>= 5 segundos)
    ButtonEvent ev = button.update();
    if (ev == ButtonEvent::LongPress) {
        resetStarted = true;
        wifiStarted = false;
        wifiConnected = false;
        firebaseConnected = false;

        currentState = AppState::WIFI;

        return;
    }

    // Máquina de estados
    switch (currentState) {
        case AppState::WIFI:
            Serial.println("[STATE] WIFI");

            // Início proposital (3s)
            if (!wifiStarted && millis() - wifiStartAt >= WIFI_DELAY_MS) {
                Serial.println("Iniciando Wi-Fi...");
                oled.show("Iniciando Wi-Fi...");

                WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

                wifiStarted = true;
            }

            // Aguarda conexão
            if (wifiStarted && WiFi.status() == WL_CONNECTED) {
                wifiConnected = true;

                Serial.println("Wi-Fi conectado!");
                oled.show("Wi-Fi:", "Conectado!", WiFi.localIP().toString());

                ledWifi.on();

                // 👉 TRANSIÇÃO DE ESTADO
                lastFirebaseCheck = 0;
                currentState = AppState::FIREBASE;
            }
        break;

        case AppState::FIREBASE:
            Serial.println("[STATE] FIREBASE");

            // evita flood
            if (millis() - lastFirebaseCheck >= FIREBASE_CHECK_INTERVAL) {
                lastFirebaseCheck = millis();

                if (firebaseService.checkStatus()) {
                    firebaseConnected = true;

                    Serial.println("Firebase conectado!");
                    oled.show("Firebase:", "Conectado");
                    ledFirebase.on();

                    if (resetStarted) firebaseService.desativarPlanoAtual();

                    firebaseService.getPlanoAtualComDetalhes(planoAtual);

                    currentState = AppState::ESCOLHA_PLANO;
                } else {
                    Serial.println("Firebase offline...");
                    oled.show("Firebase:", "Offline");
                    ledFirebase.off();
                }
            }
        break;

        case AppState::ESCOLHA_PLANO:
            if (!planoAtual.ativo) {
                Serial.println("[STATE] ESCOLHA_PLANO");

                oled.show("Escolha um plano:", "1x Galinha", "2x Codorna");

                if (ev == ButtonEvent::SingleClick) {
                    firebaseService.setPlanoAtual(PlanoTipo::Galinha);
                    firebaseService.getPlanoAtualComDetalhes(planoAtual);
                    currentState = AppState::OPERANDO;
                }
                else if (ev == ButtonEvent::DoubleClick) {
                    firebaseService.setPlanoAtual(PlanoTipo::Codorna);
                    firebaseService.getPlanoAtualComDetalhes(planoAtual);
                    currentState = AppState::OPERANDO;
                }
            } else {
                currentState = AppState::OPERANDO;
            }
        break;

        case AppState::OPERANDO:
            Serial.println("[STATE] OPERANDO");

            // Dados
            String plano = planoAtual.tipo == PlanoTipo::Galinha ? "Galinha" : "Codorna";
            DhtData leituraDht = dht.read();
            HcData leituraHc = hc.read();
            uint32_t diasPassados = daysSince(planoAtual.timestamp);
            int diaAtual = diasPassados + 1;
            int diaInicioEclosao = planoAtual.diaInicioEclosao;
            String status = diaAtual < planoAtual.diaInicioEclosao ? " (Incubacao)" : " (Eclosao)";
            String ranger_temperatura = diaAtual < planoAtual.diaInicioEclosao ? 
                "(" 
                    + String(planoAtual.tempIncubacaoMinC) 
                    + "/"
                    + String(planoAtual.tempIncubacaoMaxC)
                + ")"
                    : 
                "(" 
                    + String(planoAtual.tempEclosaoMinC) 
                    + "/"
                    + String(planoAtual.tempEclosaoMaxC)
                + ")";
            String ranger_umidade = diaAtual < planoAtual.diaInicioEclosao ? 
                "(" 
                    + String(planoAtual.umidadeIncubacaoMinP) 
                    + "/"
                    + String(planoAtual.umidadeIncubacaoMaxP)
                + ")"
                : 
                "(" 
                    + String(planoAtual.umidadeEclosaoMinP) 
                    + "/"
                    + String(planoAtual.umidadeEclosaoMaxP)
                + ")";
            String ranger_nivel = String(planoAtual.nivelMinimoC) + "/" + String(planoAtual.nivelMaximoC);

            // Alerta Temperatura
            float tempMin = diaAtual < planoAtual.diaInicioEclosao ? planoAtual.tempIncubacaoMinC : planoAtual.tempEclosaoMinC;
            float tempMax = diaAtual < planoAtual.diaInicioEclosao ? planoAtual.tempIncubacaoMaxC : planoAtual.tempEclosaoMaxC;
            if(leituraDht.temperature < tempMin || leituraDht.temperature > tempMax){
                Serial.println("Alerta temperatura");

                // Mensagem
                tempMsg = "T ";

                ledAlerta.on();
                buzzer.on();

                // Liga alerta
                temperaturaAlerta = true;
            } else {
                // Reseta mensagem
                tempMsg = "";

                // Desliga alerta
                temperaturaAlerta = false;
            }

            // Alerta Umidade
            float umidMin = diaAtual < planoAtual.diaInicioEclosao ? planoAtual.umidadeIncubacaoMinP : planoAtual.umidadeEclosaoMinP;
            float umidMax = diaAtual < planoAtual.diaInicioEclosao ? planoAtual.umidadeIncubacaoMaxP : planoAtual.umidadeEclosaoMaxP;
            if(leituraDht.humidity < umidMin || leituraDht.humidity > umidMax){
                Serial.println("Alerta umidade");

                // Mensagem
                umidadeMsg = "U ";

                // Liga alerta
                umidadeAlerta = true;
            } else {
                // Reseta mensagem
                umidadeMsg = "";

                // Desliga alerta
                umidadeAlerta = false;
            }

            // Alerta Nível
            if(leituraHc.distanceCm < planoAtual.nivelMinimoC || leituraHc.distanceCm > planoAtual.nivelMaximoC){
                Serial.println("Alerta nivel");

                // Mensagem
                nivelMsg = "N ";

                // Liga alerta
                nivelAlerta = true;
            } else {
                // Reseta mensagem
                nivelMsg = "";

                // Desliga alerta
                nivelAlerta = false;
            }

            // Botão Buzzer Ativo
            ButtonEvent evButtonBuzzerAtivo = buttonBuzzerAtivo.update();
            if (evButtonBuzzerAtivo == ButtonEvent::SingleClick) {
                buzzerAtivo = !buzzerAtivo;
            }

            // Buzzer ativo?
            buzzerAtivo ? ledBuzzerAtivo.on() : ledBuzzerAtivo.off();

            // Liga/desliga led alerta
            if (temperaturaAlerta || umidadeAlerta || nivelAlerta) {
                ledAlerta.on();
            } else {
                ledAlerta.off();
            }

            // Liga/desliga buzzer alerta
            if (buzzerAtivo && (temperaturaAlerta || umidadeAlerta || nivelAlerta)) {
                buzzer.on();
            } else {
                buzzer.off();
            }

            // Display
            oled.show(
                plano 
                    + " ("
                        + "In." + String(planoAtual.duracaoTotalDias)
                        + "/"
                        + "Ec." + String(planoAtual.diaInicioEclosao)
                    + ")",

                "Inicio: " + planoAtual.timestamp.substring(0, 8),

                "Dia: " + String(diaAtual) + status,

                "T:" + String(leituraDht.temperature) + " " + ranger_temperatura,

                "U:" + String(leituraDht.humidity) + " " + ranger_umidade,

                "N:" + String(leituraHc.distanceCm) + " " + ranger_nivel,

                "Alertas: " + tempMsg + umidadeMsg + nivelMsg
            );

            // Coleta registros e envia para o firebase
            if (wifiConnected && firebaseConnected && millis() - lastExec >= REGISTRO_DELAY) {
                lastExec = millis();

                firebaseService.addRegistro(
                    planoAtual.id, 
                    leituraDht.temperature, 
                    leituraDht.humidity, 
                    leituraHc.distanceCm
                );
            }
        break;
    }
}

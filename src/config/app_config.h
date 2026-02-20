#pragma once

// Ambiente
#define SIMULATION true


// Led verde
#define LED_WIFI 25

// Led azul
#define LED_FIREBASE 26

// Led vermelho
#define LED_ALERTA 32

// Led amarelo
#define LED_BUZZER_ATIVO 19

// Botão Escolha/Reset
#define BOTAO_RESET 33

// Botão Buzzer ativo
#define BOTAO_BUZZER_ATIVO 14

// Buzzer ativo
#define BUZZER_ALERTA 27

// Display Oled
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

// Sensor DHT22 de Temperatura e umidade relativa do ar
// Utiliza apenas SDA
// NC inutilizado
#define DHT_22 15

// Sensor HC-SR04 de distância
// 22 - desenvolvimento
// 11 - físico
#define HC_TRIG 4
#define HC_ECHO 5

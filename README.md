# IncubaSense 🥚🌡️

IncubaSense é um projeto de **chocadeira inteligente baseada em ESP32**, desenvolvido com foco em **arquitetura limpa, lógica não bloqueante e integração em tempo real com Firebase**.

O sistema monitora **temperatura, umidade e nível**, aplica **planos de incubação**, exibe informações em **display OLED** e emite **alertas visuais e sonoros**, com controle total via firmware.

---

## 🚀 Funcionalidades

- 📡 Conexão Wi-Fi com inicialização não bloqueante  
- ☁️ Integração com **Firebase Realtime Database**
- 🌡️ Leitura de **temperatura e umidade (DHT)**
- 📏 Leitura de **nível via sensor ultrassônico (HC-SR04)**
- 🧠 Máquina de estados bem definida:
  - Wi-Fi
  - Firebase
  - Escolha de plano
  - Operação
- 📋 Planos de incubação configuráveis:
  - Galinha
  - Codorna
- 🖥️ Exibição de dados em **OLED SSD1306**
- 🚨 Alertas por:
  - Temperatura fora da faixa
  - Umidade fora da faixa
  - Nível fora da faixa
- 🔊 Buzzer com possibilidade de **ativar/desativar via botão**
- ⏱️ Lógica **100% não bloqueante** (`millis`)
- 🔁 Reset lógico por **pressão longa do botão**

---

## 🧩 Arquitetura do Projeto

O projeto segue uma separação clara de responsabilidades:

- src/

- core/: Enums e estruturas centrais.

- services/: Serviços (Wi-Fi, Firebase, sensores, display, etc.).

- config/: Configurações gerais do sistema.

- main.cpp: Orquestrador e máquina de estados.

- Arquitetura: A main atua apenas como orquestradora, delegando toda a lógica específica para serviços dedicados.


A `main` atua apenas como **orquestradora**, delegando toda a lógica específica para serviços dedicados.

---

## 🛠️ Hardware Utilizado

- ESP32 (DOIT DevKit / similar)
- Sensor DHT (temperatura e umidade)
- Sensor ultrassônico HC-SR04
- Display OLED SSD1306 (I2C)
- Buzzer
- LEDs indicadores
- Botões (reset / controle de buzzer)

---

## 🧪 Simulação

O projeto é compatível com **Wokwi**, utilizando:

- `diagram.json`
- `wokwi.toml`

Isso permite simulação completa diretamente no VS Code.

---

## 🔧 Ambiente de Desenvolvimento

- VS Code
- PlatformIO
- Framework Arduino
- Git / GitHub

---

## 📦 Build e Upload

1. Abra o projeto no VS Code
2. Compile com PlatformIO
3. Faça o upload para o ESP32
4. Configure Wi-Fi e Firebase em `config/`

---

## 🔐 Observação sobre Credenciais

As configurações de Wi-Fi e Firebase ficam centralizadas nos arquivos da pasta `config/`.

⚠️ **Não versionar tokens sensíveis em repositórios públicos.**

---

## 📌 Status do Projeto

✔️ **Baseline estável (v1.0)**  
✔️ Funcional e validado em firmware  
✔️ Pronto para evolução (watchdog, FreeRTOS, app mobile, etc.)

---

## 📄 Licença

Este projeto é de uso educacional e experimental.  
Licença será definida futuramente.

---

## ✍️ Autor

Desenvolvido por **Rodrigo Freitas**  
Projeto técnico e educacional com foco em sistemas embarcados e IoT.

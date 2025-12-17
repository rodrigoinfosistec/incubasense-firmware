// Wi- Fi Aguarda 3 segundos para iniciar conectar Wi-Fi
// if (!wifiStarted && millis() - wifiStartAt >= WIFI_DELAY_MS) {
//     Serial.println("Iniciando Wi-Fi...");
//     oled.show("Iniciando Wi-Fi...");
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     wifiStarted = true;
// }

// Wi-Fi
// if (wifiStarted && !wifiConnected) {
//     if (WiFi.status() == WL_CONNECTED) {
//         wifiConnected = true;

//         Serial.println("Wi-Fi conectado!");
//         oled.show("Wi-Fi:", "Conectado!", WiFi.localIP().toString());

//         ledWifi.on();
//     }
// }

// Firebase
// if (wifiConnected && millis() - lastFirebaseCheck >= FIREBASE_CHECK_INTERVAL) {
//     lastFirebaseCheck = millis();

//     if (firebaseService.checkStatus()) {
//         firebaseConnected = true;
//         Serial.println("Firebase OK");
//         ledFirebase.on();

//         // NTP
//         timeSyncTick();
//     } else {
//         Serial.println("Firebase OFFLINE");
//         ledFirebase.off();
//     }
// }

// Eventos Botão
// ButtonEvent ev = button.update();
// if (ev == ButtonEvent::SingleClick) {
//     Serial.println("Clique simples");
//     buzzer.on();
//     ledAlerta.on();
// }
// else if (ev == ButtonEvent::DoubleClick) {
//     Serial.println("Clique duplo");
//     buzzer.blink(1000);
//     ledAlerta.blink(1000);
// }
// else if (ev == ButtonEvent::LongPress) {
//     Serial.println("Pressão longa");
//     buzzer.off();
//     ledAlerta.off();
// }

if (wifiConnected && firebaseConnected && millis() - lastExec >= 10000) {
    lastExec = millis();

    /**
        * Sobrescreve Plano atual
        * 
        * 1. Seta ativo = true
        * 2. Seta o plano escolhido
        * 3. Define uma nova uuid
        * 4. Registra o epoch do servidor
        */
    // firebaseService.setPlanoAtual(PlanoTipo::Codorna);

    /**
        * Inativa Plano atual
        * 
        * Seta ativo = false
        */
    // firebaseService.desativarPlanoAtual();

    /**
        * Busca Plano atual com detalhes
        */
    // PlanoDetalhado planoAtual;
    // firebaseService.getPlanoAtualComDetalhes(planoAtual);

    /**
        * DHT - Temp. e umidade
        * 
        * @return [humidity, temperature, valid]
        */
    // DhtData leituraDht = dht.read();

    /**
        * HC - Distância (cm) - Nível
        * 
        * @return [distanceCm, valid]
        */
    // HcData leituraHc = hc.read();

    /**
        * Adiciona o registro no Firebase
        * 
        * @var String id
        * @var float temperatura
        * @var float umidade
        * @var float distancia
        * 
        * @return bool true
        */
    // firebaseService.addRegistro(
    //     planoAtual.id, 
    //     leituraDht.temperature, 
    //     leituraDht.humidity, 
    //     leituraHc.distanceCm
    // );
}
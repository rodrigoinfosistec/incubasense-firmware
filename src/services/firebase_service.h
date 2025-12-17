#pragma once

#include <Arduino.h>
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>

#include "../config/ntp_config.h"
#include "../config/firebase_config.h"
#include "../core/plano_enum.h"
#include "../core/plano_detalhado.h"

class FirebaseService {
private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    unsigned long lastSend = 0;

    String generateUUID() {
        const char* hex = "0123456789abcdef";
        String uuid = "";

        for (int i = 0; i < 36; i++) {
            if (i == 8 || i == 13 || i == 18 || i == 23) {
                uuid += '-';
            } else if (i == 14) {
                uuid += '4'; // versão
            } else if (i == 19) {
                uuid += hex[(random(0, 4) + 8)]; // variante
            } else {
                uuid += hex[random(0, 16)];
            }
        }

        return uuid;
    }

public:
    void begin() {
        config.database_url = DATABASE_URL;
        config.signer.tokens.legacy_token = DATABASE_SECRET;

        Firebase.reconnectNetwork(true);
        fbdo.setBSSLBufferSize(4096, 1024);
        Firebase.begin(&config, &auth);

        // inicializa RNG
        randomSeed(esp_random());
    }

    bool checkStatus() {
        return Firebase.get(fbdo, "/status");
    }

    String planoToString(PlanoTipo plano) {
        switch (plano) {
            case PlanoTipo::Galinha:
                return "galinha";
            case PlanoTipo::Codorna:
                return "codorna";
            default:
                return "";
        }
    }

    bool setPlanoAtual(PlanoTipo plano) {
        FirebaseJson json;

        json.set("ativo", true);
        json.set("id", generateUUID());
        json.set("plano", planoToString(plano));
        json.set("timestamp/.sv", "timestamp");

        return Firebase.set(fbdo, "/plano_atual", json);
    }

    bool desativarPlanoAtual() {
        FirebaseJson json;
        json.set("ativo", false);

        return Firebase.updateNode(fbdo, "/plano_atual", json);
    }

    PlanoTipo stringToPlano(const String& value) {
        if (value == "galinha") {
            return PlanoTipo::Galinha;
        }
        if (value == "codorna") {
            return PlanoTipo::Codorna;
        }

        return PlanoTipo::Galinha;
    }

    String formatFirebaseTimestamp(const String& timestampMs) {
        if (timestampMs.length() == 0) {
            return "";
        }

        // CONVERSÃO CORRETA PARA 64 BITS
        int64_t ms = strtoll(timestampMs.c_str(), nullptr, 10);

        time_t seconds = (time_t)(ms / 1000);

        // aplica GMT-3 (Brasil)
        seconds += GMT_OFFSET_SEC;

        struct tm timeinfo;
        gmtime_r(&seconds, &timeinfo);

        char buffer[20]; // dd/mm/yy hh:mm:ss
        strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M:%S", &timeinfo);

        return String(buffer);
    }

    bool getPlanoAtual(
        bool& ativo,
        String& id,
        PlanoTipo& plano,
        String& timestamp
    ) {
        if (!Firebase.get(fbdo, "/plano_atual")) {
            return false;
        }

        FirebaseJson& json = fbdo.jsonObject();
        FirebaseJsonData result;

        json.get(result, "ativo");
        ativo = result.to<bool>();

        json.get(result, "id");
        id = result.to<String>();

        json.get(result, "plano");
        plano = stringToPlano(result.to<String>());

        json.get(result, "timestamp");
        timestamp = formatFirebaseTimestamp(result.to<String>());

        return true;
    }

    bool getPlanoDetalhado(
        PlanoTipo plano,
        int& diaInicioEclosao,
        int& duracaoTotalDias,
        float& tempIncubacaoMinC,
        float& tempIncubacaoMaxC,
        float& tempEclosaoMinC,
        float& tempEclosaoMaxC,
        float& umidadeIncubacaoMinP,
        float& umidadeIncubacaoMaxP,
        float& umidadeEclosaoMinP,
        float& umidadeEclosaoMaxP
    ) {
        String path = "/planos/" + planoToString(plano);

        if (!Firebase.get(fbdo, path)) {
            return false;
        }

        FirebaseJson& json = fbdo.jsonObject();
        FirebaseJsonData r;

        json.get(r, "diaInicioEclosao");        diaInicioEclosao        = r.to<int>();
        json.get(r, "duracaoTotalDias");        duracaoTotalDias        = r.to<int>();
        json.get(r, "tempIncubacaoMinC");       tempIncubacaoMinC       = r.to<float>();
        json.get(r, "tempIncubacaoMaxC");       tempIncubacaoMaxC       = r.to<float>();
        json.get(r, "tempEclosaoMinC");         tempEclosaoMinC         = r.to<float>();
        json.get(r, "tempEclosaoMaxC");         tempEclosaoMaxC         = r.to<float>();
        json.get(r, "umidadeIncubacaoMinP");    umidadeIncubacaoMinP    = r.to<float>();
        json.get(r, "umidadeIncubacaoMaxP");    umidadeIncubacaoMaxP    = r.to<float>();
        json.get(r, "umidadeEclosaoMinP");      umidadeEclosaoMinP      = r.to<float>();
        json.get(r, "umidadeEclosaoMaxP");      umidadeEclosaoMaxP      = r.to<float>();


        return true;
    }

    bool getPlanoAtualComDetalhes(PlanoDetalhado& planoOut) {
        // --- 1. plano_atual ---
        if (!Firebase.get(fbdo, "/plano_atual")) {
            return false;
        }

        FirebaseJson& jsonAtual = fbdo.jsonObject();
        FirebaseJsonData r;

        jsonAtual.get(r, "ativo");
        planoOut.ativo = r.to<bool>();

        jsonAtual.get(r, "id");
        planoOut.id = r.to<String>();

        jsonAtual.get(r, "plano");
        planoOut.tipo = stringToPlano(r.to<String>());

        jsonAtual.get(r, "timestamp");
        planoOut.timestamp = formatFirebaseTimestamp(r.to<String>());

        // --- 2. plano detalhado ---
        String pathPlano = "/planos/" + planoToString(planoOut.tipo);

        if (!Firebase.get(fbdo, pathPlano)) {
            return false;
        }

        FirebaseJson& json = fbdo.jsonObject();

        json.get(r, "diaInicioEclosao");        planoOut.diaInicioEclosao     = r.to<int>();
        json.get(r, "duracaoTotalDias");        planoOut.duracaoTotalDias     = r.to<int>();
        json.get(r, "tempIncubacaoMinC");       planoOut.tempIncubacaoMinC    = r.to<float>();
        json.get(r, "tempIncubacaoMaxC");       planoOut.tempIncubacaoMaxC    = r.to<float>();
        json.get(r, "tempEclosaoMinC");         planoOut.tempEclosaoMinC      = r.to<float>();
        json.get(r, "tempEclosaoMaxC");         planoOut.tempEclosaoMaxC      = r.to<float>();
        json.get(r, "umidadeIncubacaoMinP");    planoOut.umidadeIncubacaoMinP = r.to<float>();
        json.get(r, "umidadeIncubacaoMaxP");    planoOut.umidadeIncubacaoMaxP = r.to<float>();
        json.get(r, "umidadeEclosaoMinP");      planoOut.umidadeEclosaoMinP   = r.to<float>();
        json.get(r, "umidadeEclosaoMaxP");      planoOut.umidadeEclosaoMaxP   = r.to<float>();

        // --- 3. nivel ---
        if (Firebase.get(fbdo, "/nivel")) {
            FirebaseJson& jsonNivel = fbdo.jsonObject();

            jsonNivel.get(r, "minimoC");
            planoOut.nivelMinimoC = r.to<float>();

            jsonNivel.get(r, "maximoC");
            planoOut.nivelMaximoC = r.to<float>();
        }

        return true;
    }

    bool addRegistro(String id, float temperatura, float umidade, float nivel) {
        String path = "/registros/" + id + "/" + generateUUID();
        FirebaseJson json;

        json.set("temperatura", temperatura);
        json.set("umidade", umidade);
        json.set("nivel", nivel);

        json.set("timestamp/.sv", "timestamp");

        return Firebase.set(fbdo, path, json);
    }

};

#pragma once
#include "plano_enum.h"

struct PlanoDetalhado {
    // plano_atual
    bool ativo;
    String id;
    PlanoTipo tipo;
    String timestamp;

    // plano
    int diaInicioEclosao;
    int duracaoTotalDias;

    float tempIncubacaoMinC;
    float tempIncubacaoMaxC;
    float tempEclosaoMinC;
    float tempEclosaoMaxC;

    float umidadeIncubacaoMinP;
    float umidadeIncubacaoMaxP;
    float umidadeEclosaoMinP;
    float umidadeEclosaoMaxP;

    // nivel
    float nivelMinimoC;
    float nivelMaximoC;
};

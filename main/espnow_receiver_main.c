#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include <string.h>

// Estrutura para armazenar os dados recebidos
// typedef struct 
// {
//     uint32_t valor1;
//     uint32_t valor2;
//     uint32_t valor3;
// } Dados;

typedef struct
{
    uint32_t valor1;    /*!< Sinal value */
    uint32_t valor2;    /*!< Baseline value */
    uint32_t valor3;    /*!< Threshold value */
    uint8_t valor4;     /*!< Product type */
} Data_t;

static const char *TAG = "espnow_recv";

// Função de inicialização do WiFi
static void wifi_init() 
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void on_data_received(const uint8_t *mac_addr, const uint8_t *data, int len) 
{
    // Verificar se os dados recebidos têm o tamanho correto
    if (len == sizeof(Data_t)) {
        // Converter os dados recebidos para a estrutura Dados
        Data_t *dados_recebidos = (Data_t *)data;

        // Exibir os dados recebidos
        // ESP_LOGI(TAG, "Dados recebidos de %02x:%02x:%02x:%02x:%02x:%02x",
        //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        ESP_LOGI(TAG, "(%d;%d;%d;%d)",
                 dados_recebidos->valor1, dados_recebidos->valor2, dados_recebidos->valor3,dados_recebidos->valor4);
    } else {
        ESP_LOGW(TAG, "Tamanho de dados incorreto recebido: %d, esperado: %d", len, sizeof(Data_t));
    }
    vTaskDelay(pdMS_TO_TICKS(1/1000)); // Delay 10 milisegundos

}

void app_main() 
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    wifi_init();

    // Configurar ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());

    // Registrar a função de callback para dados recebidos
    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_data_received));
}

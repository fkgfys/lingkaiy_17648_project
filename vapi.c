#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8080
#define MAX_FUEL 100

static const uint8_t ERROR_CODES[] = {
    0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 
    0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 
    0x55,                                          
    0x23                                           
};

typedef struct {
    int16_t oil_temperature;
    uint16_t maf_sensor;
    uint8_t battery_voltage;
    uint16_t tire_pressure;
    uint16_t fuel_level;
    uint8_t fuel_consumption_rate;
    uint32_t error_code;
} VehicleData;

VehicleData generate_vehicle_data() {
    VehicleData data;
    data.oil_temperature = (rand() % 80) + 170;
    data.maf_sensor = rand() % 2048;
    data.battery_voltage = rand() % 13;
    data.tire_pressure = rand() % 2048;
    data.fuel_level = rand() % MAX_FUEL;
    data.fuel_consumption_rate = rand() % 50;
    data.error_code = ERROR_CODES[rand() % 18];
    return data;
}

static enum MHD_Result ahc_echo(void *cls, 
                                struct MHD_Connection *connection,
                                const char *url, 
                                const char *method, 
                                const char *version, 
                                const char *upload_data,
                                size_t *upload_data_size, 
                                void **ptr) {
    VehicleData data = generate_vehicle_data();
    char response[256];
    struct MHD_Response *http_response;

    if (0 != strcmp (method, "GET"))
        return MHD_NO;

    if (strcmp(url, "/data") == 0) {
        snprintf(response, sizeof(response),
                 "{ \"Oil Temperature\": %d, \"MAF Sensor\": %u, \"Battery Voltage\": %u, "
                 "\"Tire Pressure\": %u, \"Fuel Level\": %u, \"Fuel Comsumption Rate\": %u, "
                 "\"Error Code\": \"%X\" }",
                 data.oil_temperature, data.maf_sensor, data.battery_voltage,
                 data.tire_pressure, data.fuel_level, data.fuel_consumption_rate,
                 data.error_code);
        http_response = MHD_create_response_from_buffer(strlen(response), (void *)response, MHD_RESPMEM_MUST_COPY);
        enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, http_response);
        MHD_destroy_response(http_response);
        return ret;
    }
}

int main() {
    srand(time(NULL));
    struct MHD_Daemon *d;

    d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, 
                         PORT, 
                         NULL, NULL, &ahc_echo, NULL, MHD_OPTION_END);
    if (d == NULL) {
        return 1;
    }
    
    printf("Vehicle API running on port %d...\n", PORT);
    while(1) sleep(1);
    return 0;
}
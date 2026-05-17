#include <pgmspace.h>

#define SECRET
#define THINGNAME "ESP32_DHT22"

const char WIFI_SSID[] = "NAMA_WIFI";
const char WIFI_PASSWORD[] = "PASSWORD_WIFI";
const char AWS_IOT_ENDPOINT[] = "xxxxx-ats.iot.region.amazonaws.com";

static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
Tuliskan isi AmazonRootCA1.pem di sini
-----END CERTIFICATE-----
)EOF";

static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
Tuliskan isi xxxxx-certificate.pem.crt di sini
-----END CERTIFICATE-----
)EOF";

static const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
Tuliskan isi xxxxx-private.pem.key di sini
-----END RSA PRIVATE KEY-----
)EOF";

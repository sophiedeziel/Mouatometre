const char* ssid = "";
const char* password = "";

// MQTT Broker settings
const char* mqtt_server = "";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* mqtt_state_topic = "mouatometre/1/state"; // Changer le chiffre si plus d'un capteur sur le même réseau
const char* mqtt_clientid = "mouatometre/1"; // Changer le chiffre si plus d'un capteur sur le même réseau
const char* mqtt_name = "Capteur d'humidité";
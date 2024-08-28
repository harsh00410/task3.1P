#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "Harsh's iPhone";  // Your network SSID (WiFi name)
char pass[] = "098765432";       // Your network password

// Sensor and threshold settings
const int lightSensorPin = A4;   // Analog pin connected to the light sensor
int threshold = 500;             // Threshold value for sunlight detection (adjust as needed)

// IFTTT Webhook details
char serverAddress[] = "maker.ifttt.com";  // The server address for IFTTT Webhooks
String eventName = "Light_trigger";        // Your IFTTT event name

// Your IFTTT Webhook key
const String IFTTT_Key = "ecptXDUSTOgRx9PL5zqy-7cReX1sWuZIckDcqcVgAU-";  // Your unique IFTTT key

// Create WiFi and HTTP client objects
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, 80);

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud rate for debugging
  connectToWiFi();     // Call function to connect to WiFi network
}

void loop() {
  // Read the light sensor value
  int lightLevel = analogRead(lightSensorPin);  
  Serial.print("Light Level: ");
  Serial.println(lightLevel);  // Print the current light level to the Serial Monitor

  // Check if the light level exceeds the threshold
  if (lightLevel > threshold) {  
    triggerIFTTTEvent();  // Trigger the IFTTT event if light is above the threshold
    delay(10000);         // Wait 10 seconds before checking the light level again
  }

  delay(1000);  // Delay for 1 second before the next reading to reduce sensor polling frequency
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  // Attempt to connect to the WiFi network
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");   // Print dots while attempting to connect
    delay(1000);         // Wait 1 second before trying again
  }
  Serial.println("\nConnected to WiFi");   // Print a message when connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the device's IP address on the network
}

void triggerIFTTTEvent() {
  // Construct the URL for the IFTTT Webhook request
  String url = "/trigger/" + eventName + "/with/key/" + IFTTT_Key;

  Serial.println("Sending request to IFTTT...");
  client.get(url);  // Send the HTTP GET request to the IFTTT server

  // Get the HTTP response status code from the server
  int statusCode = client.responseStatusCode();  
  // Get the response body from the server
  String response = client.responseBody();       

  Serial.print("Status code: ");
  Serial.println(statusCode);  // Print the HTTP status code

  // Check if the HTTP request was successful
  if (statusCode == 200) {
    Serial.println("Event triggered successfully!");  // Success message
  } else {
    Serial.println("Failed to trigger event.");       // Failure message
  }

  Serial.print("Response: ");
  Serial.println(response);  // Print the response from the IFTTT server
}

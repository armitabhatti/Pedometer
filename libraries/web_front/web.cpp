#include "web.h"

// create a WiFiServer object from Wifi.h library from esp32 to host the server on port 80
WiFiServer server(80);

//-------------------------------------wifi intialization + server initalization------------------------//
void wifi_init (){
  // wifi credentials
  char ssid[] = "Bell855";
  char pass[] = "D2F9929F";
  //initialize wifi using credentials
  WiFi.begin(ssid, pass);
  //wait until esp32 succesfully connects to wifi
  while (WiFi.status() != WL_CONNECTED){
    Serial.print("waiting.....");
    delay(1000); // 1 second delay
  }
  //confirm succesful connection to wifi once while loop exits
  Serial.println("connected!!!");
  //initialize server
  server.begin();
  //print local IP address for accessing the server in a browserser
  Serial.print ("click here: ");
  Serial.println(WiFi.localIP());
}

double accel_mag (int16_t accelX, int16_t accelY, int16_t accelZ){
    //calculate acceleration magnitude using pythagorean theorem where magnitude is in LSB
    double accelmag = sqrt(sq(accelX) + sq(accelY) + sq(accelZ));
    //now divide magnitude by 8192 LSB/g which is scale factor for +-4g to get acceleration in g's
    accelmag = accelmag/8192.0;
    return accelmag;
}

int detect_steps(int16_t accelX, int16_t accelY, int16_t accelZ){
     //declare static variable step count to avoid reinitialization to 0 each time function is called
    static int step_count = 0;
    //store acceleration magnitude in local variable
    double accelmag = accel_mag(accelX, accelY, accelZ);
    //step detection from trial and error, occurs when accel > 1.05
    if (accelmag > 1.05 ) {
    //if condition is met, increase step count by 1
       step_count ++;
  }
  return step_count;
}


//-------------------------------------------code below fully copied from chat gpt------------------------------------//
void website(int16_t accelX, int16_t accelY, int16_t accelZ) {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/data") >= 0) { // Check if the request is for data
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.print("{\"accelX\":");
      client.print(accelX);
      client.print(",\"accelY\":");
      client.print(accelY);
      client.print(",\"accelZ\":");
      client.print(accelZ);
      client.print(",\"magnitude\":");
      client.print(accel_mag(accelX, accelY, accelZ));
      client.print(",\"steps\":");
      client.print(detect_steps(accelX, accelY, accelZ));
      client.println("}");
    } else { // Serve the main HTML page
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>MPU6050 Sensor Data</title>");
      client.println("<script>");
      client.println("function fetchData() {");
      client.println("  fetch('/data').then(response => response.json()).then(data => {");
      client.println("    document.getElementById('accelX').innerText = data.accelX;");
      client.println("    document.getElementById('accelY').innerText = data.accelY;");
      client.println("    document.getElementById('accelZ').innerText = data.accelZ;");
      client.println("    document.getElementById('magnitude').innerText = data.magnitude;");
      client.println("    document.getElementById('steps').innerText = data.steps;");
      client.println("  });");
      client.println("}");
      client.println("setInterval(fetchData, 5000);"); // Fetch data every 5 seconds
      client.println("</script>");
      client.println("</head>");
      client.println("<body onload='fetchData()'>");
      client.println("<h1>MPU6050 Sensor Data</h1>");
      client.println("<p><strong>Accelerometer Data:</strong></p>");
      client.println("<p>Accel X: <span id='accelX'>0</span> m/s²</p>");
      client.println("<p>Accel Y: <span id='accelY'>0</span> m/s²</p>");
      client.println("<p>Accel Z: <span id='accelZ'>0</span> m/s²</p>");
      client.println("<p>Acceleration Magnitude: <span id='magnitude'>0</span> g</p>");
      client.println("<p>Step Count: <span id='steps'>0</span></p>");
      client.println("</body>");
      client.println("</html>");
    }
    client.stop();
  }
}

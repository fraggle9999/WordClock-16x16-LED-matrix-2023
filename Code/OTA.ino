// ###########################################################################################################################################
// # ESP32 OTA update:
// ###########################################################################################################################################
const char otaserverIndex[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
      <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <body>
    <form method='POST' action='/update' enctype='multipart/form-data'>
      <center><b><h1>WordClock software update</h1></b>
      <h2>Please select the in the Arduino IDE > "Sketch" ><br/>"Export Compiled Binary (Alt+Ctrl+S)"<br/>to generate the required .BIN file.<br/>
      Use the "Update" button 1x to start the update.<br/><br/>WordClock will restart automatically.</h2><br/>
      <input type='file' name='update'>       <input type='submit' value='Update'>
     </center></form></body>
  </html>
 )=====";


const char otaNOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>WordClock software update</h1></b>
      <h2>ERROR: Software update FAILED !!!<br/><br/>WordClock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


const char otaOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>WordClock software update</h1></b>
      <h2>Software update done =)<br/><br/>WordClock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


void setupOTAupate() {
  otaserver.on("/", HTTP_GET, []() {
    otaserver.sendHeader("Connection", "close");
    otaserver.send(200, "text/html", otaserverIndex);
  });

  otaserver.on(
    "/update", HTTP_POST, []() {
      otaserver.sendHeader("Connection", "close");
      if (Update.hasError()) {
        otaserver.send(200, "text/html", otaNOK);
        ShowReset(strip.Color(255, 0, 0));
      } else {
        otaserver.send(200, "text/html", otaOK);
        ShowReset(strip.Color(0, 255, 0));
      }
      delay(3000);
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = otaserver.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update failed unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
  otaserver.begin();
}



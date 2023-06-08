/**********
  Written by Tyler James
**********/

#include "BLEDevice.h"

//BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "ESP32TylerTimCar"

//UUID's of the service, characteristic that we want to read and characteristic that we want to write.
static BLEUUID carServiceUUID("3d3677a0-2811-42e9-9900-63db63112b8d");

//Control Characteritics
static BLEUUID dirCharacteristicUUID("09a9c85c-47b8-4776-a9e8-451adee05cef");

//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;

//Address of the peripheral device. Address will be found during scanning... Hopefully.
static BLEAddress *pServerAddress;

//Characteristic that we want to read and characteristic that we want to write.
static BLERemoteCharacteristic* dirCharacteristic;

//Activate notify
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

//Variables to store temperature and humidity
char* dirR;
char hold;

//Flags to check whether new temperature and humidity readings are available
boolean newDirR = false;

//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient();
 
  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
 
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(carServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(carServiceUUID.toString().c_str());
    return (false);
  }
 
  // Obtain a reference to the characteristics in the service of the remote BLE server.
  dirCharacteristic = pRemoteService->getCharacteristic(dirCharacteristicUUID);

  if (dirCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
 
  //Assign callback functions for the Characteristics
  dirCharacteristic->registerForNotify(dirNotifyCallback);
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
      advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
      doConnect = true; //Set indicator, stating that we are ready to connect
      Serial.println("Device found. Connecting!");
    }
  }
};

//When the BLE Server sends a new temperature reading with the notify property
static void dirNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  dirR = (char*)pData;
  newDirR = true;
}


void BLEInit(){
  //Init BLE device
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(15);
}

// void BLEDrive() {

// }

//Loop function for BLE
void BLEHandle(){
  BLEInit();
  while(1){
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
    // connected we set the connected flag to be true.
    if (doConnect == true) {
      if (connectToServer(*pServerAddress)) {
        Serial.println("We are now connected to the BLE Server.");
        //Activate the Notify property of each Characteristic
        dirCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
        connected = true;
      } else {
        Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
      }
      doConnect = false;
    }
    if (newDirR){
      // hold = *dirR - '0';
      // driveDir((int)hold);
      // Serial.println(hold);
      Serial.println(dirR);
      updateBLEDisplay();
      if (dirR[0] == 119 || dirR[0] == 102){
        driveDir(1);
      }
      else if (dirR[0] == 115 || dirR[0] == 98){
        driveDir(2);
      }
      else if (dirR[0] == 97 || dirR[0] == 108){
        driveDir(3);
      }
      else if (dirR[0] == 100 || dirR[0] == 114){
        driveDir(4);
      }
      else if (dirR[0] == 32){
        driveDir(0);
      }
      newDirR = false;
    }
    potSpeed();
    updateBLEDisplay();
    delay(200);
  }
}

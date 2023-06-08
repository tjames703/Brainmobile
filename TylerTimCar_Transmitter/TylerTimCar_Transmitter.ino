/*********
  Written by Tim Welch
*********/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
String serial;
char serialChar[10];

//BLE server name
#define bleServerName "ESP32TylerTimCar"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "3d3677a0-2811-42e9-9900-63db63112b8d"

BLECharacteristic dirCharacteristics("09a9c85c-47b8-4776-a9e8-451adee05cef", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor dirDescriptor(BLEUUID((uint16_t)0x2902));


bool deviceConnected = false;

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  // Start serial communication 
  Serial.begin(9600);  
  lcd.init();
  lcd.backlight();

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics and Create a BLE Descriptor
  // Descriptor bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml       

  bmeService->addCharacteristic(&dirCharacteristics);
  dirDescriptor.setValue("Car direction");
  dirCharacteristics.addDescriptor(new BLE2902());
 
  // Start the service
  bmeService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

void loop() {

  
  if(Serial.available())
  {
    serial = String((char)Serial.read());
    lcd.clear();
    lcd.setCursor(0,0);
    serial.toCharArray(serialChar, 10);
    lcd.print("Char Rec: "+String(serialChar));
    
  } 

  if (deviceConnected) 
  {

    //Set temperature Characteristic value and notify connected client
    dirCharacteristics.setValue(serialChar);
    dirCharacteristics.notify();
    lcd.setCursor(0,1);
    lcd.print("Connected!");
    
  }
  else if (!deviceConnected)
  {
    lcd.setCursor(0,1);
    lcd.print("Not Cnnctd");
  }
  delay(1000);
}
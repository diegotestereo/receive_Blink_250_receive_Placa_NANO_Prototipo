
#include <mcp_can_dfs.h>

#include <SPI.h>
#include <mcp_can.h>
#include <EEPROM.h>

const int SPI_CS_PIN = 10;
const int interrupcion = 9;
const int LED=2;
const int Relay_1=A5;
const int Relay_2=A4;
const int ACS712_1=0;
const int ACS712_2=0;
boolean ledON=1;
//const int canId=127;
 unsigned char canId;
 unsigned char ID_Hex;

unsigned char MsgUpOk[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char MsgUpEEprom[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char MsgLeido[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char  reg_control=0x00;
unsigned char  reg_config=0x00;
unsigned char  reg_5=0x00;
unsigned char  reg_4=0x00;
unsigned char  reg_3=0x00;
unsigned char  reg_2=0x00;
unsigned char  reg_1=0x00;
unsigned char  reg_0=0x00;
 
 MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
 
 void setup()
{
    Serial.begin(9600);
    pinMode(LED,OUTPUT);
    
    pinMode(Relay_1,OUTPUT);
    digitalWrite(Relay_1,HIGH);
    pinMode(Relay_2,OUTPUT);
    digitalWrite(Relay_1,HIGH);
    pinMode(interrupcion ,INPUT);
    ID_Hex= EEPROM.read(0x00);
    
 START_INIT:

    if(CAN_OK == CAN.begin(CAN_250KBPS,MCP_16MHz))                 
    {      
       CAN.sendMsgBuf(ID_Hex,0,8,MsgUpOk);
           digitalWrite(LED,true);
           delay(200);
           digitalWrite(LED,false);
           delay(200);
           digitalWrite(LED,true);
           delay(200);
           digitalWrite(LED,false);
             delay(1000);
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
           digitalWrite(LED,true);
           delay(100);
           digitalWrite(LED,false);
           delay(200);
           digitalWrite(LED,true);
           delay(100);
           digitalWrite(LED,false);
           delay(200);
           digitalWrite(LED,true);
           delay(800);
            digitalWrite(LED,false);
           delay(200);
      
        goto START_INIT;
    }
}

unsigned char stmpOK[8] = {0,0,0,0,0,0,0,0};
unsigned char stmpON[8] = {0,0,1,0,0,0,0,0};
unsigned char stmpOFF[8] = {0,0,0,0,0,0,0,0};

void loop()
{
// Enviar();
  Recibir();
    }



void Recibir(){
  
  
     unsigned char len = 0;
    unsigned char buf[8];

    if(!digitalRead(interrupcion))            // check if data coming
    
    {   
      
         digitalWrite(LED,HIGH);
      delay(20);
      digitalWrite(LED,LOW);
       
      
      CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
         canId = CAN.getCanId();
         ID_Hex= EEPROM.read(0x00);
         int ID=int(ID_Hex);
         
      
           MsgLeido[7]=buf[7];
           MsgLeido[6]=buf[6];
           MsgLeido[5]=buf[5];
           MsgLeido[4]=buf[4];
           MsgLeido[3]=buf[3];
           MsgLeido[2]=buf[2];
           MsgLeido[1]=buf[1];
           MsgLeido[0]=buf[0];

//////////////////////////////////7



   if(  MsgLeido[2]==0x01){
        
          digitalWrite(LED,true);
          digitalWrite(Relay_1,false);
        
       
        
        }else{
        
          digitalWrite(Relay_1,true);
           digitalWrite(LED,false);
          
          }

//////////////////////////////////






           if( canId ==255){
         CAN.sendMsgBuf(ID_Hex,0,8,MsgUpOk);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
          delay(200);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
            delay(500);
             digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
          delay(200);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
        }
     
           
       if(ID==canId){
      
           
    
      
        if(  MsgLeido[1]==0x01){
        
          digitalWrite(LED,true);
          digitalWrite(Relay_1,LOW);
          delay(1000);
          digitalWrite(Relay_1,HIGH);
          digitalWrite(LED,false);
       
            MsgUpEEprom[1]=ID_Hex;
           CAN.sendMsgBuf(canId,0,8,MsgUpEEprom);
        }
         if(MsgLeido[1]==0x02){
         digitalWrite(Relay_2,LOW);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
          delay(500);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
           digitalWrite(Relay_2,HIGH);
        }
        
         if(MsgLeido[1]==0x03){
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
          delay(200);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
          delay(200);
          digitalWrite(LED,true);
          delay(200);
          digitalWrite(LED,false);
      
        }
          if(0xFF==MsgLeido[0]){
            
            EEPROM.write(0x00, MsgLeido[7]);// escribe en la dir 0x00 el id del dispositivo
            CAN.sendMsgBuf(canId,0,8,MsgLeido);
           
        }
       }
      }
        
  
  
  
  }





void Enviar(){
  
    CAN.sendMsgBuf(0x80,0, 8, stmpON);
      digitalWrite(LED,HIGH);
    delay(4000);                       // send data per 100ms
  //   CAN.sendMsgBuf(0x80,0, 8, stmpOFF);
                          // send data per 100ms
     digitalWrite(LED,LOW);
      delay(4000);
  
  
  }
    


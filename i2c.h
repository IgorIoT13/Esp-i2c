#ifndef CONTROLE_I2C
#define CONTROLE_I2C
#include "Arduino.h"

class Controle_i2c{
  private:
    uint8_t sda;
    uint8_t scl;
    bool flag_transmit;
    bool flag_read;
    uint8_t addres;
  
  public:

    Controle_i2c(uint8_t sda, uint8_t scl, uint8_t addres){
      this->sda = sda;
      this->scl = scl;
      if(addres <= 127){
        this->addres = addres;
      }else{
        this->addres = 127;
      }
    }


    void begin(uint8_t sda = 256, uint8_t scl = 256){
      
      if(sda == 256 && this->sda){
        pinMode(this->sda, OUTPUT_OPEN_DRAIN);
      }else{
        this->sda = sda;
        pinMode(this->sda, OUTPUT_OPEN_DRAIN);
      }

      if(scl == 256 && this->scl){
        pinMode(this->scl, OUTPUT_OPEN_DRAIN);
      }else{
        this->scl = scl;
        pinMode(this->scl, OUTPUT_OPEN_DRAIN);
      }

      digitalWrite(this->sda, HIGH);
      digitalWrite(this->scl, HIGH);
    }


    void start_transmit(){
        digitalWrite(this->sda, HIGH);
        digitalWrite(this->scl, HIGH);
        delayMicroseconds(5);
        digitalWrite(this->sda, LOW);
        delayMicroseconds(5);
        digitalWrite(this->scl, LOW);
    }


    void end_transmit(){
        digitalWrite(this->sda, LOW);
        delayMicroseconds(5);
        digitalWrite(this->scl, HIGH);
        delayMicroseconds(5);
        digitalWrite(this->sda, HIGH);
    }

    bool write_data(uint8_t data){
        start_transmit();
        for (uint8_t i = 0; i < 8; i++)
        {
            digitalWrite(this->sda, (data&0x80));
            delayMicroseconds(5);
            digitalWrite(this->scl, HIGH);
            delayMicroseconds(5);
            digitalWrite(this->scl, LOW);
            data = data << 1;
        }
        

        pinMode(this->sda, INPUT);          
        digitalWrite(this->scl, HIGH);  

        bool ack = !digitalRead(this->sda); 

        digitalWrite(this->scl, LOW);
        pinMode(this->sda, OUTPUT_OPEN_DRAIN);

        this->end_transmit();         
        return ack;
    }

    void sent_ack(bool ack){
        pinMode(this->sda, OUTPUT);
        digitalWrite(this->sda, ack ? LOW : HIGH); 
        digitalWrite(this->scl, HIGH);
        delayMicroseconds(5);
        digitalWrite(this->scl, LOW);
        digitalWrite(this->sda, HIGH);
    }


    uint8_t I2C_readByte() {
        uint8_t data = 0;

        pinMode(this->sda, INPUT);  

        for (uint8_t i = 0; i < 8; i++) {
            digitalWrite(this->scl, HIGH);  
            delayMicroseconds(5); 
            data = (data << 1) | digitalRead(this->sda); 
            digitalWrite(this->scl, LOW);   
            delayMicroseconds(5);
        }

        sent_ack(data ? false : true);

        return data;
    }






};


#endif
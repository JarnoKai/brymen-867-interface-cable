/*
 * config.h
 *
 * Created: 10.10.2019 22:48:01
 *  Author: marti
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_
#define pinIR 2
#define pinLED 3
#define pinTransistor 4
#define pinRX 0
#define pinTX 1

#define F_CPU				8000000UL

#define UART_BAUD			9600
#define TX_HIGH				digitalWrite(pinTX,HIGH);
#define TX_LOW				digitalWrite(pinTX,LOW);
#define RX_LOW				(!digitalRead(pinRX))
#define UART_DELAY			1000000UL/UART_BAUD

#define IRLED_HIGH	digitalWrite(pinIR,HIGH);
#define IRLED_LOW	digitalWrite(pinIR,LOW);
#define IRTRAN_LOW	(!digitalRead(pinTransistor))
#define CLOCK_DELAY	100

#define LED_HIGH	digitalWrite(pinLED,HIGH);
#define LED_LOW		digitalWrite(pinLED,LOW);

//modes
#define MODE_5X_PER_SEC		0
#define MODE_1X_PER_SEC		1
#define MODE_STOP			2
#define MODE_SEND			3

#define MODE_DEFAULT		MODE_1X_PER_SEC;

#endif /* CONFIG_H_ */
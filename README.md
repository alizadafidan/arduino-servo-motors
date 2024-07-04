## Greenhouse Monitoring System

This project is an Arduino-based Greenhouse Monitoring System that reads temperature, humidity, and light intensity from various sensors and allows control of servo motors and LEDs through a web interface.

## Components Used

1. **Arduino Board**
2. **Ethernet Shield**
3. **DHT11/DHT22 Sensor**
4. **DS18B20 Temperature Sensors**
5. **Servo Motors (3)**
6. **LEDs**
7. **Light Sensor**
8. **Resistors and Wires**

## Libraries Required

- `SPI.h`
- `Ethernet.h`
- `DHT.h`
- `Servo.h`
- `microDS18B20.h`

## Pin Configuration

- **DHT Sensor**: Digital pin 2
- **DS18B20 Sensors**: Digital pin 3
- **Servo Motors**: Digital pins 5, 6, and 9
- **LEDs**: Digital pins 7 and 8
- **Light Sensor**: Analog pin A1

## Notes

- Ensure that your Arduino board is properly connected to the sensors and other components.
- Adjust the IP address and MAC address if needed to match your network configuration.

## Future Enhancements

- Add more sensors for comprehensive environmental monitoring.
- Implement a database to store historical sensor data.
- Integrate with IoT platforms for remote monitoring and control.

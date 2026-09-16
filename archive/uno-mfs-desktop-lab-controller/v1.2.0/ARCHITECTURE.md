# v1.2 Architecture

```text
                    PC / phone / tablet
                    Python / browser / logger
                           |
                    USB UART or HM-10
                           |
                 ASCII protocol v2 / node ID
                           |
       +-------------------------------------------+
       |        UNO / ATmega328P / MFS            |
       | cooperative scheduler + watchdog         |
       | front panel and host share state         |
       +----------+-----------+----------+---------+
                  |           |          |
       +----------+--+     +--+-----+ +--+--------+
       | DS18B20 TEMP |     | HX711 | | BH1750   |
       +------+-------+     | SCALE | | LIGHT    |
              |             +--------+ +-----------+
              | temperature compensation
              v
       +-------------+
       | HC-SR04 DIST|
       +-------------+
```

The core design remains cooperative rather than RTOS-based. Every normal service routine is bounded/nonblocking; only explicit tare/calibration transactions wait for a finite series of HX711 samples, while continuing display service and watchdog feeding.

The key v1.2 shift is that sensors no longer exist only as four isolated modes: valid air temperature can improve the physical model used by ultrasonic ranging.

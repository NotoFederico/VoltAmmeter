# Medidor de tensión y corriente de precisión para fuente de alimentación implementado con placa de desarrollo BLUEPILL (STM32F103)

Sistema operativo: Windows 10 /
IDE: Keil /
Lenguajes: C para sistemas embebidos

Utiliza un módulo ADS1115 (ADC 16 bits) para la lectura diferencial de corriente y la lectura de tensión de una fuente de alimentación regulable de 0V a 60V y de 0A a 5A.
A través de una comunicación I2C los datos son procesados por el microprocesador a través de un simple algoritmo de promedio de muestras.

Está en curso una segunda versión con tecnologia SMD y se está estudiando implementar un algoritmo que obtenga el promedio de manera mas eficiente

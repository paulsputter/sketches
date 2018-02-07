For information on installing libraries, see: http://arduino.cc/en/Guide/Libraries


Notes from me (1/18/2018) I unzipped the library Arducam_1602_12C from

http://www.arducam.com/downloads/Amazon/C0048_1602.zip 

Arducam_1602_I2C is the library mentioned in Amazon, where I bought the display which came
with the "IIC/I2C Adapter Module"

But LiquidCrystal_I2C also works with this hardware! See the sketch "HelloWorld". I don't
remember where I got this library. I thought I had a software problem when my first
two displays both failed at the same time. Evidently not.

The wiring for the 1602 16x2 display, both using and not using the I2C adapter
is shown in the accompanying .pdf.
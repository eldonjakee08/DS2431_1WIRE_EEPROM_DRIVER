# DS2431_Driver_STM32F4
This is a 1-Wire protocol driver for DS2431 chip, it is a 1024-Bit 1-Wire EEPROM, which are commonly used as ID chips on laptop power supplies. 
I made this driver to read and program the memory of DS2431 to function as an ID Chip that will interface with a laptop power supply. 

The programmed chip will then be used for my "Dell Laptop Power Supply Spoofer" project which enables me to power my laptop with any kind of power supplies as long as it outputs 20V 65W. 
Which in my case, I'll be using a type-C power bank that outputs USB PD 20V 65W. So it kinda works like a Type-C to 4.5mm DC Jack Adaptor (my laptop plug for power). 

You might be wondering, why don't I just connect a 20V supply straight to a 4.5mm DC jack and feed it into my laptop? Well dell laptops has these ID chips (see schematic below) built-in their supplies which tells the laptop the voltage, wattage & serial number of the power supply. Without the ID chip the laptop cannot identify what voltage and wattage the power supply operates at, because of this the laptop will limit the amount of power being fed into it (for safety purposes) resulting in poor performance (underclocked). So even if you plug a 20V DC source it will limit the amount of current being drawn from the supply even if its capable of outputting 65W. In my experience the laptop limits the current to 800mA without the ID chip. 

The project in a nutshell is to fool the laptop to think that it's being powered by an original dell laptop supply.

![image](https://github.com/user-attachments/assets/64c0a8c2-bde8-43bd-9e35-ec83ddc094d3)


You might see from the schematic above that the original supply uses DS2501 ID chip. My problem with this chip is that it's obsolete, is only one time programmable and needs a 12V pulse to be programmed, which I'd need to design a circuit that delivers 12V pulse into the IC which is quite inconvenient. So instead I opted to use the DS2431 chip because its more simple to operate/program (does not use a 12V pulse), not obsolete and the memory could be programmed multiple times.    


PARTS NEEDED
1. USB TYPE-C PD SPOOFER - to tell the PD 20V capable power bank or supply to output 20V
2. DS2431 IC - to spoof the laptop into thinking that its a "dell laptop supply" 
3. PCB
4. Your brain to think and debug the project :D 


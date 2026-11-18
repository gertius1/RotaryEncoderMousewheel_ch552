# RotaryEncoderMousewheel_ch552
This project describes converting a ch552 based standard USB volume control to a mousewheel control. 

<p align="center"><img width="500" src="https://m.media-amazon.com/images/W/BW_MEDIAX_AVIF_MEASUREMENT_1306696-T4/images/I/618LQ0KQ7FL._AC_SL1500_.jpg" alt="USB Volume Knob" /></p>

The idea behind this is to use it with the Reaper DAW for universal parameter control. 
Reaper already allows controlling it´s parameters and plugin parameters per mousewheel when hovering the mousecursor over it (needs to be enabled in preferences).
This project gives you an endless rotary encoder as a mousewheel alternative, for the knob feel.
Pushing the encoder button equals holding "left ctrl" for fine-grain control in Reaper.
No controller needs to be configured in Reaper, it´s plug and play.

It uses a cheap USB volume control (see Amazon link below), which has a programmable ch552 microcontroller.
The original encoder is rastered. 
If you´re looking for an unrastered encoder replacement to solder in, you could use Bourns PEC11R-4015F-S0024

The LED is deactivated here, but already prepared in code if you want to add your own LED behaviour.
If you want to change the .ino sourcecode yourself, this can be done via Arduino IDE and the ch55xduino library, see the instructions on hackaday below.

Instructions:

- DISCLAIMER: after flashing the board yourself, the original firmware is lost and cannot be restored other than reprogrammed
- download WCHISPTool from https://www.wch.cn/products/CH552.html
- set it to Chip Serie '8-Bit CH55x series', Chip Model 'CH552'. Dnld Port 'USB'
- load sketch_nov17a.ino.hex under Object File1
- open casing by removing 4 screws, take out PCB
- while unplugged, carefully connect the two pads on P6 with a wire (this will enable bootloader) and plug in USB
- the device should show in WCHISPTool Dev List (if not, try reconnecting a few times, it sometimes fails)
- when it shows up, press "Download" in WCHISPTool, this flashes the hex file onto the board
- if all went well, the rotary turn now controls mousewheel and rotary push is "left ctrl"


Sources:

https://rabid-inventor.blogspot.com/2020/05/getting-started-with-ch55x.html

https://hackaday.io/project/172143/instructions

https://www.amazon.de/USB-Lautst%C3%A4rkeregler-Multimedia-Typ-C-Controller-Medienanpassung-7-Farben-RGB-Licht-Tastenkombinationen/dp/B0DQBYZVZH/ref=dp_prsubs_d_sccl_3/258-9941439-7408910?pd_rd_w=1n1LF&content-id=amzn1.sym.f16eb515-347d-4f51-8281-c041e0040816&pf_rd_p=f16eb515-347d-4f51-8281-c041e0040816&pf_rd_r=DSEAGCEM69QQG6XYZ351&pd_rd_wg=t5JXW&pd_rd_r=54f8d289-5d0e-4a47-b37d-db57e750ca91&pd_rd_i=B0DQBYZVZH&psc=1


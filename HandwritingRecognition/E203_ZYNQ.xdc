# XDC for XiaoXiongmao FPGA
# zhangrg   2023/6/11

#=========================== Sys Clock & rstn ================================
## Clock Signal     50M
set_property -dict { PACKAGE_PIN K17    IOSTANDARD LVCMOS33 } [get_ports { clk }];
create_clock -add -name sys_clk_pin -period 20.00 -waveform {0 10} [get_ports clk]

## mcu Rst_n  M20
set_property -dict { PACKAGE_PIN W13 IOSTANDARD LVCMOS33 } [get_ports { mcu_rst }]; 
#----------------------------------------------------------------------------




#=========================== JTAG Define ======================================
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets dut_io_pads_jtag_TCK_i_ival]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets IOBUF_jtag_TCK/O]

set_property -dict { PACKAGE_PIN V20  IOSTANDARD LVCMOS33 } [get_ports { mcu_TCK }]; #V20
set_property -dict { PACKAGE_PIN U20  IOSTANDARD LVCMOS33 } [get_ports { mcu_TDO }]; #U20
set_property -dict { PACKAGE_PIN T20  IOSTANDARD LVCMOS33 } [get_ports { mcu_TMS }]; #T20
set_property -dict { PACKAGE_PIN P20  IOSTANDARD LVCMOS33 } [get_ports { mcu_TDI }]; #P20



#=========================== gpioA define ======================================
#--------------------User GPIO-----------------------------------
## sw_in 5
set_property -dict { PACKAGE_PIN U19  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[31]}]
## sw_in 4
set_property -dict { PACKAGE_PIN W19  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[30]}]
## sw_in 3
set_property -dict { PACKAGE_PIN W18  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[29]}]
## sw_in 2
set_property -dict { PACKAGE_PIN Y19  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[28]}]

## cam_data 7 Y14
set_property -dict { PACKAGE_PIN V18  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[27]}]
## cam_data 6 V12
set_property -dict { PACKAGE_PIN Y18  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[26]}]

#----------------------OLED-------------------------------------------------
## VDD              W20
set_property -dict { PACKAGE_PIN W20  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[25]}]     
## VBAT             R17
set_property -dict { PACKAGE_PIN R17  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[24]}] 
## OLED_SDIN  D1    T17
set_property -dict { PACKAGE_PIN E19  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[23]}]     
## OLED_CLK   D0    U18
set_property -dict { PACKAGE_PIN E18  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[22]}]     
## OLED_RES         V16
set_property -dict { PACKAGE_PIN F17  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[21]}]     
## OLED_DC          Y14
set_property -dict { PACKAGE_PIN F16  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[20]}]   

#-----------------------UART2-----------------------------------------------
## Lora RX (uart2-tx)
set_property -dict { PACKAGE_PIN U14  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[19]}]     
## Lora TX (uart2-rx)   
set_property -dict { PACKAGE_PIN N17  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[18]}]   

#------------------------UART0--------------------------------------------
## UART TX
set_property -dict { PACKAGE_PIN N20  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[17]}]
## UART RX
set_property -dict { PACKAGE_PIN N18  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[16]}]

#------------------------EEPROM(I2C)--------------------------------------
## EEPROM SDA
set_property -dict { PACKAGE_PIN G17  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[15]}]
## EEPROM SCL
set_property -dict { PACKAGE_PIN G18  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[14]}]

#------------------------User GPIO------------------------------------------
## cam_data 2 V13
set_property -dict { PACKAGE_PIN U17  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[13]}]     
## cam_data 1 U13
set_property -dict { PACKAGE_PIN V17  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[12]}]     
## Echo
set_property -dict { PACKAGE_PIN W15  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[11]}]
## cam_pclk W14
set_property -dict { PACKAGE_PIN W14  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[10]}]
## cam)href T12
set_property -dict { PACKAGE_PIN U15  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[9]}]      
## cam_vsync U12
set_property -dict { PACKAGE_PIN P15  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[8]}]

#------------------------40P---------------------------------------------
set_property -dict { PACKAGE_PIN P14  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[7]}]
## triggle
set_property -dict { PACKAGE_PIN V15  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[6]}]
set_property -dict { PACKAGE_PIN M17 IOSTANDARD LVCMOS33 }   [get_ports {gpioA[5]}]

#--------------------------KEY----------------------------------------
## KEY2:M19
set_property -dict { PACKAGE_PIN M19  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[4]}]

#---------------------------LEDs--------------------------------------
## LED4:W13
set_property -dict { PACKAGE_PIN M20  IOSTANDARD LVCMOS33 }   [get_ports {gpioA[3]}]
## LED3:V12
set_property -dict { PACKAGE_PIN V12  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[2]}]
## LED2:U12
set_property -dict { PACKAGE_PIN U12  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[1]}]
## LED1:T12
set_property -dict { PACKAGE_PIN T12  IOSTANDARD LVCMOS33 }  [get_ports {gpioA[0]}] 





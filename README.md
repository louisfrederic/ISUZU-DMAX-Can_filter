# Why?
if you drive a offroad vehicle and would like do mount bigger tires, there are some limitations to be street leagal.
In Germany for example, your car speedo have to show you more then the actual speed. So if you drive 100 km/h in real, the speedometer shows you smoething around 103 km/h.
If you go to the technical inspection they will check the acurracy of your speedo against a GPS speedometer.

There are some devices available to adjust the speedometer but they are arount 500€...

# basic principal
The Speedometer gets it's signal over CAN bus. With a physical MITM attack on the bus we can control the speedometer.
  	 _______	           _____________		      _________
  	| SPEEDO | -----------| µController |------------| rest of the bus
 	|_______ |   can2     |_____________|     can1   |_________

The same method is used to manipulate the mileage on older used cars. With a little research, it's easy to find the right hardware.

# STM32 CAN BUS Gateway

This chinese PCB can be found on Ebay or aliexpres if you search for 

  "18 in 1 Can Bus Filter"

There a two versions out there, a blue one and agreen one. They should be the same but I only testet my code on the blue one.

![alt text](https://github.com/louisfrederic/Can_filter/blob/main/dsc_6293-2-copy.jpg?raw=true)

and this is the schematic

![alt text](https://github.com/louisfrederic/Can_filter/blob/main/can_bus_filter_schematics.png?raw=true)

For flashing the device with a St-Link you have to go wit NRST on pin7 or solder pin7 to "Boot" an cut the trace from "boot"

That are not my pictures, I found them on the internet...

## short wrap up

To extract a CAN signal, you 'carve out' the relevant bits, take the decimal value and perform a linear scaling:
```math
pyhsical value=offset+scale*raw value decimal
```

<details><summary>table with HEX and speed</summary>
<p>

  |HEX|Km/h|scale|
  |:---|:---:|:---:|
  |00 00|00|--|
  |01 00|1,5|1,5|
  |02 00|3|1,5|
  |02 7f|7,5|3|
  |03 00|9|3|
  |04 00|12|3|
  |05 00|21|4,2|
  |06 00|25,2|4,2|
  |07 00|29,4|4,2|
  |...|...|...|
  |1E 00|126|4,2|  
</p>
</details>


<details><summary> this does all the magic</summary>
<p>
  
```C
if(RxData1[0] <= 0x03){         //don't manipulate
		  TxData2[0] = RxData1[0];    //
		  TxData2[1] = RxData1[1];
	  }

	  if(RxData1[0] < 0x07 && RxData1[0] > 0x03){   // manipulate +1/2*scale between 3 and 7
		  if(RxData1[1] < 0x80){                      // check if we can add 1/2*scale without going over 255
		  		TxData2[0] = RxData1[0];						    // don't change the first bit
		  		TxData2[1] = RxData1[1] + 0x7f;				  // + 1/2*scale
		  }else{											                // RxData1[1] + 127 = > 256
		  		TxData2[0] = RxData1[0] + 0x01;				  // +1
		  		TxData2[1] = RxData1[1] - 0x80;				  // RxData[1] - 127 = difference for bit overflow
		  }
	  }

	  if(RxData1[0] >= 0x07){								          // ca. 20 km/h
		  TxData2[0] = RxData1[0] + 0x01;					      // CAN1 -> +4,2km/h -> speedo
		  TxData2[1] = RxData1[1];							        //
	  }
```
  
  under 9 km/h I don't manipulate the speed signal
  
  between 9 km/h and 30 km/h the speedo shows 1,5 km/h more
  
  over 30 km/h the speedo shows 4,2 km/h more

</p>
</details>
  

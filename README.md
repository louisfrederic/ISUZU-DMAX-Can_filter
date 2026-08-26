# ISUZU D-MAX CAN Bus Filter

A CAN-bus man-in-the-middle (MITM) speedometer correction device for the Isuzu D-Max, built on a low-cost "18-in-1 CAN Bus Filter" STM32 gateway board.

---

## Table of Contents

- [Motivation](#motivation)
- [Basic Principle](#basic-principle)
- [Hardware: STM32 CAN Bus Gateway](#hardware-stm32-can-bus-gateway)
- [Flashing the Device](#flashing-the-device)
- [Signal Analysis](#signal-analysis)
- [Correction Logic](#correction-logic)
- [Result](#result)
- [Disclaimer](#disclaimer)

---

## Motivation

Off-road vehicles fitted with larger-than-stock tires face a common street-legality problem: fitting bigger tires changes the effective wheel circumference, which throws off the calibration of the factory speedometer. In Germany specifically, road regulations require the speedometer to *never* indicate less than the vehicle's true speed — some positive margin (indicating slightly more than actual speed) is mandatory. During the periodic technical inspection (TÜV), the accuracy of the speedometer is checked against a calibrated GPS reference.

Commercial correction modules that solve this problem exist, but typically cost around €500. This project implements the same function — recalibrating the speed signal to restore a legally compliant margin — using an inexpensive off-the-shelf CAN gateway board and custom firmware.

## Basic Principle

The Isuzu D-Max's speedometer receives its input over the vehicle's CAN bus. By physically inserting a microcontroller between the speedometer and the rest of the bus (a hardware man-in-the-middle), the speed message can be intercepted, rescaled, and forwarded — correcting the indicated speed without touching any other ECU.

```
 ________          _______________          ________________
| SPEEDO |  <---  |  µController  |  <---  | rest of the bus |
|________|  CAN2  |_______________|  CAN1  |________________|
```

The controller listens on one CAN interface (CAN1, connected to the vehicle bus) and re-transmits a modified copy of the relevant frame on the second CAN interface (CAN2, connected only to the speedometer cluster), leaving the rest of the bus untouched.

> **Note:** The same physical MITM technique is commonly used (illegally) to tamper with the odometer reading on used vehicles. This project uses the identical hardware approach for a different, legitimate purpose — restoring correct speedometer calibration after a tire-size change — but the underlying hardware pattern is worth being aware of.

## Hardware: STM32 CAN Bus Gateway

The project is built around a widely available Chinese PCB, commonly sold on eBay or AliExpress under the name:

> **"18 in 1 CAN Bus Filter"**

Two hardware revisions exist in the wild — a blue and a green variant. They are believed to be functionally identical, but the firmware in this repository has only been tested on the **blue** version.

**Board photo:**
![CAN bus filter board](https://github.com/louisfrederic/Can_filter/raw/main/dsc_6293-2-copy.jpg?raw=true)

**Schematic:**
![CAN bus filter schematic](https://github.com/louisfrederic/Can_filter/raw/main/can_bus_filter_schematics.png?raw=true)

## Flashing the Device

The board is flashed using a standard ST-Link programmer. Because the board is not designed for end-user reflashing out of the box, one of the following modifications is required to enter bootloader mode:

1. Wire **NRST** to **pin 7**, or
2. Solder **pin 7** directly to the **BOOT** pin, and cut the existing trace from BOOT.

## Signal Analysis

The vehicle speed is transmitted as a 2-byte value on the CAN bus. To decode it, the relevant bits are extracted from the raw frame, converted to a decimal value, and mapped to a physical speed using a linear scaling relationship:

```
physical_value = offset + scale × raw_value
```

Reverse-engineered mapping between the raw HEX payload and the resulting speed:

| HEX (raw) | Speed (km/h) | Scale (per step) |
|-----------|--------------|-------------------|
| 00 00     | 0            | —                 |
| 01 00     | 1.5          | 1.5               |
| 02 00     | 3            | 1.5               |
| 02 7F     | 7.5          | 3                 |
| 03 00     | 9            | 3                 |
| 04 00     | 12           | 3                 |
| 05 00     | 21           | 4.2               |
| 06 00     | 25.2         | 4.2               |
| 07 00     | 29.4         | 4.2               |
| ...       | ...          | ...               |
| 1E 00     | 126          | 4.2               |

The scale factor is not constant across the full range — it increases in steps, so the correction logic below is applied piecewise depending on which speed band the raw value falls into.

## Correction Logic

The core of the firmware inspects the incoming raw speed frame (`RxData1`) and produces a corrected outgoing frame (`TxData2`) for the speedometer, according to three speed bands:

```c
if (RxData1[0] <= 0x03) {                    // low speed: pass through unmodified
    TxData2[0] = RxData1[0];
    TxData2[1] = RxData1[1];
}

if (RxData1[0] < 0x07 && RxData1[0] > 0x03) { // mid band: add half a scale step (~9–30 km/h)
    if (RxData1[1] < 0x80) {                  // safe to add without overflow
        TxData2[0] = RxData1[0];
        TxData2[1] = RxData1[1] + 0x7F;       // + 1/2 scale
    } else {                                  // would overflow the low byte
        TxData2[0] = RxData1[0] + 0x01;
        TxData2[1] = RxData1[1] - 0x80;       // carry into the high byte
    }
}

if (RxData1[0] >= 0x07) {                     // high band: add a full scale step (~30 km/h+)
    TxData2[0] = RxData1[0] + 0x01;           // CAN1 → +4.2 km/h → speedometer
    TxData2[1] = RxData1[1];
}
```

## Result

The correction produces the following behavior at the speedometer:

| True speed range | Indicated offset |
|-------------------|-------------------|
| Below ~9 km/h      | No correction applied |
| ~9 – 30 km/h       | Speedometer reads +1.5 km/h |
| Above ~30 km/h     | Speedometer reads +4.2 km/h |

This restores a consistent, legally required positive margin between indicated and actual speed after a change in tire diameter.

## Disclaimer

This project is documented for educational and personal off-road vehicle use. Modifying a vehicle's CAN bus and speedometer signal may be subject to road traffic and vehicle safety regulations that vary by country and jurisdiction, and may affect insurance coverage, warranty, or roadworthiness certification. Verify local legality before installing this device on a vehicle used on public roads, and only use it to restore compliant calibration — never to defeat safety or inspection systems.
  

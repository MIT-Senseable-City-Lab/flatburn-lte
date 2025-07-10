---
sidebar_position: 2
---

# 📦 Bill Of Materials

<!-- a nice top-down photo can be put here following the previous version below -->
<!-- ![Parts](@site/static/files/bom/parts.jpg) -->

:::info
The list below refers to a single device.
:::

### Identifier standard (ID)

All names follow the same _pattern:_

```
    X Y _ NN _ ( Z / NNN )
```

- **X** → Electronics, Mechanics, Consumables or Accessories
- **Y** → Type
- **N** → Crescent number
- **Z** → Different alternatives (A, B, C, ...)
  _or_
  **N** → Crescent sub-number

## ⚡ Electronics

:::warning
Depending on where the device will be deployed a different type of **Particle MCU** will be needed.

Make sure you order the correct option, being **EL_01_A for North America** or **EL_01_B for Europe**.
:::

### General Parts

| ID          | Type    | Subtype      | Specification                                        |                                                     URL                                                      |  Qty  |
| :---------- | :------ | :----------- | :--------------------------------------------------- | :----------------------------------------------------------------------------------------------------------: | :---: |
| EB_01       | Board   | Main         | Custom                                               |                                              [custom](#boards)                                               |   1   |
| **EL_01_A** | Logic   | MCU          | Particle B-SoM LTE-M B404X (for North America)       |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat-m1-noram-with-ethersim)    |  (1)  |
| **EL-01_B** | Logic   | MCU          | Particle B-SoM LTE B524 (for Europe)                 |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat1-3g-2g-europe-ethersim)    |  (1)  |
| EP_01       | Power   | Battery      | Lithium Ion Battery - 3.7V 10050mAh (10 Ah)          |                                [link](https://www.adafruit.com/product/5035)                                 |   1   |
| EP_02       | Power   | Solar Panel  | Voltaic Systems P105 - 5.5W 6V Solar Panel           |                [link](https://www.amazon.com/Voltaic-Systems-Small-Solar-Panel/dp/B085W9KG6V)                |   1   |
| ES_01       | Sensor  | Gas          | Alphasense A4x2 (CO+NO2) + AFE board                 |                                      [link](https://www.alphasense.com)                                      |   1   |
| ES_02       | Sensor  | Particulate  | Sensirion SPS30                                      |                [link](https://www.digikey.com/en/products/detail/sensirion-ag/SPS30/9598990)                 |   1   |
| EH_01       | Harness | Cable        | U.FL to U.FL – CINCH 415-0086-050                    | [link](https://www.digikey.com/en/products/detail/cinch-connectivity-solutions-johnson/415-0086-050/1305528) |   1   |
| EH_03       | Harness | Cable        | AreMe – 90 Degree USB-C male to female               |                            [link](https://www.amazon.com/gp/product/B0CQH48YFQ/)                             |   1   |
| EH_04       | Harness | Cable        | Qianrenon – 90º Waterproof USB-C "pass through" 30cm |                            [link](https://www.amazon.com/gp/product/B0BPCHR24G/)                             |   1   |
| EV_01       | Vault   | Storage Card | 32GB MicroSD                                         |                                [link](https://www.amazon.com/dp/B08GY9NYRM/)                                 |   1   |

### Custom Parts

#### Boards

Due to the quantity of items, these lists are available in a dedicated spreadsheet:
- [📝 Main Board](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/blob/main/hardware/Production/BOM_Flatburn_V4.csv) (2025-04-18)

#### Cables

| ID       | Type    | Subtype   | From  |  To   | Specification              |                                              URL                                              |  Qty  |
| :------- | :------ | :-------- | :---: | :---: | :------------------------- | :-------------------------------------------------------------------------------------------: | :---: |
| EH_02_01 | Harness | Wire      | EB_01 | ES_02 | 0.08 mm2 (AWG 28) x 300 mm | [link](https://www.digikey.com/en/products/detail/cnc-tech/10064-28-1-0500-001-1-TS/4486265)  |   5   |
| EH_02_02 | Harness | Socket    | EB_01 | ES_02 | JST SZH-002T-P0.5          | [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/szh-002t-p0-5/527363) |  10   |
| EH_02_03 | Harness | Housing   | EB_01 | ES_02 | JST ZHR-5                  |     [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/ZHR-5/608642)     |   2   |

<!-- | EH_01_01 | Harness | Wire      | EB_01 | ES_01 | TBD                        |                                              TBD                                              |   1   | --> 
<!-- | EH_01_02 | Harness | Connector | EB_01 | ES_01 | TBD                        |                                              TBD                                              |   2   | --> 


:::tip
💡 You can buy these boards fully assembled together with cables by contacting [My Fast PCBA from FAE Technology S.p.a.](myfastpcba@fae.technology).
:::

## ⚙️ Mechanics

### General Parts

| ID    | Type     | Subtype         | Specification                                          |                                           URL                                            |  Qty  |
| :---- | :------- | :-------------- | :----------------------------------------------------- | :--------------------------------------------------------------------------------------: | :---: |
| MM_01 | Mounting | Magnet          | RigMount Magnetic 1/4" Mount – Long                    |                [link](https://www.rigwheels.com/product/magnetic-mount/)                 |   2   |
| MF_01 | Fixation | Brass Insert    | 1/4" – for Plastic                                     |                       [link](https://www.mcmaster.com/94459A390/)                        |   2   |
| MF_02 | Fixation | Brass Insert    | M2 – flanged, for Plastic                              |                       [link](https://www.mcmaster.com/97171A300/)                        |   7   |
| MF_03 | Fixation | Brass Insert    | M3 - tapered, for Plastic                              |                       [link](https://www.mcmaster.com/94180A331/)                        |  13   |
| MF_04 | Fixation | Stainless Screw | M2X6 – socket head hex                                 |                       [link](https://www.mcmaster.com/91292A831/)                        |   7   |
| MF_05 | Fixation | Stainless Screw | M2.5X4 – socket head hex                               |                       [link](https://www.mcmaster.com/91292A015/)                        |   1   |
| MF_06 | Fixation | Stainless Screw | M3X16 – socket head hex                                |                       [link](https://www.mcmaster.com/91292A115/)                        |  13   |
| MF_07 | Fixation | Spacer          | M3 6mm spacer – ABS                                    |             [link](https://www.amazon.com/dp/B09N2NKNDJ?smid=A2U2UFFPJ8JX7R)             |   6   |
| MS_01 | Sealing  | O-Ring          | Oil-Resistant Buna-N – 3/32 Fractional Width, Dash 116 |                        [link](https://www.mcmaster.com/9452K28/)                         |   2   |
| MS_02 | Sealing  | O-Ring Cord     | Oil-Resistant Soft Buna-N – 3/32 Fractional Width      |                        [link](https://www.mcmaster.com/9864K25/)                         | 2 ft  |
| MI_01 | ID       | Label           | Plastic Thermal Label (optional)                       | [link](https://www.amazon.com/LabelManager-Handheld-Label-QWERTY-Keyboard/dp/B006O87KUA) |   2   |

### [3D Printed Parts](./mechanics)

More info in the [mechanics](./mechanics) section.

| ID    | Type  | Subtype | Specification         |  Qty  |
| :---- | :---- | :------ | :-------------------- | :---: |
| MP_01 | Print | 3D      | Top                   |   1   |
| MP_02 | Print | 3D      | Middle                |   1   |
| MP_03 | Print | 3D      | Bottom                |   1   |
| MP_04 | Print | 3D      | Switch (top piece)    |   1   |
| MP_05 | Print | 3D      | Switch (bottom piece) |   1   |
| MP_06 | Print | 3D      | Battery Cover         |   1   |


## 🧵 Consumables

| ID    | Type  | Subtype  | Specification |                                                     URL                                                     |  Qty   |
| :---- | :---- | :------- | :------------ | :---------------------------------------------------------------------------------------------------------: | :----: |
| CP_01 | Print | Filament | PLA White     | [link](https://us.store.bambulab.com/collections/bambu-lab-3d-printer-filament/products/pla-basic-filament) | 0.7 kg |

## 🔌 Accessories

:::info
The device uses USB-C for charging, flashing firmware and transfering data.

Please, choose among the cable options below according to your needs (A or B).
:::

### General

| ID      | Type  | Subtype            | Specification           |                                           URL                                           |  Qty  |
| :------ | :---- | :----------------- | :---------------------- | :-------------------------------------------------------------------------------------: | :---: |
| AL_01_A | Logic | Data + power cable | USB-C → USB-C           | [link](https://www.amazon.com/Amazon-Basics-Charging-Transfer-Certified/dp/B085SB6RSN/) |   1   |
| AL_01_B | Logic | Data + power cable | USB-C → USB-A           |  [link](https://www.amazon.com/Amazon-Basics-Charger-480Mbps-Certified/dp/B01GGKYN0A/)  |   1   |
| AP_01_A | Power | Outlet Charger     | USB-C, 30 Watts, Bivolt |           [link](https://www.amazon.com/dp/B0B2MM1W65/ref=twister_B0DNYG7ZY9)           |   1   |

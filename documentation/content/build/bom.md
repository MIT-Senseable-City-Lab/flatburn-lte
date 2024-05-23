---
sidebar_position: 2
---

# 📝 Bill Of Materials

:::info
The list below refers to a single device.
:::

### Identifier standard (ID)

All materials follow the same _pattern:_

```
    X Y - NN - ( Z / NNN )
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

Make sure you order the correct option, being **EL-01-A for North America** or **EL-01-B for Europe**.
:::

### General Parts

| ID          | Type    | Subtype      | Specification                                        |                                                     URL                                                      | Qty |
| :---------- | :------ | :----------- | :--------------------------------------------------- | :----------------------------------------------------------------------------------------------------------: | :-: |
| EB-01       | Board   | Main         | Custom                                               |                                              [custom](#boards)                                               |  1  |
| EB-02       | Board   | Sensing      | Custom                                               |                                              [custom](#boards)                                               |  1  |
| **EL-01-A** | Logic   | MCU          | Particle B-SoM LTE-M B404X (for North America)       |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat-m1-noram-with-ethersim)    | (1) |
| **EL-01-B** | Logic   | MCU          | Particle B-SoM LTE B524 (for Europe)                 |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat1-3g-2g-europe-ethersim)    | (1) |
| EP-01       | Power   | Battery      | Custom                                               |                                                [⚠️](https://)                                                |  1  |
| EP-02       | Power   | Solar Panel  | Voltaic Systems P105 - 5.5W 6V Solar Panel           |                [link](https://www.amazon.com/Voltaic-Systems-Small-Solar-Panel/dp/B085W9KG6V)                |  1  |
| ES-01       | Sensor  | Gas          | Alphasense A4x2 (CO+NO2) + AFE board                 |                                       [⚠️](https://www.alphasense.com)                                       |  1  |
| ES-02       | Sensor  | Particulate  | Sensirion SPS30                                      |                [link](https://www.digikey.com/en/products/detail/sensirion-ag/SPS30/9598990)                 |  1  |
| ES-03       | Sensor  | Thermal      | Adafruit MLX90640 110º FoV shield                    |                                [link](https://www.adafruit.com/product/4469)                                 |  1  |
| EH-01       | Harness | Cable        | U.FL to U.FL – CINCH 415-0086-050                    | [link](https://www.digikey.com/en/products/detail/cinch-connectivity-solutions-johnson/415-0086-050/1305528) |  1  |
| EH-02       | Harness | Cable        | Custom – From Sensor Board to Main Board             |                                             [custom](#harnesses)                                             |  1  |
| EH-03       | Harness | Cable        | Custom – From Particulate Sensor to Sensor Board     |                                             [custom](#harnesses)                                             |  1  |
| EH-04       | Harness | Cable        | Custom – From Thermal Sensor to Sensor Board         |                                             [custom](#harnesses)                                             |  1  |
| EH-05       | Harness | Cable        | AreMe – 90 Degree USB-C male to female               |                            [link](https://www.amazon.com/gp/product/B0CQH48YFQ/)                             |  1  |
| EH-06       | Harness | Cable        | Qianrenon – 90º Waterproof USB-C "pass through" 30cm |                            [link](https://www.amazon.com/gp/product/B0BPCHR24G/)                             |  1  |
| EV-01       | Vault   | Storage Card | 32GB MicroSD                                         |                                [link](https://www.amazon.com/dp/B08GY9NYRM/)                                 |  1  |

### Custom Parts

#### Boards

Due to the quantity of items, these lists are available in a dedicated spreadsheet:

- [📝 Main Board](https://docs.google.com/spreadsheets/d/1auN_yF0JGPYfZu30dk296jcuLERBpaTcwPwNGI0ffaU/edit?usp=sharing) – Revision 3 (2023-07-07)
- [📝 Sensing Board](https://docs.google.com/spreadsheets/d/1auN_yF0JGPYfZu30dk296jcuLERBpaTcwPwNGI0ffaU/edit#gid=1571186501) – Revision 3 (2023-07-07)

#### Harnesses

| ID        | Type    | Subtype      | From  |  To   | Specification              |                                                    URL                                                     | Qty |
| :-------- | :------ | :----------- | :---: | :---: | :------------------------- | :--------------------------------------------------------------------------------------------------------: | :-: |
| EH-02-02  | Harness | Crimped Wire | EB-02 | EB-01 | Molex 0503948051-06-B6-D   |            [link](https://www.digikey.com/en/products/detail/molex/0503948051-06-B6-D/6055654)             | 20  |
| EH-02-03  | Harness | Housing      | EB-02 | EB-01 | Molex 0511102052           | [link](https://www.digikey.com/en/products/detail/molex/0511102052/15204355?s=N4IgTCBcDaIKwEYkAYzLhAugXyA) |  2  |
| EH-03-01  | Harness | Wire         | ES-02 | EB-02 | 0.08 mm2 (AWG 28) x 300 mm |        [link](https://www.digikey.com/en/products/detail/cnc-tech/10064-28-1-0500-001-1-TS/4486265)        |  5  |
| EH-03-02  | Harness | Socket       | ES-02 | EB-02 | JST SZH-002T-P0.5          |       [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/szh-002t-p0-5/527363)        |  2  |
| EH-03-023 | Harness | Housing      | ES-02 | EB-02 | JST ZHR-5                  |           [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/ZHR-5/608642)            |  2  |
| EH-04-01  | Harness | Wire         | ES-03 | EB-02 | 0.08 mm2 (AWG 28) x 150 mm |        [link](https://www.digikey.com/en/products/detail/cnc-tech/10064-28-1-0500-001-1-TS/4486265)        |  4  |
| EH-04-02  | Harness | Socket       | ES-03 | EB-02 | JST SSHL-002T-P0.2         |       [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/SSHL-002T-P0-2/807828)       |  1  |
| EH-04-03  | Harness | Socket       | ES-03 | EB-02 | JST SSH-003T-P0.2-H        |      [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/ssh-003t-p0-2-h/2804713)      |  1  |
| EH-04-04  | Harness | Housing      | ES-03 | EB-02 | JST GHR-06V-S              |         [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/GHR-06V-S/807818)          |  1  |
| EH-04-05  | Harness | Housing      | ES-03 | EB-02 | JST SHR-04V-S-B            |        [link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/shr-04v-s-b/759868)         |  1  |

:::tip
💡 You can buy these boards fully assembled together with harnesses [here](https://).
:::

## ⚙️ Mechanics

### General Parts

| ID    | Type     | Subtype         | Specification                                          |                                              URL                                              | Qty  |
| :---- | :------- | :-------------- | :----------------------------------------------------- | :-------------------------------------------------------------------------------------------: | :--: |
| MM-01 | Mounting | Magnet          | RigMount Magnetic 1/4" Mount – Long                    |                   [link](https://www.rigwheels.com/product/magnetic-mount/)                   |  2   |
| MF-01 | Fixation | Brass Insert    | 1/4" – for Plastic                                     |                          [link](https://www.mcmaster.com/94459A390/)                          |  2   |
| MF-02 | Fixation | Brass Insert    | M2 – flanged, for Plastic                              |                          [link](https://www.mcmaster.com/97171A300/)                          |  13  |
| MF-03 | Fixation | Brass Insert    | M3 - tapered, for Plastic                              |                          [link](https://www.mcmaster.com/94180A331/)                          |  14  |
| MF-04 | Fixation | Stainless Screw | M2X6 – socket head hex                                 |                          [link](https://www.mcmaster.com/91292A831/)                          |  13  |
| MF-05 | Fixation | Stainless Screw | M2.5X4 – socket head hex                               |                          [link](https://www.mcmaster.com/91292A015/)                          |  1   |
| MF-06 | Fixation | Stainless Screw | M3X16 – socket head hex                                |                          [link](https://www.mcmaster.com/91292A115/)                          |  14  |
| MF-07 | Fixation | Washer          | M2 washer – stainless                                  |                          [link](https://www.mcmaster.com/93475A195/)                          |  5   |
| MF-08 | Fixation | Washer          | M3 washer – stainless                                  |                          [link](https://www.mcmaster.com/93475a210/)                          |  7   |
| MF-09 | Fixation | Spacer          | M3 6mm spacer – ABS                                    |               [link](https://www.amazon.com/dp/B09N2NKNDJ?smid=A2U2UFFPJ8JX7R)                |  6   |
| MO-01 | Optics   | Lens            | Germanium, Flat, Polished – ø 25.4mm x 2mm             | [link](https://www.amazon.com/diameter-HAND-POLISHED-Germanium-99-999-Element/dp/B06XXDKTSR/) |  1   |
| MS-01 | Sealing  | O-Ring          | Oil-Resistant Buna-N – 3/32 Fractional Width, Dash 116 |                           [link](https://www.mcmaster.com/9452K28/)                           |  2   |
| MF-02 | Sealing  | O-Ring Cord     | Oil-Resistant Soft Buna-N – 3/32 Fractional Width      |                           [link](https://www.mcmaster.com/9864K25/)                           | 2 ft |

### 3D Printed Parts

| ID       | Type  | Subtype | Specification             | Weight |         URL         | Qty |
| :------- | :---- | :------ | :------------------------ | -----: | :-----------------: | :-: |
| MP-01    | Print | 3D      | Top                       |  350 g | [link](./mechanics) |  1  |
| MP-02    | Print | 3D      | Middle                    |  150 g | [link](./mechanics) |  1  |
| MP-03    | Print | 3D      | Bottom                    |  150 g | [link](./mechanics) |  1  |
| MP-04    | Print | 3D      | Battery Cover             |   15 g | [link](./mechanics) |  1  |
| MP-05-01 | Print | 3D      | Switch (top)              |    5 g | [link](./mechanics) |  1  |
| MP-05-02 | Print | 3D      | Switch (bottom)           |    5 g | [link](./mechanics) |  1  |
| MP-06-A  | Print | 3D      | Camera Mount (horizontal) |   15 g | [link](./mechanics) | (1) |
| MP-06-B  | Print | 3D      | Camera Cap (no camera)    |   15 g | [link](./mechanics) | (1) |
| MP-06-C  | Print | 3D      | Camera Mount (19º)        |   15 g | [link](./mechanics) | (1) |

## 🧵 Consumables

| ID    | Type    | Subtype       | Specification |                                                     URL                                                     |  Qty   |
| :---- | :------ | :------------ | :------------ | :---------------------------------------------------------------------------------------------------------: | :----: |
| CP-01 | Print   | Filament      | PLA White     | [link](https://us.store.bambulab.com/collections/bambu-lab-3d-printer-filament/products/pla-basic-filament) | 0.7 kg |
| CS-01 | Sealing | Lubrification | Rubber Grease |                                              [link](https://)                                               |  5 g   |

## 🔌 Accessories

:::info
The device uses USB-C for charging, flashing firmware and transfering data.

Please, choose among the cable options below according to your needs (A or B).
:::

### General

| ID      | Type   | Subtype             | Specification           |       URL        | Qty |
| :------ | :----- | :------------------ | :---------------------- | :--------------: | :-: |
| AL-01-A | Logic  | Data + power cable  | USB-C → USB-C           | [link](https://) |  1  |
| AL-01-B | Logic  | Data + power cable  | USB-C → USB-A           | [link](https://) |  1  |
| AP-01-A | Power  | Outlet Charger      | USB-C, ?? Watts, Bivolt | [link](https://) |  1  |
| AP-01-B | Power  | Outlet Charger      | USB-A, ?? Watts, Bivolt | [link](https://) |  1  |
| AV-01-A | Vault  | MicroSD Card Reader | USB-C                   | [link](https://) |  1  |
| AV-01-B | Vault  | MicroSD Card Reader | USB-A                   | [link](https://) |  1  |
| AO-01   | Optics | Cleaning Cloth      | Microfiber              | [link](https://) |  1  |

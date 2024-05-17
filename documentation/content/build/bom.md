---
sidebar_position: 2
---

# 📝 Bill Of Materials

:::info
The list below refers to a single device.
:::

### Identifier (ID) standard

All materials follow the same _pattern:_

```
    X Y - NN - ( Z / NNN )
```

- **X** → Electronics, Mechanics, Consumables or Accessories
- **Y** → Type
- **N** → Crescent number
- **Z** → Different alternatives (A, B, C, ...)
  or
  **N** → Crescent sub-number

## ⚡ Electronics

### General

| ID      | Type    | Subtype        | Specification                                        |                                                     URL                                                      | Qty |
| :------ | :------ | :------------- | :--------------------------------------------------- | :----------------------------------------------------------------------------------------------------------: | :-: |
| EB-01   | Board   | Main           | Custom                                               |                                            [link](#custom-boards)                                            |  1  |
| EB-02   | Board   | Sensing        | Custom                                               |                                            [link](#custom-boards)                                            |  1  |
| EL-01-A | Logic   | MCU            | Particle (for North America)                         |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat-m1-noram-with-ethersim)    | (1) |
| EL-01-B | Logic   | MCU            | Particle (for Europe)                                |   [link](https://store.particle.io/collections/cellular/products/b-series-lte-cat1-3g-2g-europe-ethersim)    | (1) |
| EP-01   | Power   | Battery        | Custom                                               |                                                [⚠️](https://)                                                |  1  |
| EP-02   | Power   | Solar Panel    | Voltaic Systems P105 - 5.5W 6V Solar Panel           |                [link](https://www.amazon.com/Voltaic-Systems-Small-Solar-Panel/dp/B085W9KG6V)                |  1  |
| ES-01   | Sensor  | Gas            | Alphasense A4x2 (CO+NO2) + AFE board                 |                                       [⚠️](https://www.alphasense.com)                                       |  1  |
| ES-02   | Sensor  | Particulate    | Sensirion SPS30                                      |                [link](https://www.digikey.com/en/products/detail/sensirion-ag/SPS30/9598990)                 |  1  |
| ES-03   | Sensor  | Thermal Camera | Adafruit MLX90640 110º FoV shield                    |                                [link](https://www.adafruit.com/product/4469)                                 |  1  |
| EH-01   | Harness | Cable          | GSM antenna U.FL (UMCC) to U.FL (UMCC)               | [link](https://www.digikey.com/en/products/detail/cinch-connectivity-solutions-johnson/415-0086-050/1305528) |  1  |
| EH-02   | Harness | Cable          | Sensor board to mother board (custom)                |                                                [⚠️](https://)                                                |  1  |
| EH-03   | Harness | Cable          | Sensor board to SPS30                                |                                                [⚠️](https://)                                                |  1  |
| EH-04   | Harness | Cable          | Sensor board to MLX (custom)                         |                                                [⚠️](https://)                                                |  1  |
| EH-05   | Harness | Cable          | AreMe – 90 Degree USB-C male to female               |                            [link](https://www.amazon.com/gp/product/B0CQH48YFQ/)                             |  1  |
| EH-06   | Harness | Cable          | QIANRENON – 90º Waterproof USB-C "pass through" 30cm |                            [link](https://www.amazon.com/gp/product/B0BPCHR24G/)                             |  1  |
| EV-01   | Vault   | Storage Card   | 32Gb MicroSD                                         |                                [link](https://www.amazon.com/dp/B08GY9NYRM/)                                 |  1  |

### Custom Boards BOM

Due to the quantity of items, these lists are available in a dedicated spreadsheet:

- [📝 Main Board](https://docs.google.com/spreadsheets/d/1auN_yF0JGPYfZu30dk296jcuLERBpaTcwPwNGI0ffaU/edit?usp=sharing) – Revision 3 (2023-07-07)
- [📝 Sensing Board](https://docs.google.com/spreadsheets/d/1auN_yF0JGPYfZu30dk296jcuLERBpaTcwPwNGI0ffaU/edit#gid=1571186501) – Revision 3 (2023-07-07)

:::tip
💡 You can buy these boards fully assembled [here](https://).
:::

## ⚙️ Mechanics

### General

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

| ID      | Type  | Subtype | Specification             | Weight |         URL         | Qty |
| :------ | :---- | :------ | :------------------------ | -----: | :-----------------: | :-: |
| MP-01   | Print | 3D      | Top                       |  350 g | [link](./mechanics) |  1  |
| MP-02   | Print | 3D      | Middle                    |  150 g | [link](./mechanics) |  1  |
| MP-03   | Print | 3D      | Bottom                    |  150 g | [link](./mechanics) |  1  |
| MP-04   | Print | 3D      | Switch (top)              |    5 g | [link](./mechanics) |  1  |
| MP-05   | Print | 3D      | Switch (bottom)           |    5 g | [link](./mechanics) |  1  |
| MP-06   | Print | 3D      | Battery Cover             |   15 g | [link](./mechanics) |  1  |
| MP-07-A | Print | 3D      | Camera Mount (horizontal) |   15 g | [link](./mechanics) | (1) |
| MP-07-B | Print | 3D      | Camera Cap (no camera)    |   15 g | [link](./mechanics) | (1) |
| MP-07-C | Print | 3D      | Camera Mount (19º)        |   15 g | [link](./mechanics) | (1) |

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

| ID      | Type   | Subtype              | Specification      |       URL        | Qty |
| :------ | :----- | :------------------- | :----------------- | :--------------: | :-: |
| AL-01-A | Logic  | USB-C to USB-C       | Data + power cable | [link](https://) |  1  |
| AL-01-B | Logic  | USB-C to USB-A       | Data + power cable | [link](https://) |  1  |
| AP-01-A | Power  | USB-C Outlet Charger | ?? Watts, Bivolt   | [link](https://) |  1  |
| AP-01-B | Power  | USB-A Outlet Charger | ?? Watts, Bivolt   | [link](https://) |  1  |
| AV-01-A | Vault  | USB-C Card Reader    | MicroSD            | [link](https://) |  1  |
| AV-01-B | Vault  | USB-A Card Reader    | MicroSD            | [link](https://) |  1  |
| AO-01   | Optics | Cleaning Cloth       | Microfiber         | [link](https://) |  1  |

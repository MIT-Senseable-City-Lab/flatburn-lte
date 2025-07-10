---
sidebar_position: 5
---

import Top from '@site/static/files/printed/top.png'
import Middle from '@site/static/files/printed/middle.png'
import Bottom from '@site/static/files/printed/bottom.png'
<!-- import BatteryCover from '@site/static/files/printed/battery_cover.png' --> 
import SwitchTop from '@site/static/files/printed/switch_top.png'
import CameraMount from '@site/static/files/printed/camera.png'

# 🔩 Mechanics (files)

## 3D Printed parts

![3D Printed Parts](@site/static/files/printed/printedparts.jpg)

###

### Material and Suggested Settings

The easiest material to print with is _PLA_, but _ASA_ is recommended for prolonged outdoor use with high exposure to weather.

It is important to choose a **light material** (i.e. white) to avoid overheating when exposed to sun, which might interfere with measurements and overall electronics' life-cycle.

Since the device might also be exposed to moisture, vibration and stress – during trips and mounting/unmounting – it is recommended to print with settings that provide sturdyness and plenty layer and walls sealing:

- **_layer height_ – 0.28mm**
- **_top layers_ – 5**
- **_bottom layers_ – 5**
- **_walls_ – 5**
- **_infill_ – 15%**

These settings were used for time estimations below, with a total time of approximately **15 hours**.

### Supports

_image with where supports are needed_

### Parts List

:::warning
Both _middle_ and _camera_ are the only parts that need supports.
:::

:::tip
The 3d files below are provided with optimal orientation for printing, which is different from their respective reference images.
:::

| ID       | name              |                                              image                                              | support | weight | ~time |            download            |
| :------- | :---------------- | :---------------------------------------------------------------------------------------------: | :-----: | -----: | :---: | :----------------------------: |
| MP_01    | **top**           |  <img src={Top} style={{width: "75%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} />      |    N    |  350 g | 07:30 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) |
| MP_02    | **middle**        |  <img src={Middle} style={{width: "75%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} />    |    Y    |  150 g | 03:30 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) |
| MP_03    | **bottom**        |  <img src={Bottom} style={{width: "75%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} />    |    N    |  150 g | 03:30 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) |
| MP_04    | **switch_top**    |  <img src={SwitchTop} style={{width: "20%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} />   |    N    |    5 g | 00:10 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) |


<!-- | MP_06    | **battery_cover** | <img src={BatteryCover} style={{width: "25%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} />  |    Y    |   15 g | 00:30 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) | --> 
<!--  | MP_04    | **battery_cover** |  <img src={BatteryCover} style={{width: "75%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} /> |    N    |   15 g | 00:30 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) | --> 
<!-- | MP_05_02 | **switch_bottom** | <img src={SwitchBottom} style={{width: "20%", filter: "drop-shadow(0 0 1px rgba(0,0,0,1))"}} /> |    N    |    5 g | 00:10 | [link](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/stl) | --> 

The sources are available [here](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/hardware/enclosure/step) in *.STEP* format.

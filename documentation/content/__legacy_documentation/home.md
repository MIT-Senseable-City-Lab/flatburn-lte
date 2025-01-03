# FLATBURN: the open source city scanner

[Project website](https://senseable.mit.edu/flatburn).

### Flatburn Overview
<img src="https://github.com/MIT-Senseable-City-Lab/OSCS/blob/main/flatburn-images/cover.jpeg" width="400px"><img src="https://github.com/MIT-Senseable-City-Lab/OSCS/blob/main/flatburn-images/Flatburn-design.png" width="300px">

Part of the [City Scanner](https://senseable.mit.edu/cityscanner/) project at [MIT Senseable City Lab](https://senseable.mit.edu), Flatburn is an open-source, solar-powered, modular sensing platform that can be easily deployed on road vehicles to collect various environmental data. 

This repository contains all information to [build](https://github.com/MIT-Senseable-City-Lab/OSCS/tree/main/Build) your own Flatburn, [explore](https://github.com/MIT-Senseable-City-Lab/OSCS/tree/main/Explore) datasets collected using Flatburn and other City Scanner deployments worldwide and [learn](https://github.com/MIT-Senseable-City-Lab/OSCS/tree/main/Learn) how to perform environmental analysis and use the data to contribute to the development of healtier cities.

## Build
 - [/hardware](Build/Hardware)  
    - [fabrication files](Build/Hardware/Hardware%20enclosure) for Flatburn mechanical parts, including files for [3D printing](Build/Hardware/Hardware%20enclosure/To%20Print), [laser cutting](Build/Hardware/Hardware%20enclosure/To%20lasercut), and a [CAD model](Build/Hardware/Hardware%20enclosure/Flatburn_assembly.step) of the full assembly. A [bill of materials](https://docs.google.com/spreadsheets/d/1qiVyINNuo0XEu6MasEJE_p4onlJh241j/edit?usp=sharing&ouid=118244402021817924300&rtpof=true&sd=true) listing all the required mechanical components is also included.
    - [hardware schematics](Build/Hardware/Hardware%20schematics) contains `.brb` files for the main-board and the sensor-board which can be viewed using the [Cadence](https://www.cadence.com/en_US/home/tools/pcb-design-and-analysis/allegro-downloads-start.html) free viewer, schematics of the PCB in `.pdf` format, and a [bill of materials](https://docs.google.com/spreadsheets/d/1-UVIJEneydIf8kLRng4QxvzjM4k2ZoDa/edit?usp=sharing&ouid=118244402021817924300&rtpof=true&sd=true) for the electronics components.
 - [/firmware](Build/Firmware): The Flatburn main firmware code base, developed using [Particle.io](https://www.particle.io/workbench/) workbench.
 - [/fabrication handbook](Build/Handbook): The assembly guide with detailed instructions on how to build flatburn.

## Explore
- [/datasets](Explore/Datasets): `.csv` calibrated air quality datasets collected during City Scanner deployments worldwide. 
- [/calibration handbook](Explore/Calibration%20Handbook): a guide for validation and calibration of the data collected by Flatburn.

## Learn
- [/coding exercise](Learn/Coding%20Exercise): [jupyter](https://jupyter.org/) notebooks for a Python coding activity to introduce non-experts to air quality and environmental sensing and analysis methods. The activity includes exploring time series analysis, geospatial analysis, and pollution hotspot analysis. The conding activity can be used with the [datasets](Explore/Datasets) captured by previous City Scanner deployment as well as data captured by Flatburn.
- [/facilitator handbook](Learn/Facilitator%20Handbook): a facilitator guide and educational slides to implement the coding exercise as part of workshops or classroom activities.

All materials are released under [MIT license](LICENSE).

---
sidebar_position: 1
---

# 🏗️ Platform

<!-- Explain how all different researches and devices are structured under the long-term City Scanner project. -->

<div style={{textAlign: 'center', padding: 25}}>
 ```mermaid
graph LR;
    A[City Scanner Platform] -->C(Flatburn)
    A[<b>City Scanner Platform</b>] -->D(Nautilus)

    B --> |Data| E[Repository]
    C --> |Data| E[Repository]
    D --> |Data| E[Repository]

    E --> |Localized Data| F{Research}

    F --> G[Air Pollution]
    F --> H[Urban Heat]
    F --> I[Greenery Health]

```
</div>


The City Scanner platform proposes a drive-by solution to capture the spatiotemporal variation in environmental indicators in urban areas, such as air quality or the thermal flux of the built environment. Instead of relying on a dedicated fleet, the project began by deploying various types of environmental sensors on garbage trucks in the City of Cambridge.

With a few garbage trucks, we collected various environmental data for the whole city over several months. The result was 1.6 million data points including thermal images, temperature, humidity, and air quality data.

The spatio­temporal datasets play a significant role in smart city domain by empowering advanced analytics solutions for decision makers and urban managers. The City Scanner project follows a centralized IoT regime to generate a near real-time map of sensed data, building on existing fleets and moving sensing platforms.
```

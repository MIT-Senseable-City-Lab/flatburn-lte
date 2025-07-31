---
sidebar_position: 3
---

# 🚚 Deployment
# Flatburn Solar Feasibility & Runtime Estimation

This file estimates whether Flatburn will run continuously on solar power in Brazil, using all the  hardware and sensor configurations.

---

## 1. Instructions

Get your average daily sun hours for your city/month using:
   - [Global Solar Atlas](https://globalsolaratlas.info/)
   - [PVWatts Calculator](https://pvwatts.nrel.gov/)
   - Example: **Rio de Janeiro** in July ≈ 5.5 h/day

---

## 2. Flatburn Power Consumption Estimate

- **Total current:** 211 mA  
- **Total power:** 0.85 W
- **Device daily consumption:** 0.85 W × 24 h = **20.4 Wh/day**

---

## 3. Solar Panel and Battery Specs

| Parameter                | Value              | Units      | Notes                        |
|--------------------------|--------------------|------------|------------------------------|
| Solar panel power        | 5.5                | W          |                              |
| Solar panel voltage      | 6                  | V          |                              |
| Average daily sun hours  | 5.5                | hours/day  | Brazil average (Rio, July)   |
| Solar energy per day     | 5.5 × 5.5 = 30.25  | Wh/day     | Panel power × sun hours      |
| Battery capacity         | 10050 × 3.7 / 1000 = 37.19 | Wh | 10050 mAh, 3.7 V Li-Ion      |
| Days battery lasts w/o sun | 37.19 / 20.4 = 1.8 | days      | Battery Wh / daily use       |

---

## 4. Results

- **Solar panel provides:** ~30 Wh/day
- **Flatburn requires:** ~20.4 Wh/day
- **Battery stores:** ~37 Wh

- Daily solar energy is enough for continuous operation with some surplus for battery charging.
- Battery can run Flatburn for **1.8 days** without sun (cloudy/rainy buffer).

---

## 5. Tips

- For worst-case (cloudy, winter), use the lowest sun hours for your city/month.

---

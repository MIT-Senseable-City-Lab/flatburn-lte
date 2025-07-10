---
sidebar_position: 5
---

# Troubleshoot


## LED Indicators
The Flatburn environmental sensing platform includes multiple onboard LEDs that serve different roles. Proper understanding and use of these indicators can help during deployment, diagnostics, and field maintenance.
## Main RGB LED – Particle connectivity status
This LED is part of the built-in RGB LED for the particle module (e.g, Boron)
Behavior (default, unless overridden):

🟢 Breathing Cyan: Connected to Particle Cloud

🟡 Blinking Yellow: Listening mode (waiting for setup)

🔴 Blinking Red: Error state (number of blinks = error code)

⚪ Off: Device off or LED turned off in firmware

More information here: https://docs.particle.io/troubleshooting/led/

## Red LED – Charging Status
This is a dedicated red LED located near the power management circuit.
It indicates the device’s charging state:

🔴 On: The device is currently charging

⚪ Off: Charging is complete, or no power is supplied from either the solar or the USB charging

## Second RGB LED – System Status
This is a secondary RGB LED on the Flatburn board, separate from the built-in Particle RGB LED. 

🟢 Solid Green: All sensors responding; normal operation

🟡 Blinking Yellow: Temporary error (e.g., GPS searching, network retrying)

🔴 Solid Red: Critical error (e.g., sensor failure, not temp data, or pm data)

This would give quick visual insight into the system’s state.

# Data download

## Step 1: Connect Flatburn to Your Computer
## Step 2: Run the USB Data Script
## Step 3: Select the USB Port
## Step 4: Choose Files to Download
## Step 5: Choose Files to Parse
## Step 6: You're Done!

---
sidebar_position: 1
---

# 🪁 Flatburn LTE Test Plan

This test plan outlines the validation process for the Flatburn sensing platform, integrating sensors for temperature, humidity, particulate matter (PM), noise, gas, GPS. 
The validation includes hardware, firmware, system integration, and field testing.



## 1. Hardware Validation

### 1.1 Sensor Functional Testing

**Objective:**  
Verify each sensor powers up and produces reasonable output over its expected range.

**Procedure:**  
- **Temperature/Humidity:** Power up; verify readings at room temperature, with heat/cool/humidity sources.
- **PM (Air Quality):** Use reference aerosol source; check readings at baseline and elevated PM levels.
- **Noise:** Play reference sound levels in a controlled environment; verify output.
- **Gas Sensor:** Expose to calibration gases; check sensor response.



### 1.2 Accuracy and Precision Validation

**Objective:**  
Assess how accurately and precisely each sensor measures environmental parameters.

**Procedure:**  
- **Accuracy Testing:** Compare sensor readings to calibrated reference devices.
- **Precision Validation:** Repeat measurements under stable conditions and calculate standard deviation.



### 1.3 GPS Module Testing

**Objective:**  
Verify GPS fix and position accuracy under real-world conditions.

**Procedure:**  
- **Static:** Place device at a known location; compare fix to actual coordinates.
- **Dynamic:** Move device along a predefined path; record and map results.
- **Signal Integrity:** Test GPS in open, urban canyon, and indoor environments.



### 1.4 RGB, Buzzer Testing

**Objective:**  
Validate that the RGB LED and buzzer function as intended.

**Procedure:**  
- Trigger RGB and buzzer via firmware; confirm colors and sound levels match expected behaviors.



### 1.5 Power Consumption Profiling

**Objective:**  
Measure the device’s current draw in various modes to verify battery life and identify power spikes.

**Procedure:**  
- Setup: Use a precision power analyzer to measure current.
- Modes: Check current in deep sleep (μA), idle/listening (μA/mA), active sensing (mA), GPS locked (peaks up to hundreds of mA).
- Profile: Use oscilloscope to record waveform for a full duty cycle (wake-up, sensor reads, GPS fix, data send, sleep).
- Battery Test: Run the device on battery until shutdown to validate expected runtime.
- Solar Testing: Evaluate with solar panel attached.



### 1.6 Environmental Stress Testing

**Objective:**  
Verify device operation under various temperature and humidity conditions.

**Procedure:**  
- Test in temperature/humidity chamber; record sensor and device operation.



### 1.7 Logging During Hardware Tests

**Procedure:**  
- Log all test results in CSV format (timestamp, raw sensor values, GPS, buzzer/RGB status, error flags).
- Good/Bad: Note missing values or timeouts for investigation.



## 2. Software and Firmware Validation

### 2.1 Firmware Unit and Module Testing

**Objective:**  
Use updated code and verify code with sensor functionalities.

**Procedure:**  
- Unit Test: Isolate sensor drivers, measure current.
- Error Paths: Inject faults (e.g., disable a sensor) and verify firmware stability.
- New Components: Test new additions (RGB, buzzer, ADC).



### 2.2 Integration Testing (Firmware + Hardware)

**Objective:**  
Ensure hardware and firmware operate together correctly.

**Procedure:**  
- End-to-End Test: Power on device, verify sampling and logging of each sensor with timestamp.
- Sequencing: Confirm sensors are read in correct order and synchronization.
- Unplugging: Confirm unplugging one sensor does not prevent others from working; ensure errors are logged.



### 2.3 Data Pipeline Verification

**Objective:**  
Validate data flow via USB

**Procedure:**  
- Offline Buffering: retrieve data locally, log to SD.
- Retries: Test retry logic and data loss handling.
- Timestamp Sync: Ensure correct time alignment in all logged data.



## 3. System-Level and Stress Testing

### 3.1 Full System Integration Tests

**Objective:**  
Test the fully assembled system as an integrated unit (hardware, firmware, GPS).

**Procedure:**  
- Startup/Shutdown: System should boot and begin operation; power removal should not corrupt data.
- Continuous Operation: Run device continuously for ≥72 hours.



### 3.2 USB Testing

**Objective:**  
Ensure incoming data from USB is captured correctly.

**Procedure:**  
- Device Logs: Monitor via serial print.
- USB Logs: Use script to retrieve data.



## 4. Field (Drive-By) Testing

### 4.1 Test Route Planning

**Objective:**  
Validate device under real-world driving conditions.

**Procedure:**  
- Route Selection: Cover industrial, residential, urban areas; test GPS signal in challenging locations.
- Repeatability: Plan multiple routes to test reproducibility.



### 4.2 Post-Drive Analysis

**Procedure:**  
- Data Download: Retrieve all onboard logs.
- Quality Assessment: Assess data completeness and integrity.



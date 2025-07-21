---
sidebar_position: 4
---

# 📊 Data retrieval via USB cable

**Follow these steps to retrieve data from the Flatburn device using the python script.**
## Step 1: Connect Flatburn to Your Computer
Turn on your Flatburn device.

Use a USB cable to connect the device to your computer.
## Step 2: Run the USB Data Script
**For `Mac/Linux`:** 

Open your terminal and navigate to the folder where the `usb_flatburn.py` is saved: `cd path\to\your\script`

run the script with admin permissions

`sudo python usb_flatburn.py`

You will be prompted to enter your password to continue.

**For `Windows`:** 

Open Command Prompt or PowerShell as Administrator:

Press Start, search for `“cmd”` or `“powershell”`.

Right-click and choose `“Run as administrator”`.

Navigate to the folder where `usb_flatburn.py` is saved:

Run the script using Python:

`python usb_flatburn.py`
## Step 3: Select the USB Port
The script will show a list of available serial ports:

`Available serial ports:`
1. /dev/cu.xx - n/a
2. /dev/cu.xx - n/a
3. /dev/cu.xx - n/a
4. /dev/cu.usbmodem3101 - B-SoM CDC Mode

Enter the number of the port to use:

`Identify the Flatburn port (usually says "CDC Mode") and enter the corresponding number (e.g., 4)`.
## Step 4: Choose Files to Download
The script will show the available files on the SD card, for ex:

`Available files:`
1. /12052114.CSV
2. /12052130.CSV
3. /12061515.CSV

You’ll be prompted to select which files to download:

`Enter file numbers to download (comma-separated, or 'all')`

To download all files: `type: all`

`To download specific files, type their numbers (e.g., 3):`

You'll the see the prompt display:

`Downloading /12061515.CSV`

`Saved: /Users/user/sd_card_downloads/12061515.CSV (63448 bytes)`

Files are saved to the folder: `~/sd_card_downloads/`

## Step 5: Choose Files to Parse
Once files are downloaded, you'll see:
Downloaded files:
1. 12052114.CSV
2. 12052130.CSV
3. 12061515.CSV
   
You’ll be prompted to select which files to parse (i.e., clean and format for analysis):

`Enter file numbers to parse (comma-separated, or 'all'):`

To parse all downloaded files: `type: all`

`To parse specific files, type their numbers (e.g., 3):`

You'll the see the prompt display:

`Parsed and saved: /Users/user/sd_card_downloads/parsed_files/12061515_parsed.csv`

Parsed files will be saved to: `~/sd_card_downloads/parsed_files/`

Each file is saved with a `_parsed.csv` suffix.
## Step 6: You're Done!
You now have clean, ready-to-use CSV files containing environmental and telemetry data.
You can now use these files for analysis, upload to cloud storage, or integrate them into your research workflow.

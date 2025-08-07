import serial
import serial.tools.list_ports
import time
import os
import pandas as pd

# Configuration
BAUD_RATE = 115200
OUTPUT_DIR = os.path.expanduser("~/sd_card_downloads")
PARSED_FOLDER = os.path.join(OUTPUT_DIR, "parsed_files")
SKIP_FILES = {"_ACTIV~1.CSV"}

def should_skip(filename):
    normalized = filename.strip().lstrip("/").upper()
    return normalized in SKIP_FILES

def find_serial_port():
    ports = serial.tools.list_ports.comports()
    if not ports:
        raise Exception("No serial devices found!")
    print("Available serial ports:")
    for i, port in enumerate(ports, 1):
        print(f"{i}. {port.device} - {port.description}")
    selection = input("Enter the number of the port to use: ")
    index = int(selection) - 1
    if 0 <= index < len(ports):
        return ports[index].device
    raise Exception("Invalid selection!")

def get_file_list(ser):
    print("Sending LIST command...")
    ser.write(b"LIST\n")
    files = []
    start_time = time.time()
    while True:
        if time.time() - start_time > 10:
            print("ERROR: Timeout waiting for LIST_START")
            return []
        line = ser.readline().decode(errors="ignore").strip()
        if line == "LIST_START":
            break
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if line == "LIST_END":
            break
        if line:
            files.append(line)
    return files

def download_file(ser, file_name):
    ser.write(f"READ:{file_name}\n".encode())
    file_content = b""
    start_time = time.time()
    while True:
        if time.time() - start_time > 10:
            raise Exception(f"Timeout waiting for FILE_START of {file_name}")
        line = ser.readline().decode(errors="ignore").strip()
        if line.startswith("FILE_START:"):
            print(f"Downloading {file_name}...")
            break
    start_time = time.time()
    while True:
        if time.time() - start_time > 30:
            raise Exception(f"Timeout while reading content of {file_name}")
        line = ser.readline()
        if line.startswith(b"FILE_END:"):
            break
        file_content += line
    output_path = os.path.join(OUTPUT_DIR, os.path.basename(file_name))
    with open(output_path, "wb") as f:
        f.write(file_content)
    print(f"Saved: {output_path} ({len(file_content)} bytes)")

def parse_csv_file(filepath, parsed_output_folder):
    expected_column_count = 22
    new_column_names = [f'col{i+1}' for i in range(expected_column_count)]

    try:
        rows = []
        with open(filepath, 'r') as f:
            for line in f:
                parts = line.strip().split(',')
                if len(parts) == expected_column_count:
                    rows.append(parts)

        if not rows:
            print(f"No valid rows in {os.path.basename(filepath)}.")
            return

        df = pd.DataFrame(rows, columns=new_column_names)

        # Drop unwanted columns
        #columns_to_delete = [0, 1, 9, 10, 11, 12, 13, 14, 22, 23]
        columns_to_delete = [0, 1, 9, 10, 11, 12, 13, 14]
        df.drop(df.columns[columns_to_delete], axis=1, inplace=True)

        # Rename columns
        df.columns = [
            'timestamp', 'latitude', 'longitude', 'PM1_0', 'PM2_5', 'PM4',
            'PM10', 'temperature', 'humidity', 'gas_op1_w', 'gas_op1_r',
            'gas_op2_w', 'gas_op2_r', 'noise'
        ]

        os.makedirs(parsed_output_folder, exist_ok=True)
        name, ext = os.path.splitext(os.path.basename(filepath))
        output_path = os.path.join(parsed_output_folder, f"{name}_parsed.csv")
        df.to_csv(output_path, index=False)
        print(f"Parsed and saved: {output_path}")

    except Exception as e:
        print(f"Error parsing {filepath}: {e}")

def parse_downloaded_files():
    downloaded_files = sorted([
        f for f in os.listdir(OUTPUT_DIR)
        if f.lower().endswith(".csv") and not f.startswith(".")
    ])

    if not downloaded_files:
        print("No CSV files found to parse.")
        return

    print("\nDownloaded files:")
    for i, f in enumerate(downloaded_files, 1):
        print(f"{i}. {f}")

    selections = input("\nEnter file numbers to parse (comma-separated, or 'all'): ").strip().lower()
    
    if selections == 'all':
        selected_files = downloaded_files
    else:
        selected_files = []
        for num in selections.split(','):
            try:
                index = int(num.strip()) - 1
                if 0 <= index < len(downloaded_files):
                    selected_files.append(downloaded_files[index])
            except ValueError:
                continue

    for file in selected_files:
        input_path = os.path.join(OUTPUT_DIR, file)
        parse_csv_file(input_path, PARSED_FOLDER)

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    try:
        port = find_serial_port()
        print(f"Using device at {port}")
        ser = serial.Serial(port, BAUD_RATE, timeout=2)
        time.sleep(2)
        files = get_file_list(ser)

        if not files:
            print("No files found on SD card!")
            return

        print("\nAvailable files:")
        for i, file in enumerate(files, 1):
            print(f"{i}. {file}")

        selections = input("\nEnter file numbers to download (comma-separated, or 'all'): ")

        if selections.lower() == 'all':
            selected_files = [f for f in files if not should_skip(f)]
        else:
            selected_files = []
            for num in selections.split(','):
                try:
                    index = int(num.strip()) - 1
                    if 0 <= index < len(files):
                        file_to_add = files[index]
                        if not should_skip(file_to_add):
                            selected_files.append(file_to_add)
                except ValueError:
                    pass

        if not selected_files:
            print("No valid files selected.")
            return

        for file in selected_files:
            try:
                print(f"\n>>> Downloading {file}")
                download_file(ser, file)
            except Exception as e:
                print(f"Failed to download {file}: {str(e)}")

    except Exception as e:
        print(f"Error: {str(e)}")
    finally:
        if 'ser' in locals():
            ser.close()

    # Parse after download
    parse_downloaded_files()

if __name__ == "__main__":
    main()

import struct
import sys
import os
import datetime

# MUST MATCH THE STORING FORMAT EXACTLY
header_format = (
    "<" +
    "I" +            # m_FileFormat
    "I" +            # m_StartTimestamp
    "Q" +            # m_Sensor1SerialNo
    "I" +            # m_MeasurementCnt1
    "Q" +            # m_Sensor2SerialNo
    "I" +            # m_MeasurementCnt2
    "I" +            # m_MeasurementRate
    "I" +            # m_AlgorithmMode
    "f" * 8 +        # m_SensorXTempY and HumiY
    "d" * 8 +        # m_ProxiData[0-7]
    "32s" +          # AppVersion
    "32s" +          # Pi Serial
    "d" * 2 +        # Padding
    "I" * 16 +       # m_ADCChannelSel[16]
    "16s" +          # SensorID 1
    "16s" +          # SensorID 2
    "16s" +          # SampleID 1
    "16s" +          # SampleID 2
    "I" +            # m_MeasAbortCnt1
    "I" +            # m_MeasAbortCnt2
    "I"              # m_TestResult
)

header_size = struct.calcsize(header_format)


def parse_header_from_file(file_path):

    print(f"Parsing header from file: {file_path}")

    if not os.path.exists(file_path):
        print("File not found.")
        return

    with open(file_path, "rb") as f:
        header_bytes = f.read(header_size)

    if len(header_bytes) < header_size:
        print("File too small to contain full header.")
        return

    unpacked = struct.unpack(header_format, header_bytes)

    idx = 0

    print("\n--- Parsed Header ---\n")

    print(f"m_FileFormat: {unpacked[idx]}")
    idx += 1

    timestamp = unpacked[idx]
    dt_utc = datetime.datetime.utcfromtimestamp(timestamp)
    print(f"m_StartTimestamp: {timestamp} -> {dt_utc.strftime('%Y-%m-%d %H:%M:%S')} UTC")
    idx += 1

    print(f"m_Sensor1SerialNo: {unpacked[idx]}")
    idx += 1

    print(f"m_MeasurementCnt1: {unpacked[idx]}")
    idx += 1

    print(f"m_Sensor2SerialNo: {unpacked[idx]}")
    idx += 1

    print(f"m_MeasurementCnt2: {unpacked[idx]}")
    idx += 1

    print(f"m_MeasurementRate: {unpacked[idx]}")
    idx += 1

    print(f"m_AlgorithmMode: {unpacked[idx]}")
    idx += 1

    print("\nTemperature / Humidity:")

    print(f"m_Sensor1Temp1: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor1Humi1: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor1Temp2: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor1Humi2: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor2Temp1: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor2Humi1: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor2Temp2: {unpacked[idx]}"); idx += 1
    print(f"m_Sensor2Humi2: {unpacked[idx]}"); idx += 1

    print("\nm_ProxiData Section:")

    for i in range(8):
        print(f"  ProxiData[{i}]: {unpacked[idx]}")
        idx += 1

    app_version = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"App Version: {app_version}")
    idx += 1

    pi_serial = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"Pi Serial Number: {pi_serial}")
    idx += 1

    print(f"Padding1: {unpacked[idx]}"); idx += 1
    print(f"Padding2: {unpacked[idx]}"); idx += 1

    print("\nm_ADCChannelSel[16]:")

    for i in range(16):
        print(f"  ADC[{i}] = {unpacked[idx]}")
        idx += 1

    print("\nSensor / Sample IDs:")

    sensor1 = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"SensorID1: {sensor1}")
    idx += 1

    sensor2 = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"SensorID2: {sensor2}")
    idx += 1

    sample1 = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"SampleID1: {sample1}")
    idx += 1

    sample2 = unpacked[idx].decode("utf-8").strip("\x00")
    print(f"SampleID2: {sample2}")
    idx += 1

    print("\nAbort Counters:")

    print(f"m_MeasAbortCnt1: {unpacked[idx]}")
    idx += 1

    print(f"m_MeasAbortCnt2: {unpacked[idx]}")
    idx += 1

    print(f"m_TestResult: {unpacked[idx]}")
    idx += 1

    print("\nHeader parsing completed.")


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Usage:")
        print("python3 parse_rawtext_header.py <rawtext_file>")
        sys.exit(1)

    filepath = sys.argv[1]

    parse_header_from_file(filepath)

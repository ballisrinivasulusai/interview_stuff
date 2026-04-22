# Copyright (c) 2021-2024 VOC Health, Inc.
import numpy as np

APP_MEAS_BIN_FILE_NAME = "rawtext"
TOTAL_ADC_CHN_SITES_MAX = 512
metadata = {}
metadataKey = [
    "FileFormat",  # element_00
    "StartTime(epoch)",  # element_01
    "VOC_S1 SrNo", "VOC_S1 MeasurementCount", "VOC_S2 SrNo", "VOC_S2 MeasurementCount",  # element_02 to #element_05
    "MeasRate", "Algorithm Mode",  # element_06 to #element_07

    "VOC_S1 Temp1 degC", "VOC_S1 Humid1 RH", "VOC_S1 Temp2 degC", "VOC_S1 Humid2 RH",  # element_08 to #element_11
    "VOC_S2 Temp1 degC", "VOC_S2 Humid1 RH", "VOC_S2 Temp2 degC", "VOC_S2 Humid2 RH",  # element_12 to #element_15

    "P1 Dist.min mm", "P1 Dist.max mm", "P1 Dist.mean mm", "P2 Dist.min mm",  # element_16 to #element_19
    "P2 Dist.max mm", "P2 Dist.mean mm", "P4 Dist.min mm", "P4 Dist.max mm",  # element_20 to #element_23
    "P4 Dist.mean mm", "P5 Dist.min mm", "P5 Dist.max mm", "P5 Dist.mean",  # element_24 to #element_27

    "C1S1...C1S16",  # element_28
    "C2S1...C2S16",  # element_29
    "C3S1...C3S16",  # element_30
    "C4S1...C4S16",  # element_31
    "C5S1...C5S16",  # element_32
    "C6S1...C6S16",  # element_33
    "C7S1...C7S16",  # element_34
    "C8S1...C8S16",  # element_35
    "C9S1...C9S16",  # element_36
    "C10S1...C10S16",  # element_37
    "C11S1...C11S16",  # element_38
    "C12S1...C12S16",  # element_39
    "C13S1...C13S16",  # element_40
    "C14S1...C14S16",  # element_41
    "C15S1...C15S16",  # element_42
    "C16S1...C16S16"  # element_43
]  # Total 44 elements spread across total col.46 (each 4 bytes)


def ReadMeasBinFile(file=APP_MEAS_BIN_FILE_NAME):
    raw_metadata = np.fromfile(file, dtype=np.uint8, count=(184))
    rdOffset = 0
    raw_metadata_temphumid = np.fromfile(file, dtype=np.float32, offset=(10 * 4), count=(8 * 4))
    rdOffset_temphumid = 0

    # Process only initial part of Metadata (except ADC Sensor Map) i.e. total 28 elements
    for element in range(28):
        if (element == (3 - 1) or (element == (5 - 1))):
            # VOC Sensor1 SrNo. (col.2) and VOC Sensor2 SrNo. (col.4)
            buildElement = ((raw_metadata[rdOffset + 7] << (8 * 7))
                            | (raw_metadata[rdOffset + 6] << (8 * 6))
                            | (raw_metadata[rdOffset + 5] << (8 * 5))
                            | (raw_metadata[rdOffset + 4] << (8 * 4))
                            | (raw_metadata[rdOffset + 3] << (8 * 3))
                            | (raw_metadata[rdOffset + 2] << (8 * 2))
                            | (raw_metadata[rdOffset + 1] << (8 * 1))
                            | (raw_metadata[rdOffset + 0] << (8 * 0)))

            rdOffset = rdOffset + 8
            metadata.update({metadataKey[element]: buildElement})

        elif ((element >= (9 - 1)) and (element <= (16 - 1))):
            # Used to parse Temp/Humid data but source is "raw_metadata_temphumid" (float32 type) and not "raw_metadata" (uint8 type)
            metadata.update({metadataKey[element]: raw_metadata_temphumid[rdOffset_temphumid]})
            rdOffset_temphumid = rdOffset_temphumid + 1

            # Mandatory to skip <temphumid data> from "raw_metadata" and iteration can move to next elements "Proxi data and ADC"
            rdOffset = rdOffset + 4

        else:
            # Used to process all other elements
            metadata.update({metadataKey[element]: (raw_metadata[rdOffset + 3] << (8 * 3))
                                                   | (raw_metadata[rdOffset + 2] << (8 * 2))
                                                   | (raw_metadata[rdOffset + 1] << (8 * 1))
                                                   | (raw_metadata[rdOffset + 0] << (8 * 0))})
            rdOffset = rdOffset + 4

    # Process only ADC Sensor Map of Metadata, starting element 28 through element 44 (i.e. C1 to C16)
    for element in range(28, 44):
        metadata.update({metadataKey[element]: hex(raw_metadata[rdOffset + 1] << 8 | raw_metadata[rdOffset])})
        rdOffset = rdOffset + 2
    raw_sensorData = np.fromfile(file, dtype=np.float32, offset=(512 * 4))
    no_samples = np.size(raw_sensorData)// TOTAL_ADC_CHN_SITES_MAX
    # print('number of samples: ',no_samples)
    sensor_data = np.reshape(raw_sensorData, [no_samples, TOTAL_ADC_CHN_SITES_MAX])
    return metadata, raw_metadata_temphumid, sensor_data

if __name__ == '__main__':
    meta_data, humidity, sensor_data = ReadMeasBinFile('rawtext')
    print(meta_data)
import sysv_ipc
import os,sys,struct,time

key = 0x1000
mq = sysv_ipc.MessageQueue(key, sysv_ipc.IPC_CREAT)
CMDID_SET_PREVIOUS_SAMPLE_RATE = 0

while True:
    print(f"Sending Message")
    bytearray1 = struct.pack("I", CMDID_SET_PREVIOUS_SAMPLE_RATE)
    CMDID_SET_PREVIOUS_SAMPLE_RATE = CMDID_SET_PREVIOUS_SAMPLE_RATE + 1

    mq.send(bytearray1, True, type=1)
    time.sleep(2)



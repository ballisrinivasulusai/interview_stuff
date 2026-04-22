import sysv_ipc
import struct

key = 0x1000
mq = sysv_ipc.MessageQueue(key, sysv_ipc.IPC_CREAT)

while True:
    print(f"Waiting for receiving the Message")
    message,t = mq.receive()  # t = message type
    value = struct.unpack("I", message)[0]   # convert bytes → integer
    print(f"Received value: {value}")


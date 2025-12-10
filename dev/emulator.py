import serial

ser = serial.Serial(
    port="COM1",
    baudrate=9600,
    timeout=1
)

while True:
    data = input("Enter command " )
    if not data:
        continue
    print(bytes(data, "utf8"))
    ser.write(bytes(f"{data}\r\n", "utf8"))
    ser.flush()


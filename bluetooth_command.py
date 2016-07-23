input sys
input bluetooth

bd_addr = "98:D3:32:10:48:E8"
port = 1

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect ((bd_addr, port))

sock.send(sys.argv[1]) #or anything else
sock.close()
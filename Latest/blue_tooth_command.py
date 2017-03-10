
# CMAKE command
# find_package(PythonLibs REQUIRED)
# include_directories(${PYTHON_INCLUDE_DIRS})
# target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})

input sys
input bluetooth

class BlueToothConnect:
    
    def __init__(self):
        self.bd_addr = "98:D3:32:10:48:E8"
        self.port = 1
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.sock.connect ((bd_addr, port))
        print "BLUETOOTH INIT"
    
    def sendCommand(command):
        self.sock.send(command) #or anything else
        print "BLUETOOTH SENT %s" % command
        
    def closeConnection():
        self.sock.close()
        print "BLUETOOTH SOCK CLOSE"
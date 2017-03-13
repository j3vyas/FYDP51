
# CMAKE command
# find_package(PythonLibs REQUIRED)
# include_directories(${PYTHON_INCLUDE_DIRS})
# target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})

import sys
import bluetooth

class BlueToothConnect:
    
    def __init__(self):
        self.bd_addr = "98:D3:32:10:48:E8"
        self.port = 1
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.sock.connect((self.bd_addr, self.port))
    
    def sendCommand(self, command):
        self.sock.send(command) #or anything else
        
    def closeConnection(self):
        self.sock.close()

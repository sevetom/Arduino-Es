import serial.tools.list_ports

class Controller:
    def __init__(self):
        self.serialInst = serial.Serial()
        self.portsList = []
        self.portVar = None

    def select_port(self):
        ports = serial.tools.list_ports.comports()
        for onePort in ports:
            full_port = str(onePort)
            if " " in str(onePort):
                onePort = str(onePort).split(" ")
                onePort = onePort[0]
            self.portsList.append(str(onePort))
            print(f"{self.portsList.index(str(onePort))} - {str(full_port)}")

        val = input("Select Port: ")
        self.portVar = self.portsList.pop(int(val))
        print(f"Selected Port: {self.portVar}")

        self.serialInst.baudrate = 9600
        self.serialInst.timeout = 1
        self.serialInst.port = self.portVar
        print("Opening Port")
        self.serialInst.open()

    def read_serial(self):
        while True:
            if self.serialInst.in_waiting:
                packet = self.serialInst.readline()
                print(packet.decode('utf').rstrip('\n'))
      
    
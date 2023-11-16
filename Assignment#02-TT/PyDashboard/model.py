

class Model:
    def __init__(self):
        self.car_washed = 0
        self.status = True
        self.temperature = 0
    
    def get_status(self):
        return self.temperature
    
    def get_car_washed(self):
        return self.car_washed
    
    def get_temperature(self):
        return self.temperature
    
    def set_status(self, status):
        self.status = status
        
    def set_car_washed(self, car_washed):
        self.car_washed = car_washed
    
    def set_temperature(self, temperature):
        self.temperature = temperature
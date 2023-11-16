import tkinter as tk
import model as md

class View:

    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Dashboard")
        self.model = md.Model()
        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        screen_width = screen_width * 2 // 9
        screen_height = screen_height * 2 // 9
        x_position = int(screen_width + screen_width * 0.75)
        y_position = int(screen_height + screen_height * 0.75)
        self.root.geometry(f"{screen_width}x{screen_height}+{x_position}+{y_position}")
        self.root.resizable(False, False)
        self.root.protocol("WM_DELETE_WINDOW", exit)
        
        self.frame_lable = tk.Frame(self.root)
        self.frame_lable.pack(side=tk.TOP, fill=tk.X, padx=10, pady=10)
        
        self.label_car = tk.Label(self.frame_lable, text="Car Washed\n"+str(self.model.get_car_washed()))
        self.label_status = tk.Label(self.frame_lable, text="Status\n"+str(self.model.get_status()))
        self.label_temp = tk.Label(self.frame_lable, text="Temperature\n"+str(self.model.get_temperature()))
        
        self.label_car.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        self.label_status.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        self.label_temp.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        self.frame_button = tk.Frame(self.root)
        self.frame_button.pack(side=tk.BOTTOM, fill=tk.X, padx=10, pady=10)
        
        self.button = tk.Button(self.frame_button, text="Intervieni", command=self.on_click)
        self.button.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        
        
    def on_click(self):
        print("4070 in arrivo!!")

    def run(self):
        self.root.mainloop()
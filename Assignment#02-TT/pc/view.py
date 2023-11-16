import tkinter as tk
import model as md

class View:

    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Dashboard")
        self.model = md.Model()
        self.configure_main_window()
        self.frame_lable = tk.Frame(self.root)
        self.frame_lable.pack(side=tk.TOP, fill=tk.X, padx=10, pady=10)
        self.error_flashing = False
        self.error_flash_interval = 500
        
        self.label_car = tk.Label(self.frame_lable, text="Car Washed\n"+str(self.model.get_car_washed()))
        self.label_status = tk.Label(self.frame_lable, text="Status\n"+str("OK" if self.model.get_status() else "ERROR"), fg="white", bg="gray", borderwidth=5, relief="sunken")
        self.label_temp = tk.Label(self.frame_lable, text="Temperature\n"+str(self.model.get_temperature()))
        
        for label in [self.label_car, self.label_status, self.label_temp]:
            label.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        self.frame_button = tk.Frame(self.root)
        self.frame_button.pack(side=tk.BOTTOM, fill=tk.X, padx=10, pady=10)
        
        self.button = tk.Button(self.frame_button, text="Intervieni", command=self.on_click)
        self.button.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
    def configure_main_window(self):
        screen_width = self.root.winfo_screenwidth() * 2 // 9
        screen_height = self.root.winfo_screenheight() * 2 // 9
        x_position = int(screen_width + screen_width * 0.75)
        y_position = int(screen_height + screen_height * 0.75)
        self.root.geometry(f"{screen_width}x{screen_height}+{x_position}+{y_position}")
        self.root.resizable(False, False)
        self.root.protocol("WM_DELETE_WINDOW", exit)
    
    def update_labels(self):
        self.label_car.config(text="Car Washed\n"+str(self.model.get_car_washed()))
        self.label_status.config(text="Status\n"+str("OK" if self.model.get_status() else "ERROR"), fg="white", bg="gray")
        self.label_temp.config(text="Temperature\n"+str(self.model.get_temperature()))
        if not self.model.get_status():
            self.flash_label()
        
    def flash_label(self):
        if not self.model.get_status():
            if self.error_flashing:
                self.label_status.config(fg="red", bg="white")
            else:
                self.label_status.config(fg="white", bg="red")
            self.error_flashing = not self.error_flashing
            self.root.after(self.error_flash_interval, self.flash_label)
        else:
            # reset label: nessuno sfondo
            self.label_status.config(fg="white", bg="gray")
            
    def on_click(self):
        print("cambia stato")
        self.model.set_status(not self.model.get_status())
        self.update_labels()

    def run(self):
        self.update_labels()
        self.root.mainloop()
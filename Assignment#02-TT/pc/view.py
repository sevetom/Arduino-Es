import tkinter as tk

class View:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Dashboard")

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        screen_width = screen_width * 2 // 5
        screen_height = screen_height * 2 // 5
        x_position = int(screen_width/2 - 200)
        y_position = int(screen_height/2 - 200)
        self.root.geometry(f"{screen_width}x{screen_height}+{x_position}+{y_position}")

        self.button = tk.Button(self.root, text="4070", command=self.on_click)
        self.button.pack(side=tk.BOTTOM)
        
    def on_click(self):
        print("Button clicked")

    def run(self):
        self.root.mainloop()
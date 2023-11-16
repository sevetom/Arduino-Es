import threading
import controller as cn
import view as vw

if __name__ == "__main__":
    controller = cn.Controller()
    view = vw.View()
    controller.select_port()
    t1 = threading.Thread(target=controller.read_serial)
    t1.start()
    view.run()
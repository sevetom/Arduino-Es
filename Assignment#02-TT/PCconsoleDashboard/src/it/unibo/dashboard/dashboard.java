package it.unibo.dashboard;

import it.unibo.dashboard.controller.DashboardController;
import it.unibo.dashboard.model.ConsoleDashboard;
import it.unibo.dashboard.view.WashData;
import it.unibo.dashboard.view.ArduinoCommunication;

public final class dashboard {
    public static void main(String[] args) {
        ConsoleDashboard dashboard = new ConsoleDashboard();
        ArduinoCommunication arduino = new ArduinoCommunication();
        WashData washData = new WashData();
        DashboardController controller = new DashboardController(dashboard, arduino, washData);
        controller.start();
    }
}

// DashboardController.java
package it.unibo.dashboard.controller;

import it.unibo.dashboard.view.WashData;
import it.unibo.dashboard.model.ConsoleDashboard;
import it.unibo.dashboard.view.ArduinoCommunication;

public class DashboardController {
    private ConsoleDashboard dashboard;
    private ArduinoCommunication arduino;
    private WashData washData;

    public DashboardController(ConsoleDashboard dashboard, ArduinoCommunication arduino, WashData washData) {
        this.dashboard = dashboard;
        this.arduino = arduino;
        this.washData = washData;
    }

    public void start() {
        
    }
}

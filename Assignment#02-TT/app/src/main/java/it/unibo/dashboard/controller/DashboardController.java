// DashboardController.java
package it.unibo.dashboard.controller;

import it.unibo.dashboard.model.WashData;
import it.unibo.dashboard.view.ConsoleDashboard;

public class DashboardController {
    private ConsoleDashboard dashboard;
    private WashData washData;

    public DashboardController(WashData washData) {
        this.dashboard = dashboard;
        this.washData = washData;
    }
}

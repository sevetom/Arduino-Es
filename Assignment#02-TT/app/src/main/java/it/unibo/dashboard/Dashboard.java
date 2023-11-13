package it.unibo.dashboard;

import it.unibo.dashboard.controller.DashboardController;
import it.unibo.dashboard.model.WashData;
import it.unibo.dashboard.view.ConsoleDashboard;

public final class Dashboard {
    public static void main(String[] args) {
        ConsoleDashboard dashboardView = new ConsoleDashboard();
        WashData washData = new WashData();
        DashboardController controller = new DashboardController(dashboardView, washData);
    }
}
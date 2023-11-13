package it.unibo.dashboard.model;

public class WashData {
    private int totalWashes;
    private int currentTemperature;
    private boolean maintenanceRequired;

    public WashData() {
        this.totalWashes = 0;
        this.currentTemperature = 0;
        this.maintenanceRequired = false;
    }

    public int getTotalWashes() {
        return totalWashes;
    }

    public void setTotalWashes(int totalWashes) {
        this.totalWashes = totalWashes;
    }

    public int getCurrentTemperature() {
        return currentTemperature;
    }

    public void setCurrentTemperature(int currentTemperature) {
        if (currentTemperature < 0) {
            this.currentTemperature = 0;
        } else if (currentTemperature > 100) {
            this.currentTemperature = 100;
        } else {
            this.currentTemperature = currentTemperature;
        }
    }

    public boolean isMaintenanceRequired() {
        return maintenanceRequired;
    }

    public void setMaintenanceRequired(boolean maintenanceRequired) {
        this.maintenanceRequired = maintenanceRequired;
    }

    @Override
    public String toString() {
        return "WashData [currentTemperature=" + currentTemperature + ", maintenanceRequired=" + maintenanceRequired
                + ", totalWashes=" + totalWashes + "]";
    }
}
// ConsoleDashboard.java
package it.unibo.dashboard.view;

import javax.swing.JFrame;
import javax.swing.JPanel;

import it.unibo.dashboard.model.WashData;

public class ConsoleDashboard extends JFrame {
    private static final long serialVersionUID = 1L;

    private JPanel panel;

    public ConsoleDashboard() {
        super("Dashboard");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(500, 500);
        this.setVisible(true);
        this.panel = new JPanel();
        this.add(panel);
    }

    public void updateDashboard(WashData washData) {
        
    }
}
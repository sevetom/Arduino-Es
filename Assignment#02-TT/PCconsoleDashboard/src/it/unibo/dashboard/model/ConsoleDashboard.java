// ConsoleDashboard.java
package it.unibo.dashboard.model;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class ConsoleDashboard extends JFrame {
    private static final long serialVersionUID = 1L;

    public ConsoleDashboard() {
        super("Dashboard");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(800, 600);
        this.setVisible(true);
    }

    public void setPanel(JPanel panel) {
        this.getContentPane().removeAll();
        this.getContentPane().add(panel);
        this.revalidate();
        this.repaint();
    }
}
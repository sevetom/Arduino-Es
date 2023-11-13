// ConsoleDashboard.java
package it.unibo.dashboard.view;

import it.unibo.dashboard.controller.DashboardController;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import java.awt.Toolkit;

import java.awt.BorderLayout;
import java.awt.Dimension;

public class ConsoleDashboard extends JFrame {
    private static final long serialVersionUID = 1L;

    private JPanel panel;
    private DashboardController controller;

    public ConsoleDashboard(DashboardController controller) {
        super("Dashboard");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int width = (int) (screenSize.width * 1.0 / 3.0);
        int height = (int) (screenSize.height * 1.0 / 6.0);
        this.setSize(width, height);
        this.setLocationRelativeTo(null);

        this.panel = new JPanel();
        this.createMainPanel();
        this.add(panel);

        this.controller = controller;
    }

    // nel panel devono essere presenti 3 box in alto allineati al centro e sotto un bottone, le informazioni le prende dal controller
    private void createMainPanel() {
        panel.setLayout(new BorderLayout());
        createMonitor("Car Washed:", BorderLayout.WEST);
        createMonitor("Temperature:", BorderLayout.EAST);
        createMonitor("Status:" , BorderLayout.CENTER);
        createStartButton();
    }

    public void start() {
        this.setVisible(true);
    }

    private void createMonitor(String text, String place) {
        JPanel monitorPanel = new JPanel();
        JLabel monitorLabel = new JLabel(text);
        JTextField monitorField = new JTextField(5);
        monitorField.setEnabled (false);
        monitorPanel.add(monitorLabel);
        monitorPanel.add(monitorField);
        panel.add(monitorPanel, place);
    }

    private void createStartButton() {
        JButton button = new JButton("Start");
        button.addActionListener(e -> {
            System.out.println("Start");
        });
        panel.add(button, BorderLayout.SOUTH);
    }
}
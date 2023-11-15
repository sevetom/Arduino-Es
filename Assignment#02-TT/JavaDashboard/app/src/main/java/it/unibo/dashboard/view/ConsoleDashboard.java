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
    private JTextField carWashedField;
    private JTextField temperatureField;
    private JTextField statusField;
    private JButton maintenanceButton;

    public ConsoleDashboard(DashboardController controller) {
        super("Dashboard");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int width = (int) (screenSize.width * 3.0 / 7.0);
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
        carWashedField = createMonitor("Car Washed:", BorderLayout.WEST);
        temperatureField = createMonitor("Temperature:", BorderLayout.EAST);
        statusField = createMonitor("Status:" , BorderLayout.CENTER);
        statusField.setColumns(20);
        createMaintenanceButton();
    }

    public void start() {
        this.setVisible(true);
    }

    private JTextField createMonitor(String text, String placement) {
        JPanel monitor = new JPanel();
        JLabel label = new JLabel(text);
        JTextField field = new JTextField(5);
        field.setEnabled(false);
        monitor.add(label);
        monitor.add(field);
        panel.add(monitor, placement);
        return field;
    }

    private void createMaintenanceButton() {
        maintenanceButton = new JButton("Maintenance done");
        maintenanceButton.addActionListener(e -> {
            System.out.println("Maintenance done");
        });
        panel.add(maintenanceButton, BorderLayout.SOUTH);
        maintenanceButton.setEnabled(false);
    }

    public void updateValues(int carWashed, int temperature, boolean status) {
        carWashedField.setText(String.valueOf(carWashed));
        temperatureField.setText(String.valueOf(temperature)+"°C");
        maintenanceButton.setEnabled(!status);
        if (status) {
            statusField.setText("All good");
        } else {
            statusField.setText("Maintenance required");
        }
    }
}
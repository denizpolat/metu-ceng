import javax.swing.*;

public class SimulationRunner {
    private JFrame window = new JFrame();
    private Display display = new Display();



    private static void createAndShowGUI() {
        SimulationRunner runner = new SimulationRunner();
        runner.window.add(runner.display);
        runner.window.setTitle(Common.getTitle());
        runner.window.setSize(Common.getWindowWidth(), Common.getWindowHeight());
        runner.window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        runner.window.setResizable(false);
        runner.window.setLocationRelativeTo(null);
        runner.window.setVisible(true);

        Common.prepareCommon();
        new Timer(50, actionEvent -> {
            runner.display.repaint();
        }).start();
    }


    public static void main(String[] args) { SwingUtilities.invokeLater(SimulationRunner::createAndShowGUI); }
}
import javax.swing.*;
import java.awt.*;

public class Display extends JPanel {



    public Display() { this.setBackground(new Color(180, 180, 180)); }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(Common.getWindowWidth(), Common.getWindowHeight());
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        Common.getGoldPrice().draw((Graphics2D) g);
        g.drawLine(0, Common.getUpperLineY(), Common.getWindowWidth(), Common.getUpperLineY());

        // draw all entities
        for(Country country : Common.getCountries()) {
            country.draw(g2d);
        }
        for(Agent agent : Common.getAgents()) {
            agent.draw(g2d);
        }
        for (Order order : Common.getOrders()) {
            order.draw(g2d);
        }
        // and then step them
        Common.stepAllEntities();

    }

}
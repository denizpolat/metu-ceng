import java.awt.*;

public class GoldPrice extends Entity {
    private double currentPrice = 50;
    private int maxChange = 3;
    private Font font = new Font("Verdana", Font.PLAIN, 40);

    public GoldPrice(double x, double y) { super(x, y); }

    // getter
    public double getCurrentPrice() { return currentPrice; }

    @Override
    public void draw(Graphics2D g2d) {
        g2d.setColor(Color.BLACK);
        g2d.setFont(font);
        g2d.drawString(String.format("Live Gold Price: %.2f $", currentPrice), position.getIntX(), position.getIntY());
    }

    @Override
    public void step() {
        double change = Common.getRandomGenerator().nextDouble() * maxChange;
        currentPrice = Common.getRandomGenerator().nextBoolean() ? currentPrice + change : currentPrice - change;
        if(currentPrice <= 0) currentPrice = 1;
        else if(currentPrice >= 100) currentPrice = 99;
    }
}
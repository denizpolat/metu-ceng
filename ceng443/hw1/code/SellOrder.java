import java.awt.*;

// same logic with BuyOrder, comments provided in that class are also applicable for this class.

public class SellOrder extends Order {
    public SellOrder(Country c) {
        super(c.position.getX(), c.position.getY());
        this.setCountry(c);
        setInitial();
    }

    @Override
    public void draw(Graphics2D g) {
        // draw tiny SellOrder balls
        int posX = getPosition().getIntX();
        int posY = getPosition().getIntY();
        g2d = g;
        g2d.setColor(Color.PINK);
        g2d.fillOval(posX, posY, 20, 20);

        g2d.drawString(getCountryInitials(), posX + 3, posY - 6);

        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("default", Font.BOLD, 12));
        g2d.drawString(new StringBuilder().append(getOrderAmount()).toString(), posX + 4, posY + 12);
    }

    @Override
    public void step() {
        super.step();
        executeOrder();
    }

    @Override
    public void executeOrder() {
        if(!hasExecuted && hasReachedTarget() && this.getCountry().getGold() >= getOrderAmount()) {
            getCountry().changeCash((int)(Common.getGoldPrice().getCurrentPrice() * getOrderAmount()));
            getCountry().changeGold((int)(-1 * getOrderAmount()));
            hasExecuted = true;
        }
    }

    @Override
    public void stolenByAgent(Agent a) {
        int orderCash = (int)(getOrderAmount() * Common.getGoldPrice().getCurrentPrice());
        a.setStolenMoney(a.getStolenMoney() + orderCash); // set agent cash
        a.getCountry().changeCash(orderCash); // agent's country earned money
        //this.getCountry().changeCash(orderCash); // order's country bought
        //a.getCountry().changeGold(getOrderAmount());
        this.getCountry().changeGold(-1 * getOrderAmount());
        Common.getOrders().remove(this);

        // In other words, the IA gains [order amount x current gold price] cash. If it is
        //a buy order, order’s country loses while IA’s country gains the amount of
        //cash described above. When it is a sell order, order’s country loses while
        //IA’s country gains [order amount] gold.
    }
}
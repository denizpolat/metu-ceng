import java.awt.*;

public class BuyOrder extends Order {

    // constructor
    public BuyOrder(Country c) {
        super(c.position.getX(), c.position.getY());
        this.setCountry(c);
        this.setInitial();
    }

    @Override
    public void draw(Graphics2D g) {
        int posX = getPosition().getIntX();
        int posY = getPosition().getIntY();
        g2d = g;
        g2d.setColor(Color.GREEN);
        g2d.fillOval(posX, posY, 20, 20);

        g2d.drawString(getCountryInitials(), posX + 3, posY - 6);

        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("default", Font.BOLD, 12));
        g2d.drawString(String.valueOf(getOrderAmount()), posX + 4, posY + 12);



    }

    @Override
    public void step() {
        super.step();
        executeOrder(); // check whether the order will be executed after every step
    }

    @Override
    public void executeOrder() {
        // if the order has hit to upperLineY AND the generating country has enough money; execute the order.
        int moneyChange = (int)(Common.getGoldPrice().getCurrentPrice() * getOrderAmount());
        if(!hasExecuted && hasReachedTarget() && this.getCountry().getCash() >= moneyChange) {
            getCountry().changeCash((-1 * moneyChange));
            getCountry().changeGold(getOrderAmount());
            hasExecuted = true;
        }
    }

    @Override
    public void stolenByAgent(Agent a) {
        int orderCash = (int)(getOrderAmount() * Common.getGoldPrice().getCurrentPrice());
        a.setStolenMoney(a.getStolenMoney() + orderCash); // set agent stolenMoney
        a.getCountry().changeCash(orderCash); // agent's country earned money
        this.getCountry().changeCash(-1 * orderCash); // order's country lost money
        //a.getCountry().changeGold(-1 * getOrderAmount());
        //this.getCountry().changeGold(getOrderAmount());
        Common.getOrders().remove(this);

        // In other words, the IA gains [order amount x current gold price] cash. If it is
        //a buy order, order’s country loses while IA’s country gains the amount of
        //cash described above. When it is a sell order, order’s country loses while
        //IA’s country gains [order amount] gold.
    }
}
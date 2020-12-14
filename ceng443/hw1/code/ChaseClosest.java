import java.awt.*;
import java.util.ArrayList;

public class ChaseClosest extends State {

    private Position target;


    // constructor
    public ChaseClosest(Agent a) {
        super(a);
        target = findClosestOrder();
        setSpeed(Common.randomBetween(2, 5));
        setStateName("ChaseClosest");
    }
    @Override
    public void drawState(int posX, int posY, Graphics2D g2d) {
        g2d.drawString(getStateName(), posX, posY);
    }

    @Override
    public void stepState() {
        target = findClosestOrder(); // calculate closest order before every step
        chaseOneUnitTime();
    }


    // check all orders and find nearest
    private Position findClosestOrder() {
        double minDistance = Double.MAX_VALUE;
        Order closestOrder = null;
        ArrayList<Order> orders = new ArrayList<>(Common.getOrders());
        if(orders.size() == 0) return new Position(-1, -1);
        for(Order order : orders) {
            double currDistance = order.getPosition().distanceTo(getPosition().getX(), getPosition().getY());
            if(currDistance < minDistance) {
                minDistance = currDistance;
                closestOrder = order;
            }
        }
        return closestOrder.getPosition();
    }

    // making one step towards target order in one unit time
    private void chaseOneUnitTime() {
        target = findClosestOrder();
        Position p = agent.position;
        double distance = target.distanceTo(p.getX(), p.getY());
        double stepX = ((target.getX() - p.getX()) / distance) * getSpeed();
        double stepY = ((target.getY() - p.getY()) / distance) * getSpeed();

        boolean notReachedYet; // this flag shows whether it is possible for IA to go one step further or not. If going
        // one step will NOT make IA pass its target point, it can go one step. Else, it will go to its target point and
        // not any further.

        Position currPos = agent.position;
        Position newPos;
        newPos = new Position(currPos.getX() + stepX, currPos.getY() + stepY);
        if(stepX>0) { // does IA chase order from left or right?
            if(stepY>0) { // from top or bottom?
                notReachedYet = (target.getX() > newPos.getX()) && (target.getY() > newPos.getY());
            }
            else notReachedYet = target.getX() > newPos.getX() && newPos.getY() > target.getY();
        }
        else {
            if(stepY>0) {
                notReachedYet = (target.getX() < newPos.getX()) && (newPos.getY() < target.getY());
            }
            else notReachedYet = target.getX() < newPos.getX() && target.getY() < newPos.getY();
        }
        if(!notReachedYet) {
            agent.setPosition(target);
        }
        else agent.setPosition(newPos);
    }
}
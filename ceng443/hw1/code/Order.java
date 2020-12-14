import java.awt.*;
import java.nio.file.Path;

public abstract class Order extends Entity {
    private final int orderAmount;
    private final int speed;
    private Country country;
    private final Position target;
    private String countryInitials;
    private double stepX, stepY;
    private boolean isDirectionToLeft;
    private boolean hasStolen;
    protected boolean hasExecuted;  // precaution ::::: prevent from doing same thing more than once
    protected Graphics2D g2d;



    //constructor
    public Order(double x, double y) {
        super(x, y);
        orderAmount = Common.randomBetween(1, 5);
        hasExecuted = false;
        hasStolen = false;
        country = null;
        countryInitials = null;
        isDirectionToLeft = false;
        speed = Common.randomBetween(1, 5);
        target = new Position(Common.randomBetween(0, Common.getWindowWidth()), Common.getUpperLineY());
        calculateStepSizes();
    }

    @Override
    public abstract void draw(Graphics2D g2d);

    @Override
    public void step() {
        // if makeing one step wont let ia pass its target, then it can. else, it will stop by making a
        // smaller-than-regular step, at its target position.

        double epsilon = 0.000001;
        Position newPos = new Position(getPosition().getX() + stepX, getPosition().getY() + stepY);
        if(Math.abs(target.getX() - newPos.getX()) > epsilon && (newPos.getY() - target.getY()) > epsilon) {
            this.setPosition(newPos);
        }
        else this.setPosition(new Position(target.getX(), target.getY()));
    }


    // speed's x and y components && direction of order (left-or-right) flag is set.
    private void calculateStepSizes() {
        double distance = getPosition().distanceTo(target.getX(), target.getY());
        double distanceX = target.getX() - getPosition().getX();
        double distanceY = target.getY() - getPosition().getY();
        double cos = distanceX / distance;
        double sin = distanceY / distance;
        stepX = speed * cos;
        stepY = speed * sin;
        if(stepX <= 0) isDirectionToLeft = true;
    }

    // has the order hit the upperLineY ?
    protected boolean hasReachedTarget() {
        double epsilon = 0.00001;
        if(Math.abs(target.getY() - position.getY()) > epsilon) return false;
        else {
            if(isDirectionToLeft && Math.abs(target.getX() - position.getX()) > epsilon) return false;
            else return isDirectionToLeft || !(Math.abs(target.getX() - position.getX()) > epsilon);
        }
    }

    // initials to display
    protected void setInitial() {
        switch (country.getCountryName()) {
            case "Turkey":
                countryInitials = "TR";
                break;
            case "China":
                countryInitials = "CN";
                break;
            case "USA":
                countryInitials = "US";
                break;
            case "Russia":
                countryInitials = "RU";
                break;
            case "Israel":
                countryInitials = "IL";
                break;
        }
    }

    public abstract void executeOrder();

    public abstract void stolenByAgent(Agent a);

    // setters
    public void setCountry(Country c) { this.country = c; }

    public void setPosition(Position p) { this.position = p; }

    public void setHasStolen() { hasStolen = true; }


    // getters
    public Country getCountry() { return country; }

    public int getOrderAmount() { return orderAmount; }

    public String getCountryInitials() { return countryInitials; }

    public boolean getHasStolen() { return hasStolen; }




}
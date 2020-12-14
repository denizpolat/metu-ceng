import java.awt.*;

public class GotoXY extends State {

    private Position target;
    private double stepX, stepY;

    public GotoXY(Agent a) {
        super(a);
        setSpeed(Common.randomBetween(1, 5)); // scalar random speed between 1-5
        setStateName("GotoXY");
        generateTargetPosition();
        calculateStepSize();
    }



    @Override
    public void drawState(int posX, int posY, Graphics2D g2d) {
        g2d.drawString(getStateName(), posX, posY);
    }

    @Override
    public void stepState() {
        // same logic with ChaseClosest.stepState(). Comment written there is also applicable to here.
        boolean notReachedYet;
        Position currPos = agent.position;
        Position newPos;
        newPos = new Position(currPos.getX() + stepX, currPos.getY() + stepY);
        if(stepX>0) { // direction in horizontal :: +x or -x ?
            if(stepY>0) { // direction in vertical :: +y or -y?
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
            generateTargetPosition();
        }
        else agent.setPosition(newPos);
    }


    // calculate the size of movement of agent in per cycle (one unit time) in x and y coordinates' directions
    public void calculateStepSize() {
        Position p = agent.position;
        double distance = target.distanceTo(p.getX(), p.getY());
        stepX = ((target.getX() - p.getX()) / distance) * getSpeed();
        stepY = ((target.getY() - p.getY()) / distance) * getSpeed();
    }


    private boolean areSamePoints(Position t) {
        if(target == null) return false;
        else {
            double epsilon = 0.000001;
            if( Math.abs(target.getX() - t.getX()) < epsilon && Math.abs(target.getY() - t.getY()) < epsilon ) {
                return true;
            }
            return false;
        }
    }


    // this func generates a target position inside given boundaries. if the new target is same with previous target,
    // or with current position, recalculates target.
    private void generateTargetPosition() {
        int left, right, bottom, top; // boundaries
        left = 100;
        right = 900;
        top = 100;
        bottom = 550;

        int x = Common.randomBetween(left, right);
        int y = Common.randomBetween(top, bottom);
        Position toBeTarget = new Position(x, y);

        if(areSamePoints(toBeTarget) || areSamePoints(getPosition())) generateTargetPosition();
        else target = toBeTarget;
    }
}
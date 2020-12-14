import java.awt.*;

public class Shake extends State {

    //private Position nextPosition;
    private Position corner1, corner2, corner3, corner4;
    private int vibration;

    public Shake(Agent a) {
        super(a);
        setSpeed(Common.randomBetween(1,3));
        setStateName("Shake");
        vibration = Common.randomBetween(2,5); // stepsize for shake, wavelength
        detectBoundaries();
    }

    //the IA can shake between 4 points, representing a square, which has edgelength = 2 * vibration
    public void detectBoundaries() {
        Position currPos = agent.position;
        corner1 = new Position(currPos.getX() - vibration, currPos.getY() - vibration);
        corner2 = new Position(currPos.getX() + vibration, currPos.getY() - vibration);
        corner3 = new Position(currPos.getX() - vibration, currPos.getY() + vibration);
        corner4 = new Position(currPos.getX() + vibration, currPos.getY() + vibration);
    }



    @Override
    public void drawState(int posX, int posY, Graphics2D g2d) {
        g2d.drawString(getStateName(), posX, posY);
    }

    @Override
    public void stepState() {


        Position currPos = agent.position;
        Position destination;


        // choose between 4 corners of the square randomly
        int destinationChooserFlag = Common.randomBetween(1, 4);
        switch (destinationChooserFlag) {
            case(0):
                destination = new Position(corner1.getX(), corner1.getY());
                break;
            case(1):
                destination = new Position(corner2.getX(), corner2.getY());
                break;
            case(2):
                destination = new Position(corner3.getX(), corner3.getY());
                break;
            default:
                destination = new Position(corner4.getX(), corner4.getY());
                break;
        }

        // and move there.
        agent.setPosition(destination);
    }

}
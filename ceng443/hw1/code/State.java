import java.awt.*;

public abstract class State {
    private int speed; // scalar speed
    private String stateName;
    protected Agent agent;


    public State(Agent a) {
        agent = a;
    }


    public abstract void drawState(int posX, int posY, Graphics2D g2d);
    public abstract void stepState();

    // getters
    public int getSpeed() { return speed; }
    public Position getPosition() {return agent.position;}
    public String getStateName() { return stateName; }

    // setters
    public void setSpeed(int s) { speed = s; }
    public void setStateName(String sn) { stateName = sn; }
}
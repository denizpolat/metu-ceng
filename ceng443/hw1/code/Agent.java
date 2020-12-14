import java.awt.*;

public abstract class Agent extends Entity {

    protected State state;
    private String imageName; // to read from file
    private String name; // to draw in GUI
    protected Graphics2D g2d;
    private int stolenMoney;
    private Country country;
    private char agentUpdate;  // as a switch, or flag, to prevent wrapping with same decorator, e.g. Master(Master(Master(Master...)))



    // default const
    public Agent(double x, double y) {
        super(x, y);
        name = "";
        imageName = "";
        stolenMoney = 0;
        agentUpdate = '0';
    }


    // copy const
    public Agent(Agent agent) {
        super(agent.getPosition().getX(), agent.getPosition().getY());
        state = agent.state;
        name = agent.name;
        imageName = agent.imageName;
        stolenMoney = agent.stolenMoney;
        country = agent.country;
        agentUpdate = '0';
        this.g2d = agent.g2d;
    }



    public abstract void draw(Graphics2D g2d);
    public abstract void step();


    // getters
    public State getState() { return state; }
    public String getImageName() { return imageName; }
    public String getName() { return name; }
    public int getStolenMoney() { return stolenMoney; }
    public Country getCountry() { return country; }
    public char getAgentUpdate() { return agentUpdate; }

    // setters
    public void setState(State s) { state = s; }
    public void setImageName(String in) { imageName = in; }
    public void setName(String n) { name = n; }
    public void setStolenMoney(int s) { stolenMoney = s; }
    public void setCountry(Country c) { country = c; }
    public void setG2d(Graphics2D g) { this.g2d = g; }
    public void setPosition(Position p) { this.position = p; }
    public void setAgentUpdate(char au) { agentUpdate = au; }




}
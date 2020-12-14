import java.awt.*;

public class Master extends AgentDecorator {

    // constructor
    public Master(Agent a) {
        super(a);
        this.setDecoratedAgent(a);
        this.getDecoratedAgent().setAgentUpdate('m');     }


    @Override
    public void draw(Graphics2D g2d) {
        // add yellow badge to @param g2d and pass new version of g2d to
        // inside of this wrap, it will handle the rest as recursion
        // in other words, do what you want to add to @param and let the inner decorator
        // handle the rest
        g2d.setColor(Color.YELLOW);
        g2d.fillRect(getDecoratedAgent().getPosition().getIntX() + 45, getDecoratedAgent().getPosition().getIntY() - 33, 10, 10);
        this.getDecoratedAgent().draw(g2d);
    }

    @Override
    public void step() {
        getDecoratedAgent().step();
    }


    // i needed to override all the functions from super super class Agent, because the core of the decorator pattern is
    // decoratedAgent. by saying func = decoratedAgent.func, we provide all the decorators to "send(?)" the function to
    // one inner circle, until decoratedAgent has no more decorated agents, which is innermost circle (or base case) and
    // apply all the operations that we want to the Agent sitting in the core, wrapped by decorators.


    // getters
    @Override
    public char getAgentUpdate() { return this.getDecoratedAgent().getAgentUpdate(); }

    @Override
    public int getStolenMoney() { return this.getDecoratedAgent().getStolenMoney(); }

    @Override
    public Country getCountry() { return getDecoratedAgent().getCountry(); }

    @Override
    public String getImageName() { return getDecoratedAgent().getImageName(); }

    @Override
    public String getName() { return getDecoratedAgent().getImageName(); }

    @Override
    public State getState() { return getDecoratedAgent().getState(); }

    @Override
    public Position getPosition() { return getDecoratedAgent().getPosition(); }


    // setters

    @Override
    public void setAgentUpdate(char au) { this.getDecoratedAgent().setAgentUpdate(au); }

    @Override
    public void setStolenMoney(int s) { this.getDecoratedAgent().setStolenMoney(s); }

    @Override
    public void setCountry(Country c) { getDecoratedAgent().setCountry(c); }

    @Override
    public void setG2d(Graphics2D g) { getDecoratedAgent().setG2d(g); }

    @Override
    public void setPosition(Position p) { getDecoratedAgent().setPosition(p); }

    @Override
    public void setState(State s) { getDecoratedAgent().setState(s); }

    @Override
    public void setImageName(String in) { getDecoratedAgent().setImageName(in); }

    @Override
    public void setName(String n) { getDecoratedAgent().setName(n); }



}
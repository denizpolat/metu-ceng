import java.awt.*;

public class Novice extends AgentDecorator {

    // master, expert and novice apply the same logic, explanations for all are
    // given in master class.


    public Novice(Agent a) {
        super(a);
        this.setDecoratedAgent(a);
        this.setAgentUpdate('n');
    }


    @Override
    public void draw(Graphics2D g2d) {
        g2d.setColor(Color.WHITE);
        g2d.fillRect(getDecoratedAgent().getPosition().getIntX() + 30, getDecoratedAgent().getPosition().getIntY() - 33, 10, 10);
        this.getDecoratedAgent().draw(g2d);
    }

    @Override
    public void step() {
        getDecoratedAgent().step();
    }



    @Override
    public int getStolenMoney() {
        return this.getDecoratedAgent().getStolenMoney();
    }

    @Override
    public void setStolenMoney(int s) { this.getDecoratedAgent().setStolenMoney(s); }


    @Override
    public void setAgentUpdate(char au) {
        this.getDecoratedAgent().setAgentUpdate(au);
    }

    @Override
    public char getAgentUpdate() { return this.getDecoratedAgent().getAgentUpdate(); }

    @Override
    public Country getCountry() { return getDecoratedAgent().getCountry(); }

    @Override
    public void setCountry(Country c) { getDecoratedAgent().setCountry(c); }

    @Override
    public void setG2d(Graphics2D g) { getDecoratedAgent().setG2d(g); }

    @Override
    public void setPosition(Position p) { getDecoratedAgent().setPosition(p); }

    @Override
    public void setState(State s) { getDecoratedAgent().setState(s); }

    @Override
    public void setImageName(String in) { getDecoratedAgent().setImageName(in);; }

    @Override
    public void setName(String n) { getDecoratedAgent().setName(n); }

    @Override
    public String getImageName() { return getDecoratedAgent().getImageName(); }

    @Override
    public String getName() { return getDecoratedAgent().getImageName(); }

    @Override
    public State getState() { return getDecoratedAgent().getState(); }

    @Override
    public Position getPosition() { return getDecoratedAgent().getPosition(); }




}


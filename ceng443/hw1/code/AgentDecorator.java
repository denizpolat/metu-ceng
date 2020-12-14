import java.awt.*;

public abstract class AgentDecorator extends Agent {

    //protected Graphics2D badge;
    private Agent decoratedAgent; // just like a base case of recursion, hidden inside all of the decorators

    public AgentDecorator(Agent agent) {
        super(agent);
        decoratedAgent = agent;
    }


    public Agent getDecoratedAgent() { return decoratedAgent; }
    public void setDecoratedAgent(Agent a) { decoratedAgent = a; }


}
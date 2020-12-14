import java.awt.*;

public class Rest extends State {

    public Rest(Agent a) {
        super(a);
        setStateName("Rest");
        setSpeed(0);
    }

    @Override
    public void drawState(int posX, int posY, Graphics2D g2d) {
        g2d.drawString(getStateName(), posX, posY);
    }

    @Override
    public void stepState() { }
}
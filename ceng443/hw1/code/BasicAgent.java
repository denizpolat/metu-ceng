import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;


public class BasicAgent extends Agent {

    private int agentWidth, agentHeight;
    private int agentImageWidth, agentImageHeight;
    private BufferedImage image;


    // constructor
    public BasicAgent(double x, double y) {
        super(x, y);
    }


    // constructor
    public BasicAgent(String name, Position pos, String imageName) {
        super(pos.getX(), pos.getY());
        setImageName(imageName);
        setName(name);
        stateChanger();

        agentHeight = 150;
        agentWidth = 120;
        agentImageHeight = 120;
        agentImageWidth = 120;
        try {
            image = ImageIO.read(new File(getImageName()));
            image = Common.resizeImage(image, agentImageWidth, agentImageHeight);

        } catch (IOException e) {
            e.printStackTrace();
        }

    }




    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
        //draw agent image
        g2d.drawImage(image, getPosition().getIntX(), getPosition().getIntY(), null);


        // draw text
        // name black, state blue, cash red
        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("default", Font.BOLD, 16));

        int textX = getPosition().getIntX()+35;
        int textY = getPosition().getIntY()-5;
        g2d.drawString(getName(), textX, textY);


        textY += agentImageHeight+30;
        textX  = getPosition().getIntX()+35;
        g2d.setColor(Color.blue);
        getState().drawState(textX,textY,g2d);

        textX += 10;
        textY += 23;
        g2d.setColor(Color.red);
        g2d.drawString(String.valueOf(getStolenMoney()),textX,textY);
        stateChanger();

    }

    @Override
    public void step() {
        stealOrder(); // check if there is order around IA which could be stolen
        getState().stepState();
    }



    // decorate (((wrap))) agent
    private void updateAgentStatus() {
        int size = Common.getAgents().size();
        for(int i=0; i<size; i++) {
            Agent a = Common.getAgents().get(i);

            // if agent is an expert or isnt able to become novice, continue.
            if(a.getAgentUpdate() == 'e' || a.getStolenMoney() <= 2000) continue;

            // if agent has more than 6000 stolenMoney and is Master currently
            if(a.getStolenMoney() > 6000 && a.getAgentUpdate() == 'm') {
                Common.getAgents().set(i, new Expert(a));
            }

            // if agent has more than 4000 stolenMoney and is Novice currently
            else if((a.getStolenMoney() > 4000) && (a.getAgentUpdate() == 'n')) {
                Common.getAgents().set(i, new Master(a));
            }

            // if agent has more than 2000 stolenMoney and has no badge currently
            else if(a.getStolenMoney() > 2000 && a.getAgentUpdate() == '0') {
                Common.getAgents().set(i, new Novice(a));
            }
        }
    }


    public void stealOrder() {
        double epsilon = (agentImageHeight + agentImageWidth) / 4.;

        // if an order enters the circular area having center = currentAgentCenter && radius = epsilon, it could be
        // stolen

        Position currentAgentCenter = new Position(getPosition().getX() + agentImageWidth / 2.,
                getPosition().getY() + agentImageHeight / 2.);
        ArrayList<Order> orders = new ArrayList<>(Common.getOrders());
        for(Order order : orders) {
            double currDistance = currentAgentCenter.distanceTo(order.getPosition().getX(), order.getPosition().getY());
            if(!order.getHasStolen() && currDistance < epsilon) { // order hasn't stolen by any other agents &&
                order.stolenByAgent(this);                      // order is in the desired area
                Common.getOrders().remove(order);
                updateAgentStatus();
                order.setHasStolen();
            }
        }
    }

    private void stateChanger() {
        int ifFlag = Common.randomBetween(0, 250);
        if(getState() == null || ifFlag == 7) { // change states of IAs randomly. getState will be null only when stateChanger() is called by constructor.
            int stateCase = Common.randomBetween(1, 4);
            switch (stateCase) {
                case(1):
                    if(getState() == null || !(getState().getStateName().equals("Rest")))setState(new Rest(this));
                    break;
                case(2):
                    if(getState() == null || !(getState().getStateName().equals("GotoXY")))setState(new GotoXY(this));
                    break;
                case(3):
                    if(getState() == null || !(getState().getStateName().equals("ChaseClosest")))setState(new ChaseClosest(this));
                    break;
                default:
                    if(getState() == null || !(getState().getStateName().equals("Shake"))) {
                        setState(new Shake(this));
                    }
                    break;
            }
        }
    }
}
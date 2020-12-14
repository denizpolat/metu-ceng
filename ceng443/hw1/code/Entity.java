import java.awt.*;

public abstract class Entity {
    protected Position position;

    public Entity(double x, double y) { position = new Position(x, y); }

    // getter
    public Position getPosition() { return position; }

    public abstract void draw(Graphics2D g2d);
    public abstract void step();


}
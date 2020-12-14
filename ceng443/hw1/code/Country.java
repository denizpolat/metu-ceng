import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class Country extends Entity {
    private String countryName;
    private int gold;
    private int cash;
    private int worth;
    private final int photoWidth;
    private final int photoHeight;
    private String imageName;
    private Graphics2D g2d;
    private BufferedImage image;


    // constructor
    public Country(String cName, double x, double y, String iName) {
        super(x, y);
        countryName = cName;
        gold = 50;
        cash = 10000;
        calculateWorth();
        imageName = iName;
        photoWidth = 120;
        photoHeight = 100;
        // read flag image once in constructor, save it.
        try {
            image = ImageIO.read(new File(imageName));
            image = Common.resizeImage(image, photoWidth, photoHeight);
        }
        catch (IOException | IllegalArgumentException /*| URISyntaxException*/ ioe) {
            ioe.printStackTrace();
        }

    }


    // -----

    // dynamic worth of country
    private void calculateWorth() {
        worth = (int)(cash + Common.getGoldPrice().getCurrentPrice() * gold);
    }





    // generates Order, adds to order list but does not know whether it is buy or sell, because Country cannot see type
    // of Common,getOrderFactory()
    public void generateOrder() {
        Order order = Common.getOrderFactory().createOrder(this);
        order.setPosition(new Position(this.getPosition().getX()+(this.photoWidth/2), this.getPosition().getY()-10));
        Common.getOrders().add(order);
    }



    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
        //draw flag
        g2d.drawImage(image, getPosition().getIntX(), getPosition().getIntY(), null);

        // black name, yellow gold, green cash, blue Worth
        int textx=0, texty=0;

        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("default", Font.BOLD, 18));
        textx = getPosition().getIntX() + 30;
        texty = getPosition().getIntY() + getPhotoHeight() + 30;
        g2d.drawString(countryName, textx, texty);

        textx -= 25;
        texty += 23;
        g2d.setColor(Color.YELLOW);
        g2d.drawString(gold + " gold", textx, texty);

        texty += 23;
        g2d.setColor(Color.GREEN);
        g2d.drawString(cash + " cash", textx, texty);

        texty += 23;
        g2d.setColor(Color.BLUE);
        //worth = calculateWorth(gold.);
        g2d.drawString("Worth: " + worth, textx, texty);

        int random = Common.randomBetween(1, 154); // generate orders random
        if(random == 15) {
            generateOrder();
        }

    }

    @Override
    public void step() {
        calculateWorth();
    }

    // setters

    public void setPosition(Position p) { this.position = p; }

    public void setGold(int gold) { this.gold = gold; }

    public void setCash(int cash) { this.cash = cash; }

    public void setImageName(String name) { this.imageName = name; }

    public void setCountryName(String name) { this.countryName = name; }

    // to simplify addition of cash & gold (instead of setK(getK + amount))

    public void changeCash(int amount) {
        cash += amount;
    }

    public void changeGold(int amount) {
        gold += amount;
    }


    // getters
    public int getPhotoWidth() { return photoWidth; }

    public int getPhotoHeight() { return photoHeight; }

    public int getGold() { return gold; }

    public int getCash() { return cash; }

    public String getImageName() { return imageName; }

    public int getWorth() { return worth; }

    public String getCountryName() { return countryName; }

}
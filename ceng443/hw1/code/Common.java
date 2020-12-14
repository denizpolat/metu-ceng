import java.awt.*; // added
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Common {
    private static final String title = "Gold Wars";
    private static final int windowWidth = 1650;
    private static final int windowHeight = 1000;

    private static final GoldPrice goldPrice = new GoldPrice(588, 62);

    private static final Random randomGenerator = new Random(1234);
    private static final int upperLineY = 100;

    // --------
    //
    private static List<Order> orders;
    private static List<Agent> agents;
    private static List<String> agentPhotos;
    private static List<Country> countries;
    private static List<String> countryPhotos;
    private static String image;


    static  {

        countries = new ArrayList<>();
        agents = new ArrayList<>();
        orders = new ArrayList<>();
        agentPhotos = new ArrayList<>();
        countryPhotos = new ArrayList<>();
        image = "images/";
    }

    // getters
    public static String getTitle() { return title; }
    public static int getWindowWidth() { return windowWidth; }
    public static int getWindowHeight() { return windowHeight; }

    // getter
    public static GoldPrice getGoldPrice() { return goldPrice; }

    // getters
    public static Random getRandomGenerator() { return randomGenerator; }
    public static int getUpperLineY() { return upperLineY; }

    public static void stepAllEntities() {
        if(randomGenerator.nextInt(200) == 0) goldPrice.step();
        for(Country country : countries) {
            country.step();
        }
        for(Order order : orders) {
            order.step();
        }
        removeNecessaryOrders(); // check if any order has hit the upperLineY to remove immediately.
        for(Agent agent : agents) {
            agent.step();
        }
    }
    // -----





    // taken from https://stackoverflow.com/questions/9417356/bufferedimage-resize
    public static BufferedImage resizeImage(BufferedImage bi, int width, int height) {
        BufferedImage resized = new BufferedImage(width, height, BufferedImage.TRANSLUCENT);
        Graphics2D g = resized.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
                RenderingHints.VALUE_INTERPOLATION_BILINEAR);
        g.drawImage(bi, 0, 0, width, height, null);
        g.dispose();
        return resized;
        //BufferedImage bimage = new BufferedImage(width, height, bi.TYPE_INT_RGB);
        //return bimage;
    }


    // in order to make countries dont know type of order they have generated, this function decides whether a
    // buyorderfactory or sellorderfactory will be produced. their explanations are inside related classes.
    public static OrderFactory getOrderFactory() {
        int random = randomBetween(1,2);
        if(random % 2 == 0) return new BuyOrderFactory();
        else return new SellOrderFactory();
    }

    // when an order hits the upperLineY, it needs to be disappeared from scene. This function decides which ones are
    // going to be deleted.
    private static void removeNecessaryOrders() {
        ArrayList<Order> toBeDeleted = new ArrayList<>();
        int count = 0;
        for(Order order : orders) {
            if(order.hasReachedTarget()) toBeDeleted.add(order);
        }
        for(Order order : toBeDeleted) {
            order.executeOrder();
            orders.remove(toBeDeleted.get(count));
            count++;
        }
    }

    // fill static lists, create needed IA objects
    public static void prepareAgents() {
        // prepare the list agentPhotos filled with names of images (with path /images/imageName.xxx)
        agentPhotos.add(new StringBuilder().append(image).append("cia.png").toString());
        agentPhotos.add(new StringBuilder().append(image).append("mossad.png").toString());
        agentPhotos.add(new StringBuilder().append(image).append("mit.png").toString());
        agentPhotos.add(new StringBuilder().append(image).append("svr.png").toString());
        agentPhotos.add(new StringBuilder().append(image).append("mss.png").toString());

        int moveX = 300; // initial distance between IAs
        int posX = 150, posY = 260; // leftmost IA's starting position

        // order ---> cia mossad mit svr mss

        // create needed BasicAgent objects
        Agent agent = new BasicAgent("CIA", new Position(posX, posY), agentPhotos.get(0));
        agent.setCountry(getCountries().get(0));
        agents.add(agent);

        posX += moveX;

        agent = new BasicAgent("MOSSAD", new Position(posX, posY), agentPhotos.get(1));
        agent.setCountry(getCountries().get(1));
        agents.add(agent);

        posX += moveX;

        agent = new BasicAgent("MIT", new Position(posX, posY), agentPhotos.get(2));
        agent.setCountry(getCountries().get(2));
        agents.add(agent);

        posX += moveX;

        agent = new BasicAgent("SVR", new Position(posX, posY), agentPhotos.get(3));
        agent.setCountry(getCountries().get(3));
        agents.add(agent);

        posX += moveX;

        agent = new BasicAgent("MSS", new Position(posX, posY), agentPhotos.get(4));
        agent.setCountry(getCountries().get(4));
        agents.add(agent);


    }

    // fill static lists, create needed Country objects
    public static void prepareCountries() {

        // prepare list of images' names
        countryPhotos.add(new StringBuilder().append(image).append("usa.jpg").toString());
        countryPhotos.add(new StringBuilder().append(image).append("israel.jpg").toString());
        countryPhotos.add(new StringBuilder().append(image).append("turkey.jpg").toString());
        countryPhotos.add(new StringBuilder().append(image).append("russia.jpg").toString());
        countryPhotos.add(new StringBuilder().append(image).append("china.jpg").toString());



        int offset = 90; // stepsize between each country
        int posX = 150, posY = 675; // set position
        int photoWidth, photoHeight;

        // order --> usa israel turkey russia china
        Country country = new Country("USA", posX, posY, countryPhotos.get(0));
        photoWidth = country.getPhotoWidth();
        photoHeight = country.getPhotoHeight();

        posX += 2*offset+photoWidth;
        countries.add(country);

        country = new Country("Israel", posX, posY, countryPhotos.get(1));
        posX += 2*offset+photoWidth;
        countries.add(country);

        country = new Country("Turkey", posX, posY, countryPhotos.get(2));
        posX += 2*offset+photoWidth;
        countries.add(country);

        country = new Country("Russia", posX, posY, countryPhotos.get(3));
        posX += 2*offset+photoWidth;
        countries.add(country);

        country = new Country("China", posX, posY, countryPhotos.get(4));
        posX += 2*offset+photoWidth;
        countries.add(country);


    }


    // return an integer between (min, max)
    public static int randomBetween(int min, int max) { // max inclusive
        if(min == max) return min;
        else if(min > max) {
            int tmp = min;
            min = max;
            max = tmp;
        }
        int random = Math.abs(getRandomGenerator().nextInt());
        int interval = (max - min + 1);
        min %= interval;
        return (random % interval) + min;
    }


    public static void prepareCommon() {
        prepareCountries();
        prepareAgents();
    }


    // getters
    public static List<Order> getOrders() { return orders; }
    public static List<Country> getCountries() { return countries; }
    public static List<Agent> getAgents() { return agents; }



}
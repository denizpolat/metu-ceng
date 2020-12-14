public class BuyOrderFactory extends OrderFactory {

    // Overriden to produce BuyOrder
    @Override
    public Order createOrder(Country c) {
        return new BuyOrder(c);
    }
}
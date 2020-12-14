public class SellOrderFactory extends OrderFactory {

    // SellOrderFactory.createOrder is overriden to produce SellOrder

    @Override
    public Order createOrder(Country c) {
        return new SellOrder(c);
    }
}
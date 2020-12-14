public abstract class OrderFactory {
    // this is overriden in BuyOrderFactory and SellOrderFactory. By doing so, it will be decided whether a BuyOrderFact
    // or SellOrderFact will be produced. This will make Country dont know what type of order it has generated. It will
    // just call getOrderFactory.createOrder
    public abstract Order createOrder(Country c);
}
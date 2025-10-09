import java.util.Random;

public class SenateBusSimulation {
    private static final int maximumCapacity = 50;
    // private static final double busArrivalMean = 20*60*1000;
    private static final double busArrivalMean = 20*1000;
    // private static final double riderArrivalMean = 30*1000;
    private static final double riderArrivalMean = 0.5*1000;

    private static final Random rand = new Random();

    public static void main(String[] args) {
        BusMonitor monitor = new BusMonitor(maximumCapacity);

        Thread busThread = new Thread(() -> {
            int busID = 1;
            while (true) {
                try {
                    Thread.sleep(getExponentialDelay(busArrivalMean));
                    int currentBusID = busID++;
                    new Thread(() -> {
                        try {
                            monitor.arriveBus(currentBusID);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }).start();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });

        Thread riderThread = new Thread(() -> {
            int riderID = 1;
            while (true) {
                try {
                    Thread.sleep(getExponentialDelay(riderArrivalMean));
                    int currentRiderID = riderID++;
                    new Thread(() -> {
                        try {
                            monitor.boardBus(currentRiderID);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }).start();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });

        busThread.start();
        riderThread.start();

    }

    private static long getExponentialDelay(double mean){
        double u = rand.nextDouble();
        return (long)(-mean * Math.log(1 - u));
    }

}

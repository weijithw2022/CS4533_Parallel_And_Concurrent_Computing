import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

// Anyone arrives while the bus is boarding, they must wait for the next bus
// If the bus is full, passengers must wait for the next bus
// If there are enough seats, passengers can board the bus
// If the waiting passengers exceed the bus capacity, only the maximum capacity can board(Given to somewhat prioritize boarding)
// When all passengers have boarded, the bus departs( the ones who couldn't board wait for the next bus)
// If there are no passengers waiting, the bus departs immediately
// This code is written under the assumption that the buses come to the bus halt is empty


class BusMonitor {

    private int riders = 0;
    private int boarded = 0;
    private int maximumCapacity;
    private static final Semaphore mutex = new Semaphore(1);
    private static final Semaphore bus = new Semaphore(0);
    private static final Semaphore multiplex = new Semaphore(50);
    private static final Semaphore allAboard = new Semaphore(0);

    public BusMonitor(int capacity) {
        this.maximumCapacity = capacity;
    }

    public void boardBus(int id) throws InterruptedException {
        multiplex.acquire(); 
        mutex.acquire();
        riders++;
        System.out.println("Passenger " + id + " is waiting to board. Total waiting: " + riders);
        mutex.release();

        bus.acquire(); 

        mutex.acquire();
        riders--;
        boarded++;
        System.out.println("Passenger " + id + " is boarding the bus. Remaining waiting: " + riders);
        // if (riders == 0 || (maximumCapacity - boarded == 0)) {
        //     allAboard.release();
        // }
        if (riders == 0 || boarded == maximumCapacity) {
            allAboard.release();
        }

        mutex.release();
        multiplex.release(); 
    }

    public void arriveBus(int busID) throws InterruptedException {
        System.out.println("Bus " + busID + " has arrived.");

        mutex.acquire();
        int passengersToBoard = Math.min(riders, maximumCapacity);
        if (passengersToBoard == 0) {
            System.out.println("No passengers waiting. Bus " + busID + " is departing immediately.");
            mutex.release();
            return;
        }

        // CyclicBarrier barrier = new CyclicBarrier(passengersToBoard + 1);
        System.out.println("Bus " + busID + " ready to board " + passengersToBoard + " passengers.");

        bus.release(passengersToBoard); 
        mutex.release();

        allAboard.acquire(); 

        // Wait at barrier until all boarding threads reach it
        // try {
        //     barrier.await(); // The bus is one of the participants
        // } catch (Exception e) {
        //     e.printStackTrace();
        // }
        depart(busID);
    }

    private void depart(int busID) {
        System.out.println("Bus " + busID + " departing with " + boarded + " riders.");
        boarded = 0;
    }


}
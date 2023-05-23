import java.util.LinkedList;

class Buffer {
    private LinkedList<Integer> buffer;
    private int capacity;

    public Buffer(int capacity) {
        this.buffer = new LinkedList<>();
        this.capacity = capacity;
    }

    public synchronized void produce(int item) throws InterruptedException {
        while (buffer.size() == capacity) {
            wait();
        }

        buffer.add(item);
        System.out.println("Produced: " + item);
        notifyAll();
    }

    public synchronized int consume() throws InterruptedException {
        while (buffer.isEmpty()) {
            wait();
        }

        int item = buffer.removeFirst();
        System.out.println("Consumed: " + item);
        notifyAll();
        return item;
    }
}

class Producer implements Runnable {
    private Buffer buffer;
    private int itemsToProduce;

    public Producer(Buffer buffer, int itemsToProduce) {
        this.buffer = buffer;
        this.itemsToProduce = itemsToProduce;
    }

    public void run() {
        try {
            for (int i = 0; i < itemsToProduce; i++) {
                buffer.produce(i);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class Consumer implements Runnable {
    private Buffer buffer;
    private int itemsToConsume;

    public Consumer(Buffer buffer, int itemsToConsume) {
        this.buffer = buffer;
        this.itemsToConsume = itemsToConsume;
    }

    public void run() {
        try {
            for (int i = 0; i < itemsToConsume; i++) {
                buffer.consume();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Buffer buffer = new Buffer(3);

        Producer producer = new Producer(buffer, 30);
        Consumer consumer = new Consumer(buffer, 30);

        Thread producerThread = new Thread(producer);
        Thread consumerThread = new Thread(consumer);

        producerThread.start();
        consumerThread.start();

        try {
            producerThread.join();
            consumerThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

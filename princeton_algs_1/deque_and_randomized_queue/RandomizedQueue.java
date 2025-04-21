import java.util.Iterator;
import java.util.NoSuchElementException;

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class RandomizedQueue<Item> implements Iterable<Item> {
    private class RandomizedIterator implements Iterator<Item> {
        int currentIndex = 0;
        private int[] shuffledIndices;

        RandomizedIterator() {
            shuffledIndices = new int[size()];
            for (int i = 0; i < size(); i++) {
                shuffledIndices[i] = i;
            }
            StdRandom.shuffle(shuffledIndices);
        }

        public boolean hasNext() {
            return currentIndex < size();
        }

        public Item next() {
            if (!hasNext()) {
                throw new NoSuchElementException("RandomizedQueue iterator is at end position.");
            }

            return arr[shuffledIndices[currentIndex++]];
        }

        public void remove() {
            throw new UnsupportedOperationException("RandomizedQueue iterator does not support remove.");
        }
    }

    private int size, capacity;
    private Item[] arr;

    private void resize(int newCapacity) {
        Item[] newArr = (Item[]) new Object[newCapacity];
        for (int i = 0; i < size(); i++) {
            newArr[i] = arr[i];
        }
        arr = newArr;
        capacity = newCapacity;
    }

    public RandomizedQueue() {
        size = 0;
        resize(4);
    }

    public boolean isEmpty() {
        return size() <= 0;
    }

    public int size() {
        return size;
    }

    public void enqueue(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot insert null into RandomizedQueue.");
        }

        if (size >= capacity) {
            resize(capacity * 2);
        }

        arr[size++] = item;
    }

    public Item dequeue() {
        if (isEmpty()) {
            throw new NoSuchElementException("RandomizedQueue is empty.");
        }

        int randomlySelectedIndex = StdRandom.uniformInt(size);
        Item payload = arr[randomlySelectedIndex];

        // swap randomly selected element with last element in array
        arr[randomlySelectedIndex] = arr[size - 1];
        arr[--size] = null;

        if (size * 4 <= capacity && capacity / 2 >= 4) {
            resize(capacity / 2);
        }

        return payload;
    }

    public Item sample() {
        if (isEmpty()) {
            throw new NoSuchElementException("RandomizedQueue is empty.");
        }

        return arr[StdRandom.uniformInt(size)];
    }

    public Iterator<Item> iterator() {
        return new RandomizedIterator();
    }

    // Testing all public constructors and methods of RandomizedQueue
    public static void main(String[] args) {
        RandomizedQueue<Integer> randomizedQueue = new RandomizedQueue<>();

        StdOut.println("\nInitial empty check:");
        StdOut.println("Is randomized queue empty: " + randomizedQueue.isEmpty());
        StdOut.println("Randomized queue size: " + randomizedQueue.size());
        StdOut.println();

        StdOut.println("Inserting `1` to randomized queue.");
        randomizedQueue.enqueue(1);
        StdOut.println("Is randomized queue empty: " + randomizedQueue.isEmpty());
        StdOut.println("Randomized queue size: " + randomizedQueue.size());
        StdOut.println();

        StdOut.println("Result on dequeue: " + randomizedQueue.dequeue());
        StdOut.println("Is randomized queue empty: " + randomizedQueue.isEmpty());
        StdOut.println("Randomized queue size: " + randomizedQueue.size());
        StdOut.println();

        StdOut.println("Attempting to dequeue from empty randomized queue:");
        try {
            randomizedQueue.dequeue();
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (NoSuchElementException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        StdOut.println("Adding test values into randomized queue: { 1, 2, 3, 4, 5, 6, 7 }");
        for (int i : new int[]{ 1, 2, 3, 4, 5, 6, 7 }) {
            randomizedQueue.enqueue(i);
        }
        StdOut.print("Iterating through the randomized queue: ");
        for (int i : randomizedQueue) {
            StdOut.print(i + ", ");
        }
        StdOut.print("\nIterating again with a different iterator object: ");
        for (int i : randomizedQueue) {
            StdOut.print(i + ", ");
        }
        StdOut.println("\n");

        StdOut.println("Attempting to remove from randomized queue iterator.");
        try {
            randomizedQueue.iterator().remove();
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (UnsupportedOperationException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        StdOut.println("Attempting to insert null value into randomized queue:");
        try {
            randomizedQueue.enqueue(null);
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (IllegalArgumentException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        for (int i = 0; i < 5; i++) {
            StdOut.println("Result on dequeue: " + randomizedQueue.dequeue());
        }
        StdOut.println("Is randomized queue empty: " + randomizedQueue.isEmpty());
        StdOut.println("Randomized queue size: " + randomizedQueue.size());
        for (int i = 0; i < 2; i++) {
            StdOut.println("Result on dequeue: " + randomizedQueue.dequeue());
        }
        StdOut.println("Is randomized queue empty: " + randomizedQueue.isEmpty());
        StdOut.println("Randomized queue size: " + randomizedQueue.size());
        StdOut.println();
    }
}

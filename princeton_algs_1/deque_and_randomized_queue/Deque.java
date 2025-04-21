import java.util.Iterator;
import java.util.NoSuchElementException;

import edu.princeton.cs.algs4.StdOut;

public class Deque<Item> implements Iterable<Item> {
    private class LinkedListNode {
        Item payload;
        LinkedListNode next, previous;

        LinkedListNode(Item payload, LinkedListNode next, LinkedListNode previous) {
            this.payload = payload;
            this.next = next;
            this.previous = previous;
        }
    }

    private class ListIterator implements Iterator<Item> {
        LinkedListNode current = firstNode;

        public boolean hasNext() {
            return current != null;
        }

        public Item next() {
            if (!hasNext()) {
                throw new NoSuchElementException("Deque iterator is at end position.");
            }

            Item payload = current.payload;
            current = current.next;
            return payload;
        }

        public void remove() {
            throw new UnsupportedOperationException("Deque iterator does not support remove.");
        }
    }

    private int size;
    private LinkedListNode firstNode, lastNode;

    public Deque() {
        size = 0;
        firstNode = lastNode = null;
    }

    public boolean isEmpty() {
        return size() <= 0;
    }

    public int size() {
        return size;
    }

    public void addFirst(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot insert null into Deque.");
        }

        firstNode = new LinkedListNode(item, firstNode, null);
        size++;

        if (lastNode == null) {
            lastNode = firstNode;
        } else {
            firstNode.next.previous = firstNode;
        }
    }

    public void addLast(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot insert null into Deque.");
        }

        if (lastNode == null) {
            addFirst(item);
        } else {
            lastNode.next = new LinkedListNode(item, null, lastNode);
            lastNode = lastNode.next;
            size++;
        }
    }

    public Item removeFirst() {
        if (isEmpty()) {
            throw new NoSuchElementException("Deque is empty.");
        }

        Item payload = firstNode.payload;

        firstNode = firstNode.next;
        if (firstNode == null) {
            lastNode = null;
        } else {
            firstNode.previous = null;
        }
        size--;

        return payload;
    }

    public Item removeLast() {
        if (isEmpty()) {
            throw new NoSuchElementException("Deque is empty.");
        }

        Item payload = lastNode.payload;

        lastNode = lastNode.previous;
        if (lastNode == null) {
            firstNode = null;
        } else {
            lastNode.next = null;
        }
        size--;

        return payload;
    }

    public Iterator<Item> iterator() {
        return new ListIterator();
    }

    // Testing all public constructors and methods of Deque
    public static void main(String[] args) {
        Deque<Integer> deque = new Deque<>();

        StdOut.println("\nInitial empty check:");
        StdOut.println("Is deque empty: " + deque.isEmpty());
        StdOut.println("Deque size: " + deque.size());
        StdOut.println();

        StdOut.println("Inserting `1` to front.");
        deque.addFirst(1);
        StdOut.println("Is deque empty: " + deque.isEmpty());
        StdOut.println("Deque size: " + deque.size());
        StdOut.println();

        StdOut.println("Result on removing last item: " + deque.removeLast());
        StdOut.println("Is deque empty: " + deque.isEmpty());
        StdOut.println("Deque size: " + deque.size());
        StdOut.println();

        StdOut.println("Attempting to remove first item from empty deque:");
        try {
            deque.removeFirst();
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (NoSuchElementException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        StdOut.println("Adding test values into deque: { 1, 2, 3, 4, 5, 6, 7 }");
        for (int i : new int[]{ 1, 2, 3, 4, 5, 6, 7 }) {
            deque.addLast(i);
        }
        StdOut.print("Result of iterating through the deque: ");
        for (int i : deque) {
            StdOut.print(i + ", ");
        }
        StdOut.println("\n");

        StdOut.println("Attempting to remove from deque iterator.");
        try {
            deque.iterator().remove();
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (UnsupportedOperationException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        StdOut.println("Attempting to insert null value into deque:");
        try {
            deque.addFirst(null);
            StdOut.println("If you are seeing this, the program has failed!");
        } catch (IllegalArgumentException e) {
            StdOut.println("Successfully caught error " + e);
        }
        StdOut.println();

        for (int i = 0; i < 5; i++) {
            if (i % 2 == 0) {
                StdOut.println("Result on remove first: " + deque.removeFirst());
            } else {
                StdOut.println("Result on remove last: " + deque.removeLast());
            }
        }
        StdOut.println("Is deque empty: " + deque.isEmpty());
        StdOut.println("Deque size: " + deque.size());
        for (int i = 0; i < 2; i++) {
            if (i % 2 == 0) {
                StdOut.println("Result on remove first: " + deque.removeFirst());
            } else {
                StdOut.println("Result on remove last: " + deque.removeLast());
            }
        }
        StdOut.println("Is deque empty: " + deque.isEmpty());
        StdOut.println("Deque size: " + deque.size());
        StdOut.println();
    }
}

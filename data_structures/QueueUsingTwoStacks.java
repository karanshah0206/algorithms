// Problem:
// Implement a queue data structure that internally uses two stack data structures.
// Ensure that enqueue and dequeue operations are constant amortized time.
//
// Solution:
// The idea is to designate one stack to store enqueued values and another for values to be dequeued.
// Enqueued values in the queue are directly pushed into enqueue stack, thereby ensuring constant time.
// When dequeuing, pop from the dequeue stack and return. If the dequeue stack is empty, pop all items
// from the enqueue stack and push them into the dequeue stack. This means that dequeuing is linear-time
// in the worst case, but constant amortized, because all values to be dequeued are pushed into and popped
// from the dequeue stack at most once.
//
// Just for fun, I've manually implemented the stack data structure here as well, which internally uses
// a linked list for its own operations.

import java.util.NoSuchElementException;

public class QueueUsingTwoStacks<T> {
    private class LinkedListStack {
        private class LinkedListNode {
            private T payload;
            private LinkedListNode next;

            LinkedListNode(T payload, LinkedListNode next) {
                this.payload = payload;
                this.next = next;
            }
        }

        private int itemsCounter = 0;
        private LinkedListNode topMostNode = null;

        void push(T payload) {
            topMostNode = new LinkedListNode(payload, topMostNode);
            itemsCounter++;
        }

        T pop() {
            if (topMostNode == null) {
                throw new IndexOutOfBoundsException();
            }

            T payload = topMostNode.payload;
            topMostNode = topMostNode.next;
            itemsCounter--;
            return payload;
        }

        boolean isEmpty() {
            return size() <= 0;
        }

        int size() {
            return itemsCounter;
        }
    }

    private LinkedListStack enqueueStack = new LinkedListStack(), dequeueStack = new LinkedListStack();

    public boolean isEmpty() {
        return enqueueStack.isEmpty() && dequeueStack.isEmpty();
    }

    public int size() {
        return enqueueStack.size() + dequeueStack.size();
    }

    public void enqueue(T payload) {
        enqueueStack.push(payload);
    }

    public T dequeue() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty.");
        }

        if (dequeueStack.isEmpty()) {
            while (!enqueueStack.isEmpty()) {
                dequeueStack.push(enqueueStack.pop());
            }
        }

        return dequeueStack.pop();
    }
}

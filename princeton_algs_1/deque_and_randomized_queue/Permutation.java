import java.util.Iterator;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class Permutation {
    public static void main(String[] args) {
        if (args.length < 1) {
            throw new IllegalArgumentException("Expected a numerical argument but received none.");
        }

        try {
            int k = Integer.parseInt(args[0]);
            if (k == 0) {
                return;
            }

            int counter = 0;

            RandomizedQueue<String> rq = new RandomizedQueue<>();

            while (!StdIn.isEmpty()) {
                String s = StdIn.readString();

                if (counter++ < k) {
                    rq.enqueue(s);
                } else {
                    if (StdRandom.uniformInt(counter) < k) {
                        rq.dequeue();
                        rq.enqueue(s);
                    }
                }
            }

            Iterator<String> iter = rq.iterator();
            for (int i = 0; i < k; i++) {
                StdOut.println(iter.next());
            }
        } catch (NumberFormatException e) {
            StdOut.println("Cannot parse argument " + args[0] + " as integer.");
        }
    }
}

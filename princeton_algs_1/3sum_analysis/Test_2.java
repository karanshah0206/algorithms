import edu.princeton.cs.algs4.Stopwatch;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.In;
import java.util.Arrays;

public class Test_2 {
    public static int count(int[] a) {
        int N = a.length;
        int count = 0;

        Arrays.sort(a);

        for (int i = 0; i < N - 2; i++) {
            int j = i + 1, k = N - 1;
            while (j < k) {
                int sum = a[i] + a[j] + a[k];
                if (sum == 0) count++;
                if (sum <= 0) j++;
                else k--;
            }
        }

        return count;
    }

    public static void main(String[] args) {
        int[] a = In.readInts(args[0]);
        Stopwatch stopwatch = new Stopwatch();
        StdOut.println(count(a));
        double time = stopwatch.elapsedTime();
        System.out.println(time);
    }
}

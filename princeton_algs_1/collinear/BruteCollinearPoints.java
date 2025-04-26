import java.util.ArrayList;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

public class BruteCollinearPoints {
    private final ArrayList<LineSegment> lineSegments;

    /**
     * The BruteCollinearPoints class examines 4 points at a time and checks whether they all lie
     * on the same line segment. It identifies all such line segments containing exactly 4 points.
     *
     * @param points the array of points to be checked for collinearity
     * @throws IllegalArgumentException if the input array is null, contains null elements, or contains duplicate points
     */
    public BruteCollinearPoints(Point[] points) {
        if (points == null) {
            throw new IllegalArgumentException("Cannot pass null as points array.");
        }

        for (int i = 0; i < points.length; i++) {
            if (points[i] == null) {
                throw new IllegalArgumentException("Points array cannot contain null elements.");
            }
        }

        lineSegments = new ArrayList<LineSegment>();

        for (int i = 0; i < points.length; i++) {
            for (int j = 0; j < points.length; j++) {
                if (j == i) {
                    continue;
                }

                if (points[i].compareTo(points[j]) == 0) {
                    throw new IllegalArgumentException("Points array cannot contain duplicate points.");
                }

                if (points[i].compareTo(points[j]) < 0) {
                    for (int k = 0; k < points.length; k++) {
                        if (k == i || k == j) {
                            continue;
                        }

                        if (points[k].compareTo(points[i]) == 0 || points[k].compareTo(points[j]) == 0) {
                            throw new IllegalArgumentException("Points array cannot contain duplicate points.");
                        }

                        if (points[j].compareTo(points[k]) < 0) {
                            for (int m = 0; m < points.length; m++) {
                                if (m == i || m == j || m == k) {
                                    continue;
                                }

                                if (points[m].compareTo(points[i]) == 0 || points[m].compareTo(points[j]) == 0 || points[m].compareTo(points[k]) == 0) {
                                    throw new IllegalArgumentException("Points array cannot contain duplicate points.");
                                }

                                if (points[k].compareTo(points[m]) < 0 && points[i].slopeTo(points[j]) == points[j].slopeTo(points[k]) && points[j].slopeTo(points[k]) == points[k].slopeTo(points[m])) {
                                    lineSegments.add(new LineSegment(points[i], points[m]));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Returns the number of line segments found.
     *
     * @return the number of line segments
     */
    public int numberOfSegments() {
        return lineSegments.size();
    }

    /**
     * Returns in an array the line segments passing through identified coolinear point groups.
     *
     * @return an array of line segments passing through identified collinear point groups
     */
    public LineSegment[] segments() {
        return lineSegments.toArray(new LineSegment[0]);
    }

    // test client
    public static void main(String[] args) {
        In in = new In(args[0]);
        int n = in.readInt();

        Point[] points = new Point[n];
        for (int i = 0; i < n; i++) {
            int x = in.readInt();
            int y = in.readInt();
            points[i] = new Point(x, y);
        }

        // draw the points
        StdDraw.enableDoubleBuffering();
        StdDraw.setXscale(0, 32768);
        StdDraw.setYscale(0, 32768);
        for (Point p : points) {
            p.draw();
        }
        StdDraw.show();

        // print and draw the line segments
        BruteCollinearPoints collinear = new BruteCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}

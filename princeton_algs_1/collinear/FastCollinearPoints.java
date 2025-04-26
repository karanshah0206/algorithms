import java.util.ArrayList;
import java.util.Arrays;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

public class FastCollinearPoints {
    private final ArrayList<LineSegment> lineSegments;

    /**
     * The FastCollinearPoints class is used to find all maximal line segments containing 4 or more points
     * that are collinear in a given set of points. It achieves this by sorting points based on the slopes
     * they make with a reference point.
     *
     * @param points the array of points to be checked for collinearity
     * @throws IllegalArgumentException if the input array is null, contains null elements, or contains duplicate points
     */
    public FastCollinearPoints(Point[] points) {
        if (points == null) {
            throw new IllegalArgumentException("Cannot pass null as points array.");
        }

        for (Point point : points) {
            if (point == null) {
                throw new IllegalArgumentException("Points array cannot contain null elements.");
            }
        }

        Point[] myPoints = points.clone();

        Arrays.sort(myPoints);
        for (int i = 0; i < myPoints.length - 1; i++) {
            if (myPoints[i].compareTo(myPoints[i + 1]) == 0) {
                throw new IllegalArgumentException("Points array cannot contain duplicate points.");
            }
        }

        lineSegments = new ArrayList<LineSegment>();

        for (int i = 0; i < myPoints.length; i++) {
            Arrays.sort(myPoints, myPoints[i].slopeOrder());

            double slopeWithLastMeasuredItem = myPoints[0].slopeTo(myPoints[0]);
            int firstItemOfThisSlopeMeasure = 0;

            for (int j = 1; j < myPoints.length; j++) {
                double currentSlopeMeasure = myPoints[0].slopeTo(myPoints[j]);

                if (currentSlopeMeasure != slopeWithLastMeasuredItem) {
                    if (myPoints[firstItemOfThisSlopeMeasure].compareTo(myPoints[0]) > 0 && j - firstItemOfThisSlopeMeasure >= 3) {
                        lineSegments.add(new LineSegment(myPoints[0], myPoints[j - 1]));
                    }

                    slopeWithLastMeasuredItem = currentSlopeMeasure;
                    firstItemOfThisSlopeMeasure = j;
                }
            }

            if (myPoints[firstItemOfThisSlopeMeasure].compareTo(myPoints[0]) > 0 && myPoints.length - firstItemOfThisSlopeMeasure >= 3) {
                lineSegments.add(new LineSegment(myPoints[0], myPoints[myPoints.length - 1]));
            }

            Arrays.sort(myPoints);
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
        FastCollinearPoints collinear = new FastCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}

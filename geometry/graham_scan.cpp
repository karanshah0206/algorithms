#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

const double EPS = 1e-10; // used to circumvent precision issues

// custom type for points on a 2d plane
struct point_2d {
    double x, y;

    bool operator==(const point_2d &other) const {
        return std::fabs(x - other.x) < EPS && std::fabs(y - other.y) < EPS;
    }

    double dist_squared(const point_2d &other) const {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }

    double cross(const point_2d &point_before, const point_2d &point_after) const {
        return (x - point_before.x) * (point_after.y - point_before.y) - (y - point_before.y) * (point_after.x - point_before.x);
    }
};

int main() {
    // get number of points
    size_t n;
    std::cin >> n;

    // get points list
    std::vector<point_2d> points(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> points[i].x >> points[i].y;
    }

    // remove duplicate points
    std::sort(points.begin(), points.end(), [](const point_2d &a, const point_2d &b) {
        if (std::fabs(a.x - b.x) > EPS) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });
    points.erase(std::unique(points.begin(), points.end(), [](const point_2d &a, const point_2d &b) { return a == b; }), points.end());
    n = points.size();

    if (n <= 2) {
        // trivial answer if under 3 unique points
        std::cout << n << "\n";
        for (const point_2d &point : points) {
            std::cout << point.x << " " << point.y << "\n";
        }
    } else {
        // find pivot point (bottom-most right-most point)
        size_t pivot_index = 0;
        for (size_t i = 0; i < n; i++) {
            if (points[i].y < points[pivot_index].y - EPS || (std::fabs(points[i].y - points[pivot_index].y) < EPS && points[i].x > points[pivot_index].x + EPS)) {
                pivot_index = i;
            }
        }

        // put pivot point at array start and sort rest of the array
        std::swap(points[0], points[pivot_index]);
        std::sort(points.begin() + 1, points.end(), [&](const point_2d &a, const point_2d &b) {
            double cross_product = a.cross(points[0], b);

            if (std::fabs(cross_product) < EPS) {
                // collinear: nearer first
                return a.dist_squared(points[0]) < b.dist_squared(points[0]);
            }

            // not collinear: descending cross product gives counterclockwise order
            return cross_product > EPS;
        });

        // Graham scan through all points, keeping only those that turn counterclockwise.
        std::vector<size_t> hull_indices;
        hull_indices.push_back(0);
        hull_indices.push_back(1);
        for (size_t i = 2; i < n; i++) {
            while (hull_indices.size() >= 2) {
                size_t top_index = hull_indices[hull_indices.size() - 1];
                size_t next_to_top_index = hull_indices[hull_indices.size() - 2];
                if (points[top_index].cross(points[next_to_top_index], points[i]) > EPS) {
                    // counterclockwise ordering maintained, keep point
                    break;
                }
                // breaks counterclockwise ordering, remove point
                hull_indices.pop_back();
            }
            hull_indices.push_back(i);
        }
        
        std::cout << hull_indices.size() << "\n";
        for (size_t point_index : hull_indices) {
            std::cout << points[point_index].x << " " << points[point_index].y << "\n";
        }
    }
}

class KDTree:
    def __init__(self, points):
        """Initialise a static KDTree with a sequence of points, all of identical dimensionality.

        Args:
            points (Sequence[Tuple[float, ...]]): A sequence of points that make up the KDTree.

        Raises:
            ValueError: If no points are passed or if all points do not have the same dimensionality.
        """
        if not points:
            raise ValueError("At least one point is required to create KDTree.")

        self.k = len(points[0])
        for point in points:
            if len(point) != self.k:
                raise ValueError("Not all points are of identical dimensionality.")
        self.root = self._build_kd_tree(points)

    class _KDTreeNode:
        def __init__(self, point, axis):
            self.point = point
            self.axis = axis
            self.coord_on_axis = point[axis]
            self.left = None
            self.right = None

    def _build_kd_tree(self, points, depth = 0):
        if not points:
            return None

        axis = depth % self.k
        sorted_points = sorted(points, key = lambda p: p[axis])

        median_idx = len(sorted_points) // 2
        median_point = sorted_points[median_idx]

        node = self._KDTreeNode(median_point, axis)
        node.left = self._build_kd_tree(sorted_points[:median_idx], depth + 1)
        node.right = self._build_kd_tree(sorted_points[median_idx + 1:], depth + 1)

        return node

    def _dist_sq(self, point1, point2):
        return sum((x - y) ** 2 for x, y in zip(point1, point2))

    def _nearest_neighbour(self, query_point, best, current):
        if current is None:
            return best

        next_best = None
        next_branch = None
        other_branch = None

        if best is None or self._dist_sq(query_point, current.point) < self._dist_sq(query_point, best.point):
            next_best = current
        else:
            next_best = best

        if query_point[current.axis] < current.coord_on_axis:
            next_branch = current.left
            other_branch = current.right
        else:
            next_branch = current.right
            other_branch = current.left

        best = self._nearest_neighbour(query_point, next_best, next_branch)
        if self._dist_sq(query_point, best.point) > abs(query_point[current.axis] - current.coord_on_axis) ** 2:
            best = self._nearest_neighbour(query_point, best, other_branch)

        return best

    def _points_in_orthotope(self, lower_bound_coords, upper_bound_coords, current, result):
        if current is None:
            return

        within_orth = True

        for i in range(self.k):
            if current.point[i] < lower_bound_coords[i] or current.point[i] > upper_bound_coords[i]:
                within_orth = False
                break

        if within_orth:
            result.append(current.point)

        if lower_bound_coords[current.axis] <= current.coord_on_axis:
            self._points_in_orthotope(lower_bound_coords, upper_bound_coords, current.left, result)

        if upper_bound_coords[current.axis] >= current.coord_on_axis:
            self._points_in_orthotope(lower_bound_coords, upper_bound_coords, current.right, result)

    def nearest_neighbour(self, query_point):
        """Find a single nearest neighbour within the KDTree to a given query point.

        Args:
            query_point (Tuple[float, ...]): Coordinates of the query point.

        Returns:
            Tuple[float, ...] or None: A single nearest point, or None if tree is empty.

        Raises:
            ValueError: If query point does not have the same dimension as points in the KDTree.
        """
        if len(query_point) != self.k:
            raise ValueError(f"Query point is of different dimension ({len(query_point)}) to points in the KDTree ({self.k}).")

        result = self._nearest_neighbour(query_point, None, self.root)
        return result if result is None else result.point
 
    def points_in_orthotope(self, lower_bound_coords, upper_bound_coords):
        """Retrieve all points within an orthotope (axis-aligned hyperrectangle).

        Args:
            lower_bound_coords (Sequence[float]): Smallest coordinate per axis of search.
            upper_bound_coords (Sequence[float]): Largest coordinate per axis of search.

        Returns:
            List[Tuple[float, ...]]: List of points within orthotope.

        Raises:
            ValueError: If the number of cordinates in lower/upper bound sequence is different to KDTree's dimensionality.
        """
        if len(lower_bound_coords) != self.k:
            raise ValueError(f"Number of lower bound coordinates ({len(lower_bound_coords)}) does not equal KDTree's dimensionality ({self.k}).")

        if len(upper_bound_coords) != self.k:
            raise ValueError(f"Number of upper bound coordinates ({len(upper_bound_coords)}) does not equal KDTree's dimensionality ({self.k}).")

        result = []
        self._points_in_orthotope(lower_bound_coords, upper_bound_coords, self.root, result)
        return result

def main():
    points = [(3, 3), (4, 2), (2, 4), (1, 1), (6, 6), (5, 2)]
    kd_tree = KDTree(points)

    query_point = (9, 2)
    nearest_neighbour = kd_tree.nearest_neighbour(query_point)
    print(f"Nearest Nighbour to {str(query_point)}: \033[91m{str(nearest_neighbour)}\033[0m")

    lower_coords = (4, 2)
    upper_coords = (14, 16)
    range_search_result = kd_tree.points_in_orthotope(lower_coords, upper_coords)
    print(f"Points In Range {lower_coords} -> {upper_coords}: \033[91m{str(range_search_result)}\033[0m")

if __name__ == "__main__":
    main()

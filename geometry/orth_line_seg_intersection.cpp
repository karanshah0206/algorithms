/*
 * Count the number of orthogonal line intersections.
 * Assumes no degenerate "point" segments (all line segments have non-zero length).
 * Assumes all coordinates are integer pairs.
 * Assumes no overlapping horizontal line segments.
 * Vertices are counted as intersections.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <functional>

struct line {
    int x_start, y_start, x_end, y_end;

    line(int x_start, int y_start, int x_end, int y_end) {
        this->x_start = x_start;
        this->y_start = y_start;
        this->x_end = x_end;
        this->y_end = y_end;
    }
};

enum line_event_type {
    HORIZONTAL_START,
    VERTICAL,
    HORIZONTAL_END,
};

struct line_event {
    int x_coord;
    line *l;
    line_event_type type;

    line_event(line &l, line_event_type type) {
        this->l = &l;
        this->type = type;
        x_coord = (type == HORIZONTAL_END) ? l.x_end : l.x_start;
    }

    bool operator>(const line_event &other) const {
        if (x_coord == other.x_coord) {
            return type > other.type;
        }

        return x_coord > other.x_coord;
    }
};

int main() {
    std::vector<line> lines;
    lines.emplace_back(0, 0, 5, 0);
    lines.emplace_back(0, 5, 5, 5);
    lines.emplace_back(0, 0, 0, 5);
    lines.emplace_back(5, 0, 5, 5);
    lines.emplace_back(-1, 2, 1, 2);

    std::priority_queue<line_event, std::vector<line_event>, std::greater<line_event>> events;
    for (line &l : lines) {
        if (l.x_start == l.x_end) {
            events.push(line_event(l, line_event_type::VERTICAL));
        } else {
            events.push(line_event(l, line_event_type::HORIZONTAL_START));
            events.push(line_event(l, line_event_type::HORIZONTAL_END));
        }
    }

    int intersections_counter = 0;

    std::set<int> active_y_coords;
    while (!events.empty()) {
        const line_event &event = events.top();
        if (event.type == HORIZONTAL_START) {
            active_y_coords.insert(event.l->y_start);
        } else if (event.type == HORIZONTAL_END) {
            active_y_coords.erase(event.l->y_end);
        } else {
            int smaller_y_coord = std::min(event.l->y_start, event.l->y_end);
            int bigger_y_coord = std::max(event.l->y_start, event.l->y_end);
            auto range_start = active_y_coords.lower_bound(smaller_y_coord);
            auto range_end = active_y_coords.upper_bound(bigger_y_coord);
            intersections_counter += std::distance(range_start, range_end);
        }
        events.pop();
    }

    std::cout << "Number of intersections: " << intersections_counter << "\n";
}

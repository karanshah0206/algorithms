public class Event implements Comparable<Event> {
    private final double timeOfEventOccurrence;
    private final Particle a, b;
    private final int aCollisionsCount, bCollisionsCount;

    public Event(Particle a, Particle b, double timeOfEventOccurrence) {
        this.a = a;
        this.b = b;
        this.timeOfEventOccurrence = timeOfEventOccurrence;
        aCollisionsCount = (a == null ? 0 : a.getCollisionsCount());
        bCollisionsCount = (b == null ? 0 : b.getCollisionsCount());
    }

    public Particle getA() {
        return a;
    }

    public Particle getB() {
        return b;
    }

    public double getTimeOfEventOccurrence() {
        return timeOfEventOccurrence;
    }

    public int compareTo(Event other) {
        if (timeOfEventOccurrence < other.timeOfEventOccurrence) return -1;
        if (timeOfEventOccurrence == other.timeOfEventOccurrence) return 0;
        return 1;
    }

    public boolean isValid() {
        return !((a != null && aCollisionsCount != a.getCollisionsCount()) || (b != null && bCollisionsCount != b.getCollisionsCount()));
    }
}

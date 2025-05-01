import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.MinPQ;

public class CollisionSystem {
    private final Particle[] particles;
    private final MinPQ<Event> minPQ;

    private double currentTime;

    public CollisionSystem(Particle[] particles) {
        this.particles = particles;
        currentTime = 0.;
        minPQ = new MinPQ<Event>();
    }

    public void simulate() {
        for (Particle particle : particles) predict(particle);
        minPQ.insert(new Event(null, null, 0));

        while (!minPQ.isEmpty()) {
            Event event = minPQ.delMin();
            if (!event.isValid()) continue;

            Particle a = event.getA(), b = event.getB();

            for (Particle particle : particles) particle.move(event.getTimeOfEventOccurrence() - currentTime);
            currentTime = event.getTimeOfEventOccurrence();

            if (a != null && b != null) {
                a.collideWithParticle(b);
                predict(a);
                predict(b);
            } else if (a != null) {
                a.collideWithVerticalWall();
                predict(a);
            } else if (b != null) {
                b.collideWithHorizontalWall();
                predict(b);
            } else {
                StdDraw.clear();
                for (Particle particle : particles) particle.draw();
                StdDraw.show(5);
                minPQ.insert(new Event(null, null, currentTime + 0.07));
            }
        }
    }

    private void predict(Particle particle) {
        if (particle == null) return;

        double verticleWallCollisionTime = particle.predictVerticleWallCollisionTime();
        if (verticleWallCollisionTime != Double.POSITIVE_INFINITY)
            minPQ.insert(new Event(particle, null, currentTime + verticleWallCollisionTime));

        double horizontalWallCollisionTime = particle.predictHorizontalWallCollisionTime();
        if (horizontalWallCollisionTime != Double.POSITIVE_INFINITY)
            minPQ.insert(new Event(null, particle, currentTime + horizontalWallCollisionTime));

        for (Particle other : particles) {
            double particleCollisionTime = particle.predictParticleCollisionTime(other);
            if (particleCollisionTime != Double.POSITIVE_INFINITY)
                minPQ.insert(new Event(particle, other, currentTime + particleCollisionTime));
        }
    }

    // Test client, pass a number as CLI argument when invoking to specify number of particles
    public static void main(String[] args) {
        int N = Integer.parseInt(args[0]);
        Particle[] particles = new Particle[N];
        for (int i = 0; i < N; i++) particles[i] = new Particle();
        CollisionSystem collisionSystem = new CollisionSystem(particles);
        collisionSystem.simulate();
    }
}

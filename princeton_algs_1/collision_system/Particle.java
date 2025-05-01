import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdRandom;

public class Particle {
    private final double radius, mass;
    private double posX, posY, velX, velY;
    private int collisionsCount;

    public Particle() {
        radius = StdRandom.uniformDouble(0.007, 0.05);
        mass = radius * 1000;
        // mass = 0.5;
        posX = StdRandom.uniformDouble(radius, 1. - radius);
        posY = StdRandom.uniformDouble(radius, 1. - radius);
        velX = StdRandom.uniformDouble(0.0, 0.07) * (StdRandom.uniformDouble() > 0.5 ? 1 : -1);
        velY = StdRandom.uniformDouble(0.0, 0.07) * (StdRandom.uniformDouble() > 0.5 ? 1 : -1);
        collisionsCount = 0;
    }

    public void move(double timeDelta) {
        posX += velX * timeDelta;
        posY += velY * timeDelta;
    }

    public void draw() {
        StdDraw.filledCircle(posX, posY, radius);
    }

    public void collideWithParticle(Particle other) {
        double dPosX = other.posX - posX, dPosY = other.posY - posY;
        double dVelX = other.velX - velX, dVelY = other.velY - velY;

        double dVelDotDPos = dPosX*dVelX + dPosY*dVelY;
        double netRadius = radius + other.radius;

        double j = 2 * mass * other.mass * dVelDotDPos / ((mass + other.mass) * netRadius);
        double jX = j * dPosX / netRadius;
        double jY = j * dPosY / netRadius;

        velX += jX / mass;
        velY += jY / mass;
        other.velX -= jX / other.mass;
        other.velY -= jY / other.mass;

        collisionsCount++;
        other.collisionsCount++;
    }

    public void collideWithVerticalWall() {
        velY *= -1;
        posY = velY > 0 ? radius : 1. - radius;
        collisionsCount++;
    }

    public void collideWithHorizontalWall() {
        velX *= -1;
        posX = velX > 0 ? radius : 1. - radius;
        collisionsCount++;
    }

    public int getCollisionsCount() {
        return collisionsCount;
    }

    public double predictParticleCollisionTime(Particle other) {
        if (this == other) return Double.POSITIVE_INFINITY;

        double dVelX = other.velX - velX, dVelY = other.velY - velY;
        double dPosX = other.posX - posX, dPosY = other.posY - posY;

        double dVelDotDPos = dVelX * dPosX + dVelY * dPosY;
        if (dVelDotDPos > 0) return Double.POSITIVE_INFINITY;

        double dVelSquared = dVelX * dVelX + dVelY * dVelY;
        double dPosSquared = dPosX * dPosX + dPosY * dPosY;

        double d = dVelDotDPos * dVelDotDPos - dVelSquared * (dPosSquared - (radius + other.radius) * (radius + other.radius));
        if (d < 0) return Double.POSITIVE_INFINITY;

        return -(dVelDotDPos + Math.sqrt(d)) / dVelSquared;
    }

    public double predictVerticleWallCollisionTime() {
        if (velY == 0) return Double.POSITIVE_INFINITY;
        else if (velY > 0) return (1. - posY - radius) / velY;
        else return -(posY - radius) / velY;
    }

    public double predictHorizontalWallCollisionTime() {
        if (velX == 0) return Double.POSITIVE_INFINITY;
        else if (velX > 0) return (1. - posX - radius) / velX;
        else return -(posX - radius) / velX;
    }
}

import static org.junit.jupiter.api.Assertions.assertEquals;

class VehicleTest {
    Vehicle vehicle = null;

    @org.junit.jupiter.api.BeforeEach
    void setUp() {
        vehicle = new Vehicle(10, "south");
        vehicle.setSpeed(10);
    }

    @org.junit.jupiter.api.AfterEach
    void tearDown() {
        vehicle.finalize();
    }

    @org.junit.jupiter.api.Test
    void testFinalize() {
        vehicle.finalize();
        assertEquals(0, Vehicle.totalVehicle());
        vehicle = new Vehicle();
        assertEquals(1, Vehicle.totalVehicle());
    }

    @org.junit.jupiter.api.Test
    void setSpeed() {
        vehicle.setSpeed(50);
        assertEquals(50, vehicle.getSpeed());
    }

    @org.junit.jupiter.api.Test
    void setDir() {
        vehicle.setDir("east");
        assertEquals("east", vehicle.getDir());
    }

    @org.junit.jupiter.api.Test
    void getSpeed() {
        assertEquals(10, vehicle.getSpeed());
    }

    @org.junit.jupiter.api.Test
    void getDir() {
        assertEquals("south",vehicle.getDir());
    }

    @org.junit.jupiter.api.Test
    void totalVehicle() {
        vehicle = new Vehicle();
        vehicle = new Vehicle(20, "east");
        assertEquals(3, Vehicle.totalVehicle());
    }
}
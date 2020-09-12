
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import org.junit.Before;
import org.junit.Test;


public class BoundedQueueTest {

    BoundedQueue boundedQueue;

    @Before
    public void setUp() throws Exception {
        boundedQueue = new BoundedQueue(2); //BoundedQueue_C1_T
    }

    @Test(expected=IllegalArgumentException.class)
    public void BoundedQueue_C1_F() {
        boundedQueue = new BoundedQueue(-1);
    }

    @Test(expected = IllegalStateException.class)
    public void enQueue_C1C2C3_TTT () {
        boundedQueue.enQueue(6);
        boundedQueue.enQueue(5);
        boundedQueue.enQueue(4);
    }


    @Test(expected=NullPointerException.class)
    public void enQueue_C1C2C3_TFT () {
        assertEquals(true, boundedQueue.isEmpty());
        assertEquals(false, boundedQueue.isFull());
        boundedQueue.enQueue((Object) 5);
        boundedQueue.enQueue(null);
        boundedQueue.enQueue(6);
    }

    @Test(expected=NullPointerException.class)
    public void enQueue_C1C2C3_TTF () {
        assertEquals(true, boundedQueue.isEmpty());
        boundedQueue.enQueue((Object) 5);
        boundedQueue.enQueue(6);
        boundedQueue.enQueue(null);
        assertEquals(true, boundedQueue.isFull());
    }

    @Test
    public void enQueue_C1C2C4_TTT (){
        boundedQueue.enQueue(1);
        boundedQueue.deQueue();
    }

    @Test(expected=NullPointerException.class)
    public void enQueue_C1C2C4_TFT () {
        boundedQueue.enQueue((Object) 5);
        boundedQueue.enQueue(null);
        boundedQueue.deQueue();
    }

    @Test(expected=IllegalStateException.class)
    public void enQueue_C1C2C4_TTF () {
        assertEquals(true, boundedQueue.isEmpty());
        boundedQueue.enQueue((Object) 5);
        boundedQueue.deQueue();
        boundedQueue.deQueue();
        assertEquals(true, boundedQueue.isEmpty());
    }

}

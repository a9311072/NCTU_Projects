import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

public class CalTest {
    private Cal cal;

    /**
    (a) How many mutants are there? 243

    (b) How many test cases do you need to kill the non-equivalent mutants?  243-26 = 217

    (c) What mutation score were you able to achieve before analyzing for equivalent mutants? 86%

    (d) How many equivalent mutants are there?  26
    **/


    @Before
    public void setUp() throws Exception {
        cal = new Cal();
    }

    @After
    public  void tearDown() throws Exception {
        cal = null ;
    }

    @Test
    public void Test_1() {
        assertEquals(29, cal.cal(2,1,3,1,-404));
        assertEquals(29, cal.cal(2,1,1,1,-404));
        assertEquals(28, cal.cal(2,1,3,1,-402));
        assertEquals(28, cal.cal(2,1,1,1,-402));
    }

    @Test
    public void Test_2() {
        assertEquals(0, cal.cal(0,0,0,0,0));
        assertEquals(31, cal.cal(5,31,4,31,0));
        assertEquals(1, cal.cal(5,31,4,1,0));
        assertEquals(61, cal.cal(12,1,4,31,0));
        assertEquals(31, cal.cal(12,1,4,1,0));

        assertEquals(31, cal.cal(5,31,4,31,4000));
        assertEquals(1, cal.cal(5,31,4,1,4000));
        assertEquals(61, cal.cal(12,1,4,31,4000));
        assertEquals(31, cal.cal(12,1,4,1,4000));

        assertEquals(31, cal.cal(5,31,4,31,4004));
        assertEquals(1, cal.cal(5,31,4,1,4004));
        assertEquals(61, cal.cal(12,1,4,31,4004));
        assertEquals(31, cal.cal(12,1,4,1,4004));

        assertEquals(31, cal.cal(5,31,4,31,4001));
        assertEquals(1, cal.cal(5,31,4,1,4001));
        assertEquals(61, cal.cal(12,1,4,31,4001));
        assertEquals(31, cal.cal(12,1,4,1,4001));

        //assertEquals(996, cal.cal(0,-999,-999,-3,-999999));
        //assertEquals(-8994, cal.cal(0,-999,-999,-9993,-999999));
        //assertEquals(-8994, cal.cal(0,-999,-999,-9993,-999995));

        assertEquals(366, cal.cal(0,0,12,31,0));
        assertEquals(365, cal.cal(0,0,12,31,1));
        assertEquals(365, cal.cal(0,0,12,31,2));
        assertEquals(365, cal.cal(0,0,12,31,3));
        assertEquals(366, cal.cal(0,0,12,31,4));

        assertEquals(31, cal.cal(0,0,1,31,0));
        assertEquals(31, cal.cal(0,0,1,31,1));
        assertEquals(31, cal.cal(0,0,1,31,2));
        assertEquals(31, cal.cal(0,0,1,31,3));
        assertEquals(31, cal.cal(0,0,1,31,4));

        assertEquals(31, cal.cal(0,0,-1,31,0));
        assertEquals(31, cal.cal(0,0,-1,31,1));
        assertEquals(31, cal.cal(0,0,-1,31,2));
        assertEquals(31, cal.cal(0,0,-1,31,3));
        assertEquals(31, cal.cal(0,0,-1,31,4));

        assertEquals(61, cal.cal(1,1,-1,31,0));
        assertEquals(61, cal.cal(1,1,-1,31,1));
        assertEquals(61, cal.cal(1,1,-1,31,2));
        assertEquals(61, cal.cal(1,1,-1,31,3));
        assertEquals(61, cal.cal(1,1,-1,31,4));

        assertEquals(30, cal.cal(4,0,0,0,0));
        assertEquals(30, cal.cal(4,0,0,0,1));
        assertEquals(30, cal.cal(4,0,0,0,2));
        assertEquals(30, cal.cal(4,0,0,0,3));
        assertEquals(30, cal.cal(4,0,0,0,4));
        assertEquals(30, cal.cal(4,0,0,0,5));

        assertEquals(31, cal.cal(5,0,0,0,0));
        assertEquals(31, cal.cal(3,0,0,0,1));
        assertEquals(31, cal.cal(3,0,0,0,2));
        assertEquals(31, cal.cal(3,0,0,0,3));
        assertEquals(31, cal.cal(3,0,0,0,4));
        assertEquals(31, cal.cal(3,0,0,0,5));

        assertEquals(28, cal.cal(2,1,3,1,-401));
        assertEquals(29, cal.cal(2,1,3,1,-404));
        assertEquals(57, cal.cal(0,-1,3,-3,-402));
        assertEquals(58, cal.cal(0,-1,3,-3,-404));

        assertEquals(-2, cal.cal(0,-1,-3,-3,-404));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-402));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-403));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-401));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-400));

        assertEquals(-2, cal.cal(0,-1,-3,-3,-104));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-102));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-103));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-101));
        assertEquals(-2, cal.cal(0,-1,-3,-3,-100));

        assertEquals(89, cal.cal(1,-1,4,-3,-4));
        assertEquals(88, cal.cal(1,-1,4,-3,-5));
        assertEquals(63, cal.cal(1,-1,2,31,-4));
        assertEquals(63, cal.cal(1,-1,2,31,-5));
        assertEquals(-30, cal.cal(2,-1,2,-31,-4));
        assertEquals(-30, cal.cal(2,-1,2,-31,-5));
        assertEquals(-30, cal.cal(2,-1,2,-31,-2));
        assertEquals(-30, cal.cal(2,-1,2,-31,-1));
        assertEquals(-30, cal.cal(2,-1,2,-31,0));

        assertEquals(-3, cal.cal(2,1,-2,-31,-4));
        assertEquals(-4, cal.cal(2,1,-2,-31,-5));
        assertEquals(-4, cal.cal(2,1,-2,-31,-2));
        assertEquals(-4, cal.cal(2,1,-2,-31,-1));
        assertEquals(-3, cal.cal(2,1,-2,-31,0));
    }

    @Test
    public void Test_3() {


        assertEquals(123, cal.cal(0,1,5,3,40000));
        assertEquals(122, cal.cal(0,1,5,3,40001));
        assertEquals(122, cal.cal(0,1,5,3,40002));
        assertEquals(122, cal.cal(0,1,5,3,40003));
        assertEquals(123, cal.cal(0,1,5,3,40004));

        assertEquals(123, cal.cal(0,1,5,3,4000));
        assertEquals(122, cal.cal(0,1,5,3,4001));
        assertEquals(122, cal.cal(0,1,5,3,4002));
        assertEquals(122, cal.cal(0,1,5,3,4003));
        assertEquals(123, cal.cal(0,1,5,3,4004));

        assertEquals(123, cal.cal(0,1,5,3,400));
        assertEquals(122, cal.cal(0,1,5,3,401));
        assertEquals(122, cal.cal(0,1,5,3,402));
        assertEquals(122, cal.cal(0,1,5,3,403));
        assertEquals(123, cal.cal(0,1,5,3,404));

        assertEquals(30, cal.cal(2,1,2,31,99));
        assertEquals(30, cal.cal(2,1,2,31,98));
        assertEquals(30, cal.cal(2,1,2,31,97));
        assertEquals(30, cal.cal(2,1,2,31,96));
        assertEquals(30, cal.cal(2,1,2,31,95));

        assertEquals(30, cal.cal(2,1,2,31,100));
        assertEquals(30, cal.cal(2,1,2,31,101));
        assertEquals(30, cal.cal(2,1,2,31,102));
        assertEquals(30, cal.cal(2,1,2,31,103));
        assertEquals(30, cal.cal(2,1,2,31,104));
        assertEquals(30, cal.cal(2,1,2,31,0));
        assertEquals(30, cal.cal(2,1,2,31,1));
        assertEquals(30, cal.cal(2,1,2,31,2));
        assertEquals(30, cal.cal(2,1,2,31,3));
        assertEquals(30, cal.cal(2,1,2,31,4));

        assertEquals(364, cal.cal(1,1,12,31,100));
        assertEquals(364, cal.cal(1,1,12,31,200));
        assertEquals(364, cal.cal(1,1,12,31,300));
        assertEquals(365, cal.cal(1,1,12,31,400));
        assertEquals(364, cal.cal(1,1,12,31,500));

        assertEquals(364, cal.cal(1,1,12,31,600));
        assertEquals(364, cal.cal(1,1,12,31,700));
        assertEquals(365, cal.cal(1,1,12,31,800));
        assertEquals(364, cal.cal(1,1,12,31,900));
        assertEquals(364, cal.cal(1,1,12,31,1000));
        assertEquals(364, cal.cal(1,1,12,31,1100));
        assertEquals(365, cal.cal(1,1,12,31,1200));

        assertEquals(364, cal.cal(1,1,12,31,501));
        assertEquals(364, cal.cal(1,1,12,31,503));
        assertEquals(364, cal.cal(1,1,12,31,505));
        assertEquals(364, cal.cal(1,1,12,31,507));
        assertEquals(364, cal.cal(1,1,12,31,509));
        assertEquals(364, cal.cal(1,1,12,31,511));
    }

}

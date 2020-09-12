import org.junit.Before;
import org.junit.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ThermostatTest {
    Thermostat thermo;
    ProgrammedSettings pset;

    @Before
    public void SetupParameters(){
        thermo = new Thermostat();
        pset = new ProgrammedSettings();

        pset.setSetting (Period.MORNING, DayType.WEEKDAY, 69);
        thermo.setPeriod (Period.MORNING);
        thermo.setDay (DayType.WEEKDAY);
        thermo.setThresholdDiff (5);
        thermo.setMinLag (10);
    }

    @Test
    public void TTFT() {
        thermo.setCurrentTemp (61);     // a is true
        thermo.setOverride (true);      // b is true
        thermo.setOverTemp (67);        // c is false
        thermo.setTimeSinceLastRun (11); // d is true
        assertEquals(true, thermo.turnHeaterOn(pset));
    }

    @Test
    public void FTFT(){
        thermo.setCurrentTemp (65);     // a is false
        thermo.setOverride (true);      // b is true
        thermo.setOverTemp (67);        // c is false
        thermo.setTimeSinceLastRun (11); // d is true
        assertEquals(false, thermo.turnHeaterOn(pset));
    }

    @Test
    public void FTTT(){
        thermo.setCurrentTemp (65);     // a is false
        thermo.setOverride (true);      // b is tru
        thermo.setOverTemp (72);        // c is true
        thermo.setTimeSinceLastRun (11); // d is true
        assertEquals(true, thermo.turnHeaterOn(pset));
    }

    @Test
    public void FFTT(){
        thermo.setCurrentTemp (68);     // a is false
        thermo.setOverride (false);     // b is false
        thermo.setOverTemp (72);        // c is true
        thermo.setTimeSinceLastRun (11);// d is true
        assertEquals(false, thermo.turnHeaterOn(pset));
    }
    @Test
    public void TTTT(){
        thermo.setCurrentTemp (61);     // a is true
        thermo.setOverride (true);      // b is true
        thermo.setOverTemp (72);        // c is true
        thermo.setTimeSinceLastRun (11);// d is true
        assertEquals(true, thermo.turnHeaterOn(pset));
    }
    @Test
    public void TTTF(){
        thermo.setCurrentTemp (61);     // a is true
        thermo.setOverride (true);      // b is true
        thermo.setOverTemp (72);        // c is true
        thermo.setTimeSinceLastRun (8); // d is false
        assertEquals(false, thermo.turnHeaterOn(pset));
    }
}

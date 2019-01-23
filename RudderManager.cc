/************************************************************
* RudderManager.cc
*
* Berechnung der benötigten Ruderausschläge aus der errechneten
* wahren Kurs-Abweichung.
*
* Feedback des Rudder Angel über Potenziometer an analogem Eingang.
* Je länger der Ruder-Weg, desto schneller wird angefahren.
*
* 2012 (C) Jörg Pauly
*
**************************************************************/
#include RudderManager.h

void rudderAngelManager(void)
{
    Msg_WriteText("Rudersensor gestartet!\r");
    // Laden der Einstellwerte
    rudderPosBb         = EEPROM_ReadWord(MEMBBD);
    rudderPosNeutral    = EEPROM_ReadWord(MEMNEU);
    rudderPosStb        = EEPROM_ReadWord(MEMSTB);
    Msg_WriteText("Einstellungen geladen!\r");

    word leftRange;
    leftRange = rudderPosNeutral - rudderPosBb;

    word rightRange;
    rightRange = rudderPosStb - rudderPosNeutral;

    float bbRef;
    bbRef = 100.0 / leftRange;

    float stbRef;
    stbRef = 100.0 / rightRange;
    Msg_WriteText("Skalen für Messbereich erstellt!\r");

    word raw;
    Port_DataDirBit(46,1);
    Port_WriteBit(46,1);

    while(1)
    {
        raw=GetAdc(RUDDERINADC);

        if(raw < rudderPosNeutral)
        {
            RA = 0 - ((rudderPosNeutral - raw) * bbRef);
        }

        if(raw > rudderPosNeutral)
        {
            RA = ((raw - rudderPosNeutral) * stbRef);
        }

        if(raw == rudderPosNeutral)
        {
            RA = 0;
        }

        Msg_WriteInt(RA);
        Msg_WriteText("\r");
        Thread_Delay(1);
    }
}

word GetAdc(byte Channel)
{
    ADC_Set(ADC_VREF_VCC,Channel);
    return ADC_Read();
}

void calculateSteering(void)
{
    /* Berechnen des zu stellenden Ruderausschlages */
    int absxta;
    int cca;
    cca = 0;
    char lr;
    lr = 'r';

    if(xta < 0)
    {
        absxta = 0 - xta;
        lr = 'l';
    }
    else
    {
        absxta = xta;
        lr = 'r';
    }

    if(absxta > 0 && absxta <= 5)
    {
        cca = 5;
    }

    if(absxta > 5 && absxta <= 10)
    {
        cca = 10;
    }

    if(absxta > 10 && absxta <= 30)
    {
        cca = 25;
    }

    if(absxta > 30 && absxta <= 50)
    {
        cca = 50;
    }

    if(absxta > 50 && absxta <= 75)
    {
        cca = 75;
    }

    if(absxta > 75)
    {
        cca = 100;
    }

    if(lr == 'l')
    {
        cca = 0 - cca;
    }

    steerAngel = cca;
}



/*******************************************
*                                          *
*  Neue Routinen für die PWM-Ansteuerung   *
*                                          *
*******************************************/

void rudderManager(void)
{
    /* Thread-Einsprung */
    while(1)
    {
        naviManager();

        if(RA != steerAngel)
        {
            driveUpRudder(0 - RA + steerAngel);
        }
        else
        {
            driveUpRudder(0);
        }
    }
}


void driveUpRudder(int speed)
{
    /* Hochfahren des Ruderstellers je nach Abstand */
    /* Bei 0 alles stoppen */
    if(speed == 0)
    {
        DriverCutOff();
        return;
    }

    /* Speed auf min. 20 % setzen */
    if(speed < 0 && speed > -20)
    {
        speed = -20;
    }

    if(speed > 0 && speed < 20)
    {
        speed = 20;
    }

    /* Speed auf 100 % begrenzen */
    if(speed < -100)
    {
        speed = -100;
    }

    if(speed > 100)
    {
        speed = 100;
    }

    dra = speed; /* für's Monitoring */


    /* Speed in Ticks/Pulse umwandeln */
    speed = speed * 1440;

    if(speed < 0)
    {
        // Zuerst alles stoppen, sonst Kurzschluss!!!
        DriverCutOff();
        // Wert invertieren und Ansteuerung nach Bb auf B.5
        Timer_T1PWM(15, -speed, PS_1024);
    }
    else
    {
        // Zuerst alles stoppen, sonst Kurzschluss!!!
        DriverCutOff();
        // Ansteuerung nach StB auf E.3
        Timer_T3PWM(15, speed, PS_1024);
    }
}

/* Abstellen aller konfigurierten PWM-Ausgänge! */
void DriverCutOff(void)
{
    Timer_Disable(1);
    Port_DataDirBit(15,1);
    Port_WriteBit(15,0);
    Timer_Disable(3);
    Port_DataDirBit(35,1);
    Port_WriteBit(35,0);
    Thread_Delay(50);
}

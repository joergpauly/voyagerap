/******************************************************************************
*
* NaviManager.cc
*
* Berechnung eines Wertes zur Kurskorrektur aus den NMEA-Daten.
* Ergebnis: Der Winkel (XTA), um den zu drehen ist. Dieser wird von
* calculateSteering() zur Bestimmung des Ruderausschlages verwendet.
*
* 2012 (C) Jörg Pauly
*
*******************************************************************************/

void naviManager(void)
{
    calculateXta();
    calculateSteering();
}

void calculateXta(void)
{
    switch(Mode)
    {
        case MOD_LEV:
            calcAerialXta();
            break;

        case MOD_LIN:
            calcLineXta();
            break;

        case MOD_HED:
            calcHdgXta();
            break;

        case MOD_SBY:
            // Nichts machen...
            break;
    }
}

void calcAerialXta(void)
{
    float l_cts;
    float l_cog;
    float l_xta;
    l_cts = htw; // Heading to Waypoint (Soll-Kurs) sichern, z.B. 020
    l_cog = cog; // Course over Ground (Ist-Kurs) sichern, z.B. 355

    if(l_cts > l_cog) // Soll-Kurs > Ist-Kurs: 020 > 355 = NEIN
    {
        if((l_cts - l_cog) > 180)
        {
            l_cog = l_cog + 360;
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta + calcTC();
        }
        else
        {
            l_xta = l_cts - l_cog;
            xta = l_xta + calcTC();
        }
    }

    if(l_cog > l_cts) // Ist-Kurs > Soll-Kurs: 355 > 20 = JA
    {
        if((l_cog - l_cts) > 180) // 335 = JA = rechts steuern
        {
            l_cts = l_cts + 360;  // 20 + 360 = 380
            l_xta = l_cts - l_cog; // 380 - 355 = 25
            xta = l_xta + calcTC();
        }
        else // wenn nein, dann links steuern
        {
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta + calcTC();
        }
    }
}

void calcLineXta(void)
{
    float l_cts;
    float l_cog;
    float l_xta;
    l_cts = cts;
    l_cog = cog;

    if(l_cts > l_cog)
    {
        if((l_cts - l_cog) > 180)
        {
            l_cog = l_cog + 360;
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta + calcTC();
        }
        else
        {
            l_xta = l_cts - l_cog;
            xta = l_xta + calcTC();
        }
    }

    if(l_cog > l_cts)
    {
        if((l_cog - l_cts) > 180)
        {
            l_cts = l_cts + 360;
            l_xta = l_cts - l_cog;
            xta = l_xta + calcTC();
        }
        else
        {
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta + calcTC();
        }
    }
}


void calcHdgXta(void)
{
    float l_cts;
    float l_cog;
    float l_xta;
    l_cts = hdg;
    l_cog = cog;

    if(l_cts > l_cog)
    {
        if((l_cts - l_cog) > 180)
        {
            l_cog = l_cog + 360;
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta;
        }
        else
        {
            l_xta = l_cts - l_cog;
            xta = l_xta;
        }
    }

    if(l_cog > l_cts)
    {
        if((l_cog - l_cts) > 180)
        {
            l_cts = l_cts + 360;
            l_xta = l_cts - l_cog;
            xta = l_xta;
        }
        else
        {
            l_xta = l_cog - l_cts;
            xta = 0 - l_xta;
        }
    }
}


int calcTC(void)
{
    int l_xte;
    int tc;
    tc = 0;

    l_xte = xte;

    if(l_xte < -5)
    {
        tc = 10;
    }

    if(l_xte < -20)
    {
        tc = 25;
    }

    if(l_xte > 5)
    {
        tc = -10;
    }

    if(l_xte > 20)
    {
        tc = -25;
    }

    return tc;
}


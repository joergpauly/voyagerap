/********************************************************************
*
* NMEAListener.cc
*
* Auslesen und Parsen der NMEA-Sentences und setzen der Variabelen.
*
* 2012 (C) Jörg Pauly
*
*********************************************************************/


void initSerial(void)
{
    Serial_Init_IRQ(0, inBuffer, 128, 64, SR_8BIT|SR_1STOP|SR_NO_PAR, SR_BD19200);
    //Kontroll-LEDs initialisieren
    Port_DataDirBit(51,1);
    Port_DataDirBit(52,1);
}

void listenNMEA(void)
{
    Msg_WriteText("NMEA-Listener gestartet!\r");
    initSerial();
    Msg_WriteText("NMEA-Interface gestartet!\r");

    while(1)
    {
        readSentence();
        parseSentence();
    }
}

void readSentence(void)
{
    char s_byte;
    int strpos;
    strpos = 0;

    while(s_byte != 0x0a)
    {
        s_byte = Serial_ReadExt(0);

        if(s_byte != 0x00)
        {
            Port_WriteBit(51,0);
            sentence[strpos] = s_byte;
            strpos++;
            Port_WriteBit(51,1);
        }
    }

    sentence[strpos + 1] = 0x00;
}


void parseSentence(void)
{
    /* NMEA-Satz zerlegen und Variabelen setzen.
    *  Zunächst stellen wir fest, um welchen Satz es sich handelt
    *  und rufen dann die zuständige Parser-Prozedur auf. */


    if(sentence[0] == 0x00) // Satz ist leer, raus hier...
    {
        Port_WriteBit(52,1);
        return;
    }

    //Parse-LED an
    Port_WriteBit(52,0);

    Msg_WriteText(sentence);
    Msg_WriteText("\r");

    char SID[4];
    int i;

    parse2array();

    for(i=0;i<3;i++)
    {
        SID[i] = sentence[i+3];
    }

    SID[3] = 0;

    if(!Str_Comp(SID, "APB"))
    {
        parseAPB();
    }

    if(!Str_Comp(SID, "RMC"))
    {
        parseRMC();
    }

    Port_WriteBit(52,1);
}

void parseAPB(void)
{
    cts = Str_ReadFloat(valBuf09);
    btw = Str_ReadFloat(valBuf12);
    htw = Str_ReadFloat(valBuf14);
    setXTE();
}

void parseRMC(void)
{
    cog = Str_ReadFloat(valBuf09);
}


void parse2array(void)
{
    /* Teilt den String in ein Array auf. */
    int is;
    int it;
    is = 0;
    it = 0;

    if(sentence[0] == 0x00)
    {
        return;
    }

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf01[it] = sentence[is];
        is++;
        it++;
    }

    valBuf01[it] = 0;

    if(sentence[is] == 0x0a)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf02[it] = sentence[is];
        is++;
        it++;
    }

    valBuf02[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf03[it] = sentence[is];
        is++;
        it++;
    }

    valBuf03[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf04[it] = sentence[is];
        is++;
        it++;
    }

    valBuf04[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf05[it] = sentence[is];
        is++;
        it++;
    }

    valBuf05[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf06[it] = sentence[is];
        is++;
        it++;
    }

    valBuf06[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf07[it] = sentence[is];
        is++;
        it++;
    }

    valBuf07[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf08[it] = sentence[is];
        is++;
        it++;
    }

    valBuf08[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf09[it] = sentence[is];
        is++;
        it++;
    }

    valBuf09[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf10[it] = sentence[is];
        is++;
        it++;
    }

    valBuf10[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf11[it] = sentence[is];
        is++;
        it++;
    }

    valBuf11[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf12[it] = sentence[is];
        is++;
        it++;
    }

    valBuf12[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf13[it] = sentence[is];
        is++;
        it++;
    }

    valBuf13[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf14[it] = sentence[is];
        is++;
        it++;
    }

    valBuf14[it] = 0;

    if(sentence[is] == 0x0d)
    {
        return;
    }

    is++;
    it = 0;

    while(sentence[is] != ',' && sentence[is] != 0x0d)
    {
        valBuf15[it] = sentence[is];
        is++;
        it++;
    }

    valBuf15[it] = 0;
    is++;
    it = 0;
}

void setXTE(void)
{
    l_xte = Str_ReadFloat(valBuf04);
    l_xte = round(l_xte * 1852);

    if(!Str_Comp(valBuf05, "R"))
    {
        l_xte = 0 - l_xte;
    }

    if(Mode == MOD_LIN)
    {
        xte = keepXTrack - l_xte;
    }
    else
    {
        xte = l_xte;
    }
}
/******************************************************************************
*
*  PanelManager.cc
*
*  Liest Voreinstellungen aus dem Bedien-Panel aus
*
*  2012 (C) Jörg Pauly
*
******************************************************************************/

#include PanelManager.h

char str[9];

void panelManager(void)
{


    Msg_WriteText("PanelManager gestartet!\r");

    while(1)
    {
        scanKey();
        if(Key != 0)
        {
            computeKey();
        }
        Thread_Delay(10);
    }
}




void splash(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Voyager");
    LCD_CursorPos(0x40);
    LCD_WriteText("Autohelm");
    Thread_Delay(200);
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Version");
    LCD_CursorPos(0x40);
    LCD_WriteText(Version);
    Thread_Delay(200);
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("(C) 2015");
    LCD_CursorPos(0x40);
    LCD_WriteText("J. Pauly");
    Thread_Delay(200);
}

void scanKey(void)
{
    word raw;
    raw = Key_Scan();
    if(raw != 0)
    {
        Key = Key_TranslateKey(raw);
    }
    else
    {
        Key = 0;
    }
}

void computeKey(void)
{
    Msg_WriteText("State: %4d\r");
    switch(Key)
    {
        case FORWARD:
            switch(state)
            {
                case SETTINGS:
                    state = MODE;
                    break;

                case MOD_SBY:
                    state = MOD_LEV;
                    break;

                case MON_CRD:
                    state = MON_XTE;
                    break;

                case SET_RUD:
                    state = SET_RUD;
                    break;

                case MOD_LIN_XTE:
                    keepXTrack = keepXTrack + 5;
                    break;

                case MOD_HED_MAN:
                    state = MOD_HED_ACT;
                    break;

                case MOD_HED_MAN_EDT:
                    increaseHdg(10);
                    break;

                case SET_RUD_STB:
                    state = SET_RUD_BBD;
                    break;

                default:
                    state++;
            }
            setState();
            break;

        case BACK:
            switch(state)
            {
                case MODE:
                    state = SETTINGS;
                    break;

                case MOD_LEV:
                    state = MOD_SBY;
                    break;

                case MON_XTE:
                    state = MON_CRD;
                    break;

                case MOD_LIN_XTE:
                    keepXTrack = keepXTrack - 5;
                    break;

                case MOD_HED_ACT:
                    state = MOD_HED_MAN;
                    break;

                case MOD_HED_MAN_EDT:
                    decreaseHdg(10);
                    break;

                case SET_RUD_BBD:
                    state = SET_RUD_STB;
                    break;

                default:
                    state--;
            }
            setState();
            break;

        case ENTER:
            if(state < 1000)
            {
                state = state * 10 + 1;
                setState();
            }
            break;

        case UP:
            if(state > 10)
            {
                state = floor(state / 10);
                setState();
            }
            else
            {
                state = HOME;
                setState();
            }
            break;

            case MOD_HED_MAN_EDT:
                state = MOD_HED;
                setState();
                break;

        case OK:
            switch(state)
            {
                case MOD_LEV:
                    Mode = MOD_LEV;
                    setState();
                    break;

                case MOD_LIN:
                    state = MOD_LIN_XTE;
                    setState();
                    break;

                case MOD_LIN_XTE:
                    Mode = MOD_LIN;
                    confirmXTE();
                    state = MOD_LIN;
                    setState();
                    break;

                case MOD_MAN:
                    Mode = MOD_MAN;
                    setState();
                    break;

                case MOD_HED_ACT:
                    Mode = MOD_HED;
                    hdg = cog;
                    setState();
                    break;

                case MOD_HED_MAN_EDT:
                    confirmHDG();
                    cts = hdg;
                    state = MOD_HED;
                    Mode = MOD_HED;
                    setState();
                    break;

                case MOD_SBY:
                    Mode = MOD_SBY;
                    state = MOD_SBY;
                    break;

                case SET_RUD_BBD_EDT:
                    setRudderBbdPos();
                    break;

                case SET_RUD_NEU_EDT:
                    setRudderNeuPos();
                    break;

                case SET_RUD_STB_EDT:
                    setRudderStbPos();
                    break;

                case HOME:
                    state = MODE;
                    break;
            }
            break;

            // Keys nur für HDG-Eingabe

            case '1':
                decreaseHdg(1);
                setState();
                break;

            case '3':
                increaseHdg(1);
                setState();
                break;

            case '7':
                decreaseHdg(100);
                setState();
                break;

            case '9':
                increaseHdg(100);
                setState();
                break;

            // Key '0' = OFF: StandBy-Modus
            case OFF:
                state = MOD_SBY;
                Mode = MOD_SBY;
                setState();
                break;

    }
    setState();
}

void setState(void)
{
    switch(state)
    {
        case HOME:
            homeScreen();
            break;

        case MODE:
            modeScreen();
            break;

        case MOD_LEV:
            if(Mode == MOD_LEV)
            {
                modeLevelSelect();
            }
            else
            {
                modeLevelScreen();
            }
            break;

        case MOD_LIN:
            if(Mode == MOD_LIN)
            {
                modeLineSelect();
            }
            else
            {
                modeLineScreen();
            }
            break;

        case MOD_HED:
            if(Mode == MOD_HED_ACT || Mode == MOD_HED_MAN)
            {
                modeHeadingSelect();
            }
            else
            {
                modeHeadingScreen();
            }
            break;

        case MOD_HED_ACT:
            if(Mode == MOD_HED_ACT)
            {
                modeActHeadingSelect();
            }
            else
            {
                modeActHeadingScreen();
            }
            break;

        case MOD_HED_MAN:
            if(Mode == MOD_HED_MAN)
            {
                modeManHeadingSelect();
            }
            else
            {
                modeManHeadingScreen();
            }
            break;

        case MOD_HED_MAN_EDT:
            modeManHeadingSelect();
            break;

        case MOD_MAN:
            if(Mode == MOD_MAN)
            {
                modeManualSelect();
            }
            else
            {
                modeManualScreen();
            }
            break;

        case MOD_LIN_XTE:
            modeLineXTEScreen();
            break;

        case MOD_SBY:
            if(Mode == MOD_SBY)
            {
                modeStandBySelect();
            }
            else
            {
                modeStandByScreen();
            }
            break;

        case MONITOR:
            monitorScreen();
            break;

        case MON_XTE:
            monitorXteScreen();
            break;

        case MON_XTE_SHW:
            XTEmonitor();
            break;

        case MON_COG:
            monitorCogScreen();
            break;

        case MON_COG_SHW:
            COGmonitor();
            break;

        case MON_XTA:
            monitorXtaScreen();
            break;

        case MON_XTA_SHW:
            XTAmonitor();
            break;

        case MON_CTS:
            monitorCtsScreen();
            break;

        case MON_CTS_SHW:
            CTSmonitor();
            break;

        case MON_RUD:
            monitorRudderScreen();
            break;

        case MON_RUD_SHW:
            RUDmonitor();
            break;

        case MON_CRD:
            monitorCRDScreen();
            break;

        case MON_CRD_SHW:
            RCDmonitor();
            break;

        case SETTINGS:
            settingsScreen();
            break;

        case SET_RUD:
            settingsRudderScreen();
            break;

        case SET_RUD_BBD:
            settingsRudderBbd();
            break;

        case SET_RUD_NEU:
            settingsRudderNeutral();
            break;

        case SET_RUD_STB:
            settingsRudderStb();
            break;

        case SET_RUD_BBD_EDT:
            setRudderBbEdit();
            break;

        case SET_RUD_NEU_EDT:
            setRudderNeutralEdit();
            break;

        case SET_RUD_STB_EDT:
            setRudderStbEdit();
            break;

        default:
            state = floor(state / 10);
            setState();
    }
}


void homeScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Voyager");
    LCD_CursorPos(0x40);
    LCD_WriteText("Menu: #");
}

void menuScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("4:<  6:>");
    LCD_CursorPos(0x40);
    LCD_WriteText("5:Enter");
}

void modeScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MENU");
    LCD_CursorPos(0x40);
    LCD_WriteText("Mode");
}

void modeLevelScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText("Areal");
}

void modeLevelSelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText(">Areal<");
}

void modeLineScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText("Line");
}

void modeLineSelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText(">Line<");
}

void modeManualScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText("Manual");
}

void modeManualSelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText(">Manual<");
}

void modeHeadingScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText("Heading");
}

void modeHeadingSelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText(">Headng<");
}

void modeActHeadingScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("HEADING");
    LCD_CursorPos(0x40);
    LCD_WriteText("Actual");
}

void modeActHeadingSelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("HEADING");
    LCD_CursorPos(0x40);
    LCD_WriteText(">Actual<");
}

void modeManHeadingScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("HEADING");
    LCD_CursorPos(0x40);
    LCD_WriteText("Select");
}

void modeManHeadingSelect(void)
{
    char dspLine[12];
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("HEADING");
    LCD_CursorPos(0x40);
    Str_Printf(dspLine, ">%3.0f<", hdg);
    LCD_WriteText(dspLine);
}




void modeLineXTEScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("LINE XTE");
    LCD_CursorPos(0x42);
    Str_Printf(strn, "%03d", keepXTrack);
    LCD_WriteText(strn);
}

void modeStandByScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText("StandBy");
}

void modeStandBySelect(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MODE");
    LCD_CursorPos(0x40);
    LCD_WriteText(">StandBy");
}

void monitorScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MENU");
    LCD_CursorPos(0x40);
    LCD_WriteText("Monitors");
}

void monitorXteScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("Act. XTE");
}

void monitorCogScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("Act. COG");
}

void monitorXtaScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("Act. XTA");
}

void monitorCtsScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("Act. CTS");
}

void monitorRudderScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("Rudder");
}

void monitorCRDScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MONITOR");
    LCD_CursorPos(0x40);
    LCD_WriteText("C-Rudder");
}

void settingsScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("MENU");
    LCD_CursorPos(0x40);
    LCD_WriteText("Settings");
}

void settingsRudderScreen(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("SETTINGS");
    LCD_CursorPos(0x40);
    LCD_WriteText("Rud.-Pos");
}

void settingsRudderBbd(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Rudder");
    LCD_CursorPos(0x40);
    LCD_WriteText("Max. Bb");
}

void settingsRudderNeutral(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Rudder");
    LCD_CursorPos(0x40);
    LCD_WriteText("Neutral");
}

void settingsRudderStb(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Rudder");
    LCD_CursorPos(0x40);
    LCD_WriteText("Max. Stb");
}

void setRudderBbEdit(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Go Bb");
    LCD_CursorPos(0x40);
    LCD_WriteText("OK: #");
}

void setRudderNeutralEdit(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Go ahead");
    LCD_CursorPos(0x40);
    LCD_WriteText("OK: #");
}

void setRudderStbEdit(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Go Stb");
    LCD_CursorPos(0x40);
    LCD_WriteText("OK: #");
}

void XTEmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Act. XTE");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x40);
        Str_Printf(strn, "%3.0f m. ", xte);
        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_XTE;
    setState();
}

void COGmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Act. COG");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x42);
        Str_Printf(strn, "%3.0f ", cog);
        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_COG;
    setState();
}

void XTAmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Act. XTA");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x42);
        Str_Printf(strn, "%03d ", xta);
        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_XTA;
    setState();
}

void CTSmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Act. CTS");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x42);
        Str_Printf(strn, "%3.0f ", cts);
        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_CTS;
    setState();
}

void RUDmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Rudder");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x40);

        // Ruderstand graphisch darstellen
        if(RA == 0)
        {
            strn = "   ||   ";
        }
        if(RA < 0 && RA > -25)
        {
            strn = "   <    ";
        }
        if(RA <= -25 && RA > -50)
        {
            strn = "  <<    ";
        }
        if(RA <= -50 && RA > -75)
        {
            strn = " <<<    ";
        }
        if(RA <= -75)
        {
            strn = "<<<<    ";
        }
        if(RA > 0 && RA < 25)
        {
            strn = "    >   ";
        }
        if(RA >= 25 && RA < 50)
        {
            strn = "    >>  ";
        }
        if(RA >= 50 && RA < 75)
        {
            strn = "    >>> ";
        }
        if(RA >= 75)
        {
            strn = "    >>>>";
        }

        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_RUD;
    setState();
}

void RCDmonitor(void)
{
    char strn[9];
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("C-Rudder");
    while(1)
    {
        Key = Key_TranslateKey(Key_Scan());
        if(Key == UP)
        {
            break;
        }
        LCD_CursorPos(0x42);
        Str_Printf(strn, "%3d ", dra);
        LCD_WriteText(strn);
        Thread_Delay(50);
    }
    state = MON_CRD;
    setState();
}

void confirmXTE(void)
{
    char strn[9];
    Str_Printf(strn, "%3d m.",keepXTrack);
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("New XTE:");
    LCD_CursorPos(0x40);
    LCD_WriteText(strn);
    Thread_Delay(200);
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("LineMode");
    LCD_CursorPos(0x40);
    LCD_WriteText("engaged");
    Thread_Delay(200);
}

void increaseHdg(int cnt)
{
    hdg = hdg + cnt;
    if(hdg > 359)
    {
        hdg = hdg - 360;
    }
}

void decreaseHdg(int cnt)
{
    hdg = hdg - cnt;
    if(hdg < 0)
    {
        hdg = hdg + 360;
    }
}

void confirmHDG(void)
{
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("New HDG:");
    LCD_CursorPos(0x42);
    LCD_WriteFloat(hdg,0);
    Thread_Delay(100);
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("HDG Mode");
    LCD_CursorPos(0x40);
    LCD_WriteText("engaged");
    Thread_Delay(100);
}

void setRudderBbdPos(void)
{
    word rpos;
    rpos = GetAdc(7);
    EEPROM_WriteWord(MEMBBD,rpos);
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Raw:");
    LCD_CursorPos(0x04);
    LCD_WriteWord(rpos,4);
    LCD_CursorPos(0x40);
    LCD_WriteText("saved!");
    Thread_Delay(100);
    state = SET_RUD_BBD;
    setState();
}

void setRudderNeuPos(void)
{
    word rpos;
    rpos = GetAdc(7);
    EEPROM_WriteWord(MEMNEU,rpos);
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Raw:");
    LCD_CursorPos(0x04);
    LCD_WriteWord(rpos,4);
    LCD_CursorPos(0x40);
    LCD_WriteText("saved!");
    Thread_Delay(100);
    state = SET_RUD_NEU;
    setState();
}

void setRudderStbPos(void)
{
    word rpos;
    rpos = GetAdc(7);
    EEPROM_WriteWord(MEMSTB,rpos);
    LCD_Init();
    LCD_ClearLCD();
    LCD_CursorOff();
    LCD_CursorPos(0x00);
    LCD_WriteText("Raw:");
    LCD_CursorPos(0x04);
    LCD_WriteWord(rpos,4);
    LCD_CursorPos(0x40);
    LCD_WriteText("saved!");
    Thread_Delay(100);
    state = SET_RUD_STB;
    setState();
}
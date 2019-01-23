/*******************************************************************
*
* VoyagerAP.cc
*
* Hauptprogrammdatei des Projektes
*
* 2012 (C) Jörg Pauly
*
********************************************************************/
#thread 0, 0, 5
#thread 1, 64, 10
#thread 2, 64, 10
#thread 3, 128, 10
#thread 4, 64, 10

void main(void)
{
    initArray();
    Key_Init();
    splash();
    shown = 0;
    state = HOME;
    setState();
    Msg_WriteText("Starte PanelManager...\r");
    Thread_Start(4, panelManager);
    Msg_WriteText("Starte Rudersensor...\r");
    Thread_Start(1, rudderAngelManager);
    Msg_WriteText("Starte Rudersteuerung...\r");
    Thread_Start(2, rudderManager);
    Msg_WriteText("Starte NMEA-Listener...\r");
    Thread_Start(3, listenNMEA);

    while(1)
    {
        //MainLoop();
    }
}

/* Endlosschleife, die nichts tut... ausser zu Testzwecken */
void MainLoop(void)
{
    // Hier können Ausgaben zu Debugzwecken erzeugt werden!
}

void initArray(void)
{
    Version = "1.1.2836";
    xte = 0;
    l_xte = 0;
    dtw = 0;
    btw = 0;
    htw = 0;
    cog = 0;
    cts = 0;
    xta = 0;
    Mode = MOD_SBY;
}
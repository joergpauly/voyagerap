/******************************************************************************
*
*  DataArray.cc
*
*  Prototypen aller globalen Variabelen
*
*  2012 (C) Jörg Pauly
*
******************************************************************************/
/* Globale Definitionen */
/* Speicherpositionen Ruderlagen */
#define MEMBBD 0
#define MEMNEU 2
#define MEMSTB 4

/* NMEAListener */
char inBuffer[255]; //Puffer eingehende NMEA-Daten
char outBuffer[86];//Puffer für ausgehend an Funkstation
char stream[255]; //RS232-Datenstrom
char sentence[85]; //NMEA-Satz
float xte; //Track-Abweichung in Metern, negativ = links
float l_xte; //Hilfs-Kopie von xte
float dtw; //Distance to Waypoint in Metern
float btw; //Bearing to waypoint, Basis 360°
float htw; //Heading to Waypoint, rechtweisend
float cog; //Course over Ground, rechtweisend
float cts; //Course to Steer, rechtweisend
int xta; //Track Angel, Winkel TOG<>CTS, negativ = links
float sog; //Speed over Ground, km/h
float hdg; //Heading selected
char valBuf01[32]; // \
char valBuf02[32];
char valBuf03[32];
char valBuf04[32];
char valBuf05[32];
char valBuf06[32];
char valBuf07[32];     // Puffer für die Sentences
char valBuf08[32];
char valBuf09[32];
char valBuf10[32];
char valBuf11[32];
char valBuf12[32];
char valBuf13[32];
char valBuf14[32];
char valBuf15[32]; // /

/* PanelManager */
char Version[9];
char Key;           /* ASCII-Daten der Tastatur */
int state;         /* Display-Status (Bild) */
int dra;            /* errechneter Ruderausschlag */

/* RudderManager */
char strn[80];
int RA;
int steerAngel;
word rudderPosBb;       // Ruderposition Bb
word rudderPosNeutral;  // Ruderposition Neutral
word rudderPosStb;      // Ruderposition Stb

/* NaviManager */
int Mode; // Areal, Line oder Manual
int keepXTrack; /*links = negativ*/

/* I/O-Ports */
#define RUDDERINADC 7


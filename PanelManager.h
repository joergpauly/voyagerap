/*******************************************************************************
*
* PanelManager.h
*
* Defintionen der Panel-Stati
*
* 2012 (C) Jörg Pauly
*
*******************************************************************************/
/* States */
#define HOME 0
#define MODE 1
#define MOD_LEV 11
#define MOD_LIN 12
#define MOD_LIN_XTE 121
#define MOD_MAN 13
#define MOD_HED 14
#define MOD_HED_ACT 141
#define MOD_HED_MAN 142
#define MOD_HED_MAN_EDT 1421
#define MOD_SBY 15
#define MONITOR 2
#define MON_XTE 21
#define MON_XTE_SHW 211
#define MON_COG 22
#define MON_COG_SHW 221
#define MON_XTA 23
#define MON_XTA_SHW 231
#define MON_CTS 24
#define MON_CTS_SHW 241
#define MON_RUD 25
#define MON_RUD_SHW 251
#define MON_CRD 26
#define MON_CRD_SHW 261
#define SETTINGS 3
#define SET_RUD 31
#define SET_RUD_BBD 311
#define SET_RUD_BBD_EDT 3111
#define SET_RUD_NEU 312
#define SET_RUD_NEU_EDT 3121
#define SET_RUD_STB 313
#define SET_RUD_STB_EDT 3131

/* Keys */
#define FORWARD '6'
#define BACK '4'
#define ENTER '5'
#define UP '2'
#define OK '#'
#define CANCEL '*'
#define OFF '0'

int shown; // Menühilfe nur einmal anzeigen.
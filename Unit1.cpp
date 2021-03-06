// Dame-Spiel
//
// 8x8 Felder in einem "9x9"er Array - 0,x und x,0 sind unbenutzt,
// deshalb kann direkt per index 1..8 auf die felder geschrieben werden.
// Idee:
//      0 entspricht leerem Feld
//      1 entspricht Spieler1-Stein
//      2 entspricht Spieler2-Stein
//      3 entspricht Spieler1-Dame
//      4 entspricht Spieler2-Dame
//
//      startbrettbelegen: belegt das Brett mit der Startaufstellung
//      brettausgeben: gibt die aktuelle Brett-Belegung aus
//
//      move1 und move2 bewegt je Spieler1- und Spieler2-Stein
//      beat1 und beat2 schl�gt je als Spieler1- und Spieler2-Stein
//      damemove1 und damemove2 bewegt je Spieler1- und Spieler2-Dame
//      damebeat1 und damebeat2 scl�gt je als Spieler1 und Spieler2-Dame
//
//      Returnaufschluesselung der Funktionen:
//        0 - Fehler
//        1 - Erfolgreich
//       10 - Start- oder Ziel-Feldkoordinaten nicht "auf dem Brett"
//       11 - Stein im Startfeld entspricht nicht dem der aufgerufenen Funktion
//       12 - Stein wird "nicht korrekt hoch oder runter" bewegt
//       13 - Stein wird "nicht korrekt diagonal" bewegt
//       14 - Das Zielfeld ist nicht leer
//       20 - --> 10
//       21 - --> 11
//       22 - Dame wird nicht diagonal bewegt
//       24 - --> 14
//       30 - --> 10
//       31 - --> 11
//       32 - Stein wird nicht in zul�ssige Richtung bewegt
//       33 - kein Gegner zu schlagen
//       34 - --> 14
//       99 - kann weiterschlagen
//
//      Beschreibung von damemove:
//       1. Start- und Zielfeld "auf dem Brett" ??
//       2. Dame in Startfeld ??
//       3. Richtung pr�fen ((1 == DeltaX) && (DeltaX == DeltaY) ??
//       4. Zielfeld frei ??
//       5. Dame umsetzen !!
//
//       Returnaufschl�sselung von beat1:
//         0 - Fehler
//         1 - Erfolgreich
//        30 - entspricht
//
//
//        .........wird fortgesetzt!!!!
//

#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include "dame_spiel.h"
#include "dame_grafik.h"

//####################################################################################################################################
//########## BEGINN DER DEKLARATIONEN ################################################################################################
//####################################################################################################################################
//
// Spielfeldarray brett[9][9] wird in "dame_spiel.h" angelegt
// mousex und mousey werden in "dame_spiel.h" angelegt
//
//####################################################################################################################################
//########## ENDE DER DEKLARATIONEN ##################################################################################################
//####################################################################################################################################

main()                                                                          //MAIN - Spielrumpf
{
 int errorcode=1;                                                               //Fehlervariable anlegen
 int vx=0;
 int vy=0;
 int nx=0;
 int ny=0;
 bool weiterschlagen=false;
 bool mussschlagen=false;

 char jein='?';

 int sieg=0;

 spielanleitung();

 START:

 startbrettbelegen2();                                                           //Feld vorbelegen
// brett[4][4]=3;                                                               //-----------------------TEST_DAME_SETZEN----nochmal loescht nicht--TEST-
// brett[5][5]=4;                                                               //-----------------------TEST_DAME_SETZEN----nochmal loescht nicht--TEST-
 brettausgeben();                                                               //Startfeld inkl Figuren ausgeben
 do
 {
  do
  {
   spieler=1;
   gotoxy(38,9);
   textcolor(12);
   cprintf("Spieler 1 ist dran..");

   gotoxy(80,25);
   if (weiterschlagen)
   {
    vx=nx;
    vy=ny;
   }
   else
   {
    do
    {
     getmouselxy();                                                              //auf Linksklick warten und nach mousex und mousey schreiben
     vx=((mousex+2)/4);                                                          //Koordinaten zu Feldkoordinaten umrechnen
     vy=((mousey+3)/3);                                                          //Koordinaten zu Feldkoordinaten umrechnen
     if (exit_klick())
     {
      gotoxy(50,15);
      printf("Spiel wurde abgebrochen!");
      gotoxy(50,16);
      printf("Bitte Taste dr\x81 \bcken...",mousex,mousey);
      gotoxy(80,25);
      goto ENDE;
     }
    }
    while ((vx<1)||(vx>8)||(vy<1)||(vy>8));
   }
   markiere_feld(vx,vy);

   gotoxy(80,25);
   do
   {
    getmouserxy();                                                                //auf Rechtsklick warten und nach mousex und mousey schreiben
    nx=((mousex+2)/4);                                                            //Koordinaten zu Feldkoordinaten umrechnen
    ny=((mousey+3)/3);                                                            //Koordinaten zu Feldkoordinaten umrechnen
    if (exit_klick())
    {
     gotoxy(50,15);
     printf("Spiel wurde abgebrochen!");
     gotoxy(50,16);
     printf("Bitte Taste dr\x81 \bcken...");
     gotoxy(80,25);
     goto ENDE;
    }
   }
   while ((nx<1)||(nx>8)||(ny<1)||(ny>8));
   markiere_feld(nx,ny);

   gotoxy(80,25);
   _sleep(1);

   if (((weiterschlagen==false)||((weiterschlagen)&&(abs(nx-vx)==2)&&(abs(ny-vy)==2)))
       &&((mussschlagen==false)||((mussschlagen)&&(abs(nx-vx)==2)&&(abs(ny-vy)==2))))
   {
    errorcode=move_all(vx,vy,nx,ny, 1);                                           //Zug durchf�hren - bzw Errorcode "erhalten"
   }
   else errorcode=0;
   clrscr();
   brettausgeben();
   weiterschlagen=false;
   if (errorcode!=1)
   {
    if (errorcode==99)
    {
     weiterschlagen=true;
     gotoxy(38,14);
     printf("Spieler 1 kann weiterschlagen!");
     gotoxy(80,25);
    }
    else
    {
     gotoxy(38,14);
     printf("Ung\x81ltiger Zug.. nochmal!!");
     gotoxy(80,25);
    }
   }
  sieg=gewonnen(1);
  if ((sieg==1)||(sieg==2)||(sieg==3)) break;
  }
  while (errorcode!=1);                                                         //solange nicht 1 von move_all zur�ckgegeben wurde weitermachen
  mussschlagen=muss_schlagen(2);
  sieg=gewonnen(1);
  if ((sieg==1)||(sieg==2)||(sieg==3)) break;                                   //wenn Spieler1 gewonnen hat --> verlasse die Schleife
  do
  {
   spieler=2;
   gotoxy(38,9);
   textcolor(13);
   cprintf("Spieler 2 ist dran..");

   gotoxy(80,25);
   if (weiterschlagen)
   {
    vx=nx;
    vy=ny;
   }
   else
   {
    do
    {
     getmouselxy();                                                              //Linksklickkoordinaten holen
     vx=((mousex+2)/4);                                                          //Koordinaten zu Feldkoordinaten umrechnen
     vy=((mousey+3)/3);                                                          //Koordinaten zu Feldkoordinaten umrechnen
     if (exit_klick())
     {
      gotoxy(50,15);
      printf("Spiel wurde abgebrochen!");
      gotoxy(50,16);
      printf("Bitte Taste dr\x81 \bcken...");
      gotoxy(80,25);
      goto ENDE;
     }
    }
    while ((vx<1)||(vx>8)||(vy<1)||(vy>8));
   }

   markiere_feld(vx,vy);


   gotoxy(80,25);
   do
   {
    getmouserxy();                                                               //Rechtsklickkoordinaten holen
    nx=((mousex+2)/4);                                                           //Koordinaten zu Feldkoordinaten umrechnen
    ny=((mousey+3)/3);                                                           //Koordinaten zu Feldkoordinaten umrechnen
    if (exit_klick())
    {
     gotoxy(50,15);
     printf("Spiel wurde abgebrochen!");
     gotoxy(50,16);
     printf("Bitte Taste dr\x81 \bcken...");
     gotoxy(80,25);
     goto ENDE;
    }
   }
   while ((nx<1)||(nx>8)||(ny<1)||(ny>8));
   markiere_feld(nx,ny);


   gotoxy(80,25);
   _sleep(1);

   if (((weiterschlagen==false)||((weiterschlagen)&&(abs(nx-vx)==2)&&(abs(ny-vy)==2)))
       &&((mussschlagen==false)||((mussschlagen)&&(abs(nx-vx)==2)&&(abs(ny-vy)==2))))
   {
    errorcode=move_all(vx,vy,nx,ny, 2);                                       //Zug durchf�hren - bzw Errorcode "erhalten"
   }
   else errorcode=0;

   clrscr();
   brettausgeben();
   weiterschlagen=false;
   if (errorcode!=1)
   {
    if (errorcode==99)
    {
     weiterschlagen=true;
     gotoxy(38,14);
     printf("Spieler 2 kann weiterschlagen!");
     gotoxy(80,25);
    }
    else
    {
     gotoxy(38,14);
     printf("Ung\x81ltiger Zug.. nochmal!!");
     gotoxy(80,25);
    }
   }
  sieg=gewonnen(1);
  if ((sieg==1)||(sieg==2)||(sieg==3)) break;
  }
  while (errorcode!=1);                                                         //solange nicht 1 von move_all zur�c
  sieg=gewonnen(2);
  mussschlagen=muss_schlagen(1);                                                             //pr�fe ob jemand gewonnen hat
 }
 while (sieg==0);                                                               //solange niemand gewonnen hat wiederhole Schleife

 brettausgeben();
 sieger(sieg);

 gotoxy(43,10);
 printf("Nochmal spielen? (j): ");
 jein=getche();
 switch(jein)
 {
  case 'j':
  case 'J': gotoxy(43,10);
            printf ("                          ");
            gotoxy(80,25);
            goto START;
            break;
  default:  gotoxy(43,10);
            printf("Bitte Taste dr\x81 \bcken... ");
            gotoxy(80,25);
            break;
 }


 ENDE:

 getche();                                                                       // auf Eingabe warten... dann ENDE
}

//
//  Created by Alexandra Härnström on 2021-07-12.
//


#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// Globala konstanter
const int ANTAL_BOKSTAVER = 26;     // A-Z
const int ANTAL_SPRAK = 4;          // Engelska, Franska, Svenska, Tyska
const int ASCII = 65;               // Char 'A' har värde 65 i ASCII tabellen

// 2d-arrayen TOLK_HJALP innehåller bokstavsfrekvensen i %
// för fyra språk. TOLK_HJALP[0][0] är frekvensen av
// bokstaven a för engelska.TOLK_HJALP[0][25] är frekvensen av
// bokstaven z för engelska.
// Arrayen är en GLOBAL KONSTANT och skall alltså ej skickas
// som parameter till den funktion som behöver den.
const double TOLK_HJALP[ANTAL_SPRAK][ANTAL_BOKSTAVER]=
       {{8.27,1.48,2.94,4.03,11.78,2.22,1.72,6.77, //engelska
         7.39,0.12,0.81,3.76,2.85,6.71,7.79,1.54,
         0.05,5.95,6.69,9.07,2.66,1.13,2.14,0.19,
         1.89,0.03},
        {7.97,1.40,3.55,3.79,16.89,1.02,1.00,0.75, //franska
         7.08,0.38,0.04,5.51,2.82,8.11,5.19,2.78,
         1.01,6.69,8.35,7.22,6.09,1.35,0.02,0.54,
         0.30,0.15},
        {9.50,1.11,1.53,5.30,8.94,1.74,3.57,3.94,  //svenska
         3.98,0.89,3.26,4.93,3.41,8.46,5.01,1.77,
         0.00,6.73,5.56,9.20,1.94,2.42,0.00,0.05,
         0.45,0.00},
        {5.12,1.95,3.57,5.07,16.87,1.35,3.00,5.79, //tyska
         8.63,0.19,1.14,3.68,3.12,10.64,1.74,0.42,
         0.01,6.30,6.99,5.19,3.92,0.77,1.79,0.01,
         0.69,1.24}};


class Text {

private:
    // Attribut
    string m_text;
    int m_histogramAbs[ANTAL_BOKSTAVER];
    int m_histogramRel[ANTAL_BOKSTAVER];
    int m_antal;

public:
    // Attribut
    const string nyText;

    // Konstruktor
    Text();

    // Metoder
    void setText(const string &);
    bool beraknaHistogramAbs();
    void skrivHistogramAbs();
    void absTillRel();
    void plottaHistogramRel();
    bool beraknaHistogram(int);
    string tolka();
};


// Funktionsdeklarationer
string namn_pa_fil();
string inlasning(string);


int main() {
    
    // Lokala variabler
    int antal = 0;      // Håller värdet för om det absoluta histogrammet innehåller bokstäver
    Text minText;       // Skapar ett objekt av typen Text
    string filtext;     // Lagrad text från fil
    string sprak;       // Språket texten förmodligen är skriven på

    //Låter användaren öppna en .txt fil
    string filnamn = namn_pa_fil();

    // Öppna filen för inläsning
    ifstream fin(filnamn.c_str());

    // Felkontroll om filen inte finns
    if (!fin) {
        cout << endl << "Filen existerar inte." << endl << endl;

        // Avsluta program med felkod
        exit(EXIT_FAILURE);
    }
    else {
        // Spara filens innehåll i en string 'filtext'
        filtext = inlasning(filnamn);
    }
    
    // Skicka strängen 'filtext' till objektet minText
    minText.setText(filtext);
    
    // Beräkna antal och frekvens av bokstäver som finns i given text
    // Returnerar antal som är antingen true (1) om det finns bokstäver
    // i det absoluta histogrammet eller false (0) om det ej finns
    // bokstäver i det absoluta histogrammet
    antal = minText.beraknaHistogram(antal);
    
    // Skriv ut frekvens av bokstäver
    minText.skrivHistogramAbs();
    
    // Det relativa histogrammet ska endast beräknas om
    // det finns bokstäver i det absoluta histogrammet
    if (antal) {
        
        // Skriv ut stjärnor för procentvärde i det relativa histogrammet
        minText.plottaHistogramRel();
        
        // Jämför med fyra språk och returnera det språk
        // texten förmodligen är skriven på
        sprak = minText.tolka();
    }

    return 0;
    }


// Klassimplementationen

Text::Text() {
    m_text = "";
    m_histogramAbs[ANTAL_BOKSTAVER];
    m_histogramRel[ANTAL_BOKSTAVER];
    m_antal = 0;
}


// Funktionsdefinitioner:


void Text::setText(const string &nyText) {
    m_text = nyText;
}


string namn_pa_fil() {
    
    string filnamn;
    
    cout << "Skriv in ett filnamn: " << endl;
    getline(cin, filnamn);
    
    int length = (int)filnamn.length();

    // Om '.txt' saknas i slutet av filnamn läggs det till
    if (filnamn[length - 1] != 't' || filnamn[length - 2] != 'x' ||
        filnamn[length - 3] != 't' || filnamn[length - 4] != '.') {
        filnamn.append(".txt");
    }

    return filnamn;
}


string inlasning(string filnamn) {
    
    string rad;
    string filtext;

    // Öppna filen för inläsning
    ifstream fin(filnamn.c_str());
    
    // Läs filen rad för rad tills det är slut på rader
      while (getline(fin, rad)) {

          // Lägg till rad i text
            filtext = filtext.append(rad + "\n");
      }
    
    cout << filtext << endl;
    
    return filtext;
}


bool Text::beraknaHistogram(int antal) {

    // Beräkna frekvens av bokstäver i given text
    antal = beraknaHistogramAbs();
    
    // Det relativa histogrammet ska endast beräknas om
    // det finns bokstäver i det absoluta histogrammet
    if (antal) {
        absTillRel();
    }
    
    return antal;
}


bool Text::beraknaHistogramAbs() {

    int ejBokstaver = 0;
    int length = (int)m_text.length();
    int antal;
    
    // Nollställer histogram
    for (int i = 0; i < ANTAL_BOKSTAVER; i++) {
        m_histogramAbs[i] = 0;
    }
    
    // Gör om text till uppercase
    for (int i = 0; i < length; i++) {
        m_text.at(i) = toupper(m_text.at(i));
    }

    // Loopar igenom alla karaktärer
    for (int i = 0; i < length; i++) {
        int index;
        
        // Kollar om karaktären är en bokstav A-Z
        if (isalpha(m_text.at(i))) {
            
            // Sätter alla 'A' på index 0, alla 'B' på index 1 osv...
            index = m_text.at(i) - ASCII;
            m_histogramAbs[index]++;
        }
        
        // Om karaktären ej är en bokstav
        else {
            ejBokstaver++;
        }
    }
    antal = length - ejBokstaver;
    
    m_antal = antal;
    
    return antal;
}


void Text::absTillRel() {

    // Nollställer histogram
    for (int i = 0; i < ANTAL_BOKSTAVER; i++) {
        m_histogramRel[i] = 0;
    }

    // Beräkna procentandel per bokstav i m_histogramAbs
    for (int i = 0; i < ANTAL_BOKSTAVER; i++) {
        float procent = 100.0 * m_histogramAbs[i]/m_antal;
        
        // Sätt procentandel som index i m_histogramRel
        m_histogramRel[i] = procent;
        
        // Skriver ut char alfabet och bokstävernas procentdel
        cout << char(i+ASCII) << "\t" << procent << "%" << endl;
    }
    cout << endl;
}


void Text::skrivHistogramAbs(){

    // Skriver ut char alfabet och frekvens av bokstäver
    for (int i = 0; i < ANTAL_BOKSTAVER; i++) {
        cout << char(i+ASCII) << "\t" << m_histogramAbs[i] << endl;
    }
    cout << endl;
}


void Text::plottaHistogramRel() {

    char star = '*';
    int stars;

    // Skriv ut chars i alfabet och bokstävernas procentdel som stjärnor
    for (int i = 0; i < ANTAL_BOKSTAVER; i++) {
        cout << char(i+ASCII) << "\t";

        // 1% är 2*. 50% är 100*
        if (m_histogramRel[i] > 0) {
            stars = m_histogramRel[i] * 2;

            for (int j = 0; j < stars; j++) {
                cout << star;
            }
            cout << endl;
        }
        else {
            cout << endl;
        }
    }
    cout << endl;
}


string Text::tolka() {

    float ksEng, ksFr, ksSv, ksTy;
    float ks, ksLagst;

    // Testar alla fyra språk med texten
    // j = språket (j=0 är engelska...osv)
    for (int j = 0; j < 4; j++) {

        // Nollställer kvadratsumman inför nytt språk
        ks = 0.0;

        // Kör bokstav för bokstav och börjar med A på index 0
        for (int i = 0; i < ANTAL_BOKSTAVER; i++) {

            // Kollar om bokstaven finns i den inmatade texten
            if (m_histogramRel[i] > 0) {

                // Räknar ut kvadratsumman
                ks += pow(TOLK_HJALP[j][i]-m_histogramRel[i],2);
            }

            // Lägger in kvadratsumman i 'j'-språk
            switch(j) {
                case 0:
                    ksEng = ks;
                    break;
                case 1:
                    ksFr = ks;
                    break;
                case 2:
                    ksSv = ks;
                    break;
                case 3:
                    ksTy = ks;
                    break;
                default:
                    cout << "Fel i tolkning() - switch" << endl;
            }
        }
    }

    // Initierar lägsta kvadratsumman att tillhöra engelska
    ksLagst = ksEng;
    string sprak = "engelska";

    // Jämför vilket språk som har lägst kvadratsumma
    if (ksFr < ksLagst) {
        ksLagst = ksFr;
        sprak = "franska";
    }
    if (ksSv < ksLagst) {
        ksLagst = ksSv;
        sprak = "svenska";
    }
    if (ksTy < ksLagst) {
        ksLagst = ksTy;
        sprak = "tyska";
    }

    cout << "Texten är förmodligen skriven på " << sprak << "." << endl;
    cout << endl;
    
    return sprak;
}


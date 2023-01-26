#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <time.h>
#include <TStopwatch.h>
#include <TCanvas.h>
#include <TSystem.h>

 string filename;

   void rdlist(void)
 {

//  This routine reads the list of data files in the Data directory, and offers that list
//   from which one may select a file for processing

  ifstream   inlist;
  ofstream   ifile;
  int  no;
  char   fname[5000];
  string  flist[5000];
  int i = 0;

//  cout << cout <<  " Writing List " << endl;

  inlist.open( "analist.dat" );
   while( inlist.good() )
    { inlist.getline( fname, 5000 ); i++;
      flist[i] = string(fname);
      cout << "  " << i << "  " << flist[i] << endl; }

    cout << endl;
    cout << "  Which file " ;
    cin >> no;
    cout << " You have chosen number " << no << endl << endl;

   cout << endl << " file no " << no << " is " << flist[no] << endl << endl;

   filename = flist[no] ;

   return;
}

  void anawc(void)
 { 

 TFile *ff = new TFile("anaw.root", "RECREATE" );

//    datfile  is the data file to read from
//    infile has the name of the datfile
 ifstream  datfile;
 ifstream  infile;

 string    FullLine;
 string    tempfn;
 string    label;
 
 int       Ldone, Nev, NevSp ;
 int       In1, In2, In3;
 int       InNev, InNevsp;
 int       Nspill, InNspill;
 string    spDate, spTime;
 int       IplHit[8], Jwhit[8];
 int       i, j, k, ix, iy;
 float     a, b, w, t;

  TStopwatch timer;
  timer.Start();
 
 TH1     *h150[8];
 TH1     *h160[8];
 TH1     *h200[16];
 TString  hname, htit, hhnam, htname;
 
 int       EvGoing, MaxEvent;
 int       MTdc, InChan, InTime;
 int       EvW[200], EvT[200], nhit;
 int       ipl, iwpl, itdc;
 int       ConSpMo, ConSpDay, ConSpYr, ConSpHr, ConSpMin, ConSpSec;
 float     Etime, EtimeLast, dEtime;
 int       JEt1, JEt2, FoundEtime;
 
 TH1F *h100 = new TH1F( "h100", "All Wire Hits", 128, -0.5, 1023.5 );
 h100 -> SetXTitle(" Wire Number");
 h100 -> SetFillColor(3);
 
 TH1F *h120 = new TH1F("h120", "Event Time", 600, 0.0, 6.0 );
 h120 -> SetFillColor(3);
 TH1F *h121 = new TH1F("h121", "Time bet. Ev", 201, 0.0, 0.001 );
 h121 -> SetFillColor(3);
 TH1F *h130 = new TH1F("h130", "Events per Spill", 251, -0.5, 251.5 );
 h130 -> SetFillColor(3); 

//                             for each wire plane = X1, Y1, X2 ...
  string lblXY[2];    lblXY[0] = "X";  lblXY[1] = "Y";
   int ich;
   
 for( i = 0; i < 8; i++ )
 { 
    std::stringstream sstm;
    sstm << "h15" << i;
    hname = sstm.str(); 
    ich = i/2 + 1;
    j = i%2;
    sstm << " Wire Chamber " << ich   << " " << lblXY[j];
    htit = sstm.str();
   h150[i] = new TH1F( hname, htit, 128, -0.5, 127.5 );
   h150[i] -> SetXTitle(" Wire No");
   h150[i] -> SetFillColor(3); 
 }

 for( i=0; i< 8; i++ )
 {
    std::stringstream  sstn, sttx; 
    sstn << "h16" << i;
    hhnam = sstn.str();  
    ich = i/2 + 1;
    j = i%2;
    sttx << " Hits " << ich << lblXY[j] ;    
    htit = sttx.str();

    h160[i] = new TH1F( hhnam, htit, 11, -0.5, 10.5 );
    h160[i] -> SetFillColor(3);
 }

 TH1F *h110 = new TH1F( "h110", "All Time Hits", 1301, -0.5, 1300.5 );
 h110 -> SetXTitle("Time Bins");
 h110 -> SetFillColor(3);
 
 TH2F *h181 = new TH2F( "h181", " Chamber 1", 128, -0.5, 127.5, 128, -0.5, 127.5 );
 TH2F *h182 = new TH2F( "h182", " Chamber 2", 128, -0.5, 127.5, 128, -0.5, 127.5 );
 TH2F *h183 = new TH2F( "h183", " Chamber 3", 128, -0.5, 127.5, 128, -0.5, 127.5 ); 
 TH2F *h184 = new TH2F( "h184", " Chamber 4", 128, -0.5, 127.5, 128, -0.5, 127.5 );
 
  for( i = 0; i < 16; i++ )
  { 
   std::stringstream strt, sttt;
    if( i < 10 ) strt << "h20" << i;
    if( i >  9 ) strt << "h2" << i;
    htname = strt.str(); 
    
    sttt << " M TDC " << i ;
    htit = sttt.str();
   h200[i] = new TH1F( htname, htit, 1301, -0.5, 1300.5 );
   h200[i] -> SetXTitle("Time Bins");
   h200[i] -> SetFillColor(3);
  } 
 
 
//=================    off to the analysis  ==========================

 cout << endl << "  doing getlistewa " << endl << endl;
 gSystem -> Exec("./getlist.sh");

 cout << " doing rdList " << endl << endl;
 rdlist();

  datfile.open(filename.c_str() ); 

//  Save a version of h100 with the input dat file name as a title

 TH1F *h101 = new TH1F( "h101", filename.c_str(), 128, -0.5, 1023.5 );
 h101 -> SetXTitle(" Wire Number");
 h101 -> SetFillColor(4);

 Ldone = 0;
 Nev = 0;
 NevSp = 0;
 Nspill = 0;
 EvGoing = 0;
 nhit = 0;
 FoundEtime = 0;
 EtimeLast = -1.0;
 for( i=0; i<8; i++ )  IplHit[i] = 0;
 
 while( !datfile.eof() ) 
 {
 for( i = 0; i < 11; i++ ) 
   {
     getline( datfile, FullLine );
     if( Ldone < 1 ) cout << endl << " Comment line > " << FullLine << endl << endl;
     Ldone++;
     istringstream line( FullLine.c_str() );
     
     line >> label;
     
     if( label == "SPILL" )
     { line >> InNspill;  Nspill++ ; 
       a = Nspill;  b = NevSp;   h130 -> Fill(a,b);
  
      cout << " New Spill number " << Nspill << "  NevSp = " << NevSp  << 
      "   Nev " << Nev << endl; 
      NevSp = 0;
     }
     
     if( label == "SDATE" )
     { line >> In1; line >> In2;  line >> In3; 
     	       ConSpDay = In1; ConSpMo = In2; ConSpYr = In3;  }
     	       
     if( label == "STIME" )
     { line >> In1;  line >> In2;  line >> In3;
             ConSpHr = In1;  ConSpMin = In2;  ConSpSec = In3 ; }
             
     if( label == "TTIME" )
     {   line >> spDate;  line >> spTime; 
//      cout << " Spill Date " << spDate << "     Spill Time " << spTime;
//      cout << endl;  }
     }
     
   if( label =="ETIME" )
    {  line >> JEt1; line >> JEt2;  Etime = JEt2*9.14E-9;
       h120 -> Fill(Etime);
       if( FoundEtime > 0 ) 
      	  { dEtime =  Etime - EtimeLast;
      	    h121 -> Fill(dEtime);      
      	  }    
        FoundEtime = 1;  EtimeLast = Etime;
    }
     
     
    if( label == "Module" )     //  chamber TDC number 
      	{ line >> In1;   MTdc = In1; }  

    if( label == "Channel" )    // which wire hit, at what time
      	 { line >> InChan;  line >> InTime;

      	      	EvW[nhit] = InChan + 64*(MTdc-1);
      	      	EvT[nhit] = InTime;
      	        nhit++;    if( nhit > 199 ) nhit=199;  }   
      	        
//  -------------------------------   EVENT processing --------------     	        
     if( label == "EVENT" )
     {	line >> In1;  line >> In2;
     InNev = In1; InNevsp = In2; 
//      cout << endl << "  line " << Ldone << "   Got an EVENT " << endl; 
     if( EvGoing ) 
       {     Nev++ ;  NevSp++;
//       cout << endl;  cout << " ready with event, nhit = " << nhit  << endl;

   if( Nev < -3 )       // Take a look at an event  
    {   int nn; nn=nhit;
//     int j;
     if( nn < 1 ) nn = 1;     if( nn > 30) nn = 30;   
     cout << " Event Number " << Nev << "   nhits =" << nhit << endl;
     for( j=0; j < nn; j++ )
    {   cout << " j " << j << " wire " << EvW[j]  << endl;  }

 }
    for( k=1; k<nhit ; k++ )
         { w = EvW[k];   h100 -> Fill(w);   h101 -> Fill(w);
           t = EvT[k];   h110 -> Fill(t);

            ipl = EvW[k]/128;   IplHit[ipl]++;
            iwpl = EvW[k] - 128*(ipl); Jwhit[ipl] = iwpl;
            a = iwpl, t=EvT[k];
            itdc = EvW[k]/64;
  
            h150[ipl] -> Fill(a);
           if( itdc >= 0 && itdc < 16 )  h200[itdc] -> Fill(t);            
       }
         
       for( j=0; j<4; j++ )
        { ix = 2*j;    iy = ix+1;
//           if( IplHit[ix] == 1 && IplHit[iy] == 1 )
           { a = Jwhit[ix]; b = Jwhit[iy]; 
               if( j == 0 ) h181 -> Fill(a,b);
               if( j == 1 ) h182 -> Fill(a,b);
               if( j == 2 ) h183 -> Fill(a,b);
               if( j == 3 ) h184 -> Fill(a,b);   
           }
         }
         
         for( j= 0; j< 8; j++ )
         { a = IplHit[j]; h160[j] -> Fill(a); IplHit[j] = 0; }
         nhit = 0;
       }
//                  one might add event by event analysis code hier
    EvGoing = 1;
    nhit = 0;
   }
  }
 }
 datfile.close();
 
 timer.Stop();
 cout << endl;
 cout << "  timer cpu time = " << timer.CpuTime() << " sec" << endl;

//   Run t1041_30.dat   takes 3.36 sec inturpeted.  .71 sec compiled

 cout << endl;
 cout << "  Finished Reading   " << Ldone << "  lines" << endl;
 cout << "  Number of Spills   " << Nspill << endl;
 cout << "  Number of Events   " << Nev;
 cout << endl << endl; 
 cout << "  Histograms saved to   anaw.root  " << endl;
 cout << "  .X plwcg.C    to load the file" << endl << endl;

 TCanvas *anapl = new TCanvas("anapl","Some Plots",100,10,600,600);
 anapl -> Divide(1,2);
 anapl -> cd(1);
 h100 -> Draw();
 anapl -> cd(2);
 h110 -> Draw();

 TCanvas *bmpl = new TCanvas("bmpl","Some Plots",300,100,400,600);
 bmpl -> Divide(1,2);
 bmpl -> cd(1);
 h120 -> Draw();
 bmpl -> cd(2);
 h121 -> Draw();



  ff-> Write();

// cout << endl << "  Type a Number ";
// cin >> a;
// cout <<  " Thanks " <<  endl << endl;
}

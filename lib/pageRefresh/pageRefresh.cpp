/*!
 * pageRefresh.cpp - Easy library for Nextion Displays
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/*! Archivo separado < pageRefresh.cpp > para las funciones de RefreshCurrentPage(), 
 *  para no hacer demasiado largo el EasyNextionLibrary.cpp. Hay tantas funciones como
 *  número de páginas en el programa de Nextion. Así es más sencillo para editar.
 */
#ifndef EasyNextionLibrary_h
  #include "EasyNextionLibrary.h"
#endif

#ifndef pageRefresh_h
  #include "pageRefresh.h"
#endif


/* FUNCIONES PRIVADAS DE REFRESCO DE PAGINAS DEL NEXTION
 * Se definen las acciones a ejecutar específicas para cada página
 * Si tienes más páginas, añade más funciones void refreshPageXX()en
 * el .ino y en pageRefresh.h
 * y un case más en la función RefreshCurrentPage() en este cpp 
 */

void EasyNex::RefreshCurrentPage(){ 

  switch(currentPageId){ //esta variable es publica en la libreria principal
  case 0: // página: main 
    refreshPage0();
    break;      
  case 1: // página: hora 
    refreshPage1();
    break;      
  case 2: // página: setTiempo 
    refreshPage2();
    break;       
  case 3: // página: setTemp
    refreshPage3();
    break;
  case 4: // página: configA
    refreshPage4();
    break;
  case 5: // página: configB                
    refreshPage5();
    break;        
  case 6: // página: configC
    refreshPage6();
    break;
  case 7: // página: configD
    refreshPage7();
    break;
  case 8: // página: configE
    refreshPage8();
    break;            
  case 9: // página: configF
    refreshPage9();
    break;
  case 10: // página: Teclado
    refreshPage10();        
    break;
  case 11: // página: numpad
    refreshPage11();
    break;           
  case 12: // página: peso1Met1
    refreshPage12();
    break;
  case 13: // página: peso1Met2
    refreshPage13();
    break;
  case 14: // página: configDHTtemp
    refreshPage14();
    break;
  case 15: // página: configDHThum
    refreshPage15();
    break;
  case 16: // página: peso2Met1
    refreshPage16();
    break;
  case 17: // página: peso2Met2
    refreshPage17();
    break;
  case 18: // página: graficaTemp
    refreshPage18();
    break;
  case 19: // página: graficaHum
    refreshPage19();
    break;
  case 20: // página: graficaPeso1
    refreshPage20();      
    break;           
  case 21: // página: graficaPeso2
    refreshPage21();      
    break; 
  case 22: 
    refreshPage22();      
    break;     
  case 23: 
    refreshPage23();      
    break;     
  case 24: 
    refreshPage24();      
    break;     
  case 25: 
    refreshPage25();      
    break;    
  case 26: 
    refreshPage26();      
    break;     
  case 27: 
    refreshPage27();      
    break;    
  case 28: 
    refreshPage28();      
    break;     
  case 29: 
    refreshPage29();      
    break;    
  case 30: 
    refreshPage30();      
    break;    
  case 31: 
    refreshPage31();      
    break; 
  case 32: 
    refreshPage32();      
    break;     
  case 33: 
    refreshPage33();      
    break;     
  case 34: 
    refreshPage34();      
    break;     
  case 35: 
    refreshPage35();      
    break;    
  case 36: 
    refreshPage36();      
    break;     
  case 37: 
    refreshPage37();      
    break;    
  case 38: 
    refreshPage38();      
    break;     
  case 39: 
    refreshPage39();      
    break;    
  case 40: 
    refreshPage40();      
    break;    
  case 41: 
    refreshPage41();      
    break; 
  case 42: 
    refreshPage42();      
    break;     
  case 43: 
    refreshPage43();      
    break;     
  case 44: 
    refreshPage44();      
    break;     
  case 45: 
    refreshPage45();      
    break;    
  case 46: 
    refreshPage46();      
    break;     
  case 47: 
    refreshPage47();      
    break;    
  case 48: 
    refreshPage48();      
    break;     
  case 49: 
    refreshPage49();      
    break;    
  case 50: 
    refreshPage50();      
    break;    
  }
} 

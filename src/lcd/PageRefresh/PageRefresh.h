/*!
 * pageRefresh.h - Easy library for Nextion Displays
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/*! Archivo separado < pageRefresh.h > para las funciones de RefreshCurrentPage(), 
 *  para no hacer demasiado largo el EasyNextionLibrary.cpp. Hay tantas funciones como
 *  número de páginas en el programa de Nextion. Así es más sencillo para editar.
 */
#ifndef PageRefresh_h
#define PageRefresh_h

  //---------------------------------------
 // Funciones de refresco de pantallas - privadas  
//----------------------------------------- 

// FUNCIONES PRIVADAS DE REFRESCO DE PAGINAS DEL NEXTION
// Se definen las acciones a ejecutar específicas para cada página
// Si tienes más páginas, añade más funciones extern void refreshPageXX()en
// este .h y en .ino
// y un case más en la función RefreshCurrentPage() en pageRefresh.cpp
// ------------------------------------------------

extern void refreshPage0();        //página: main 
extern void refreshPage1();        //página: hora 
extern void refreshPage2();        //página: setTiempo
extern void refreshPage3();        //página: setTemp
extern void refreshPage4();        //página: configA
extern void refreshPage5();        //página: configB
extern void refreshPage6();        //página: configC
extern void refreshPage7();        //página: configD
extern void refreshPage8();        //página: configE
extern void refreshPage9();        //página: configF
extern void refreshPage10();       //página: Teclado
extern void refreshPage11();       //página: numpad
extern void refreshPage12();       //página: peso1Met1
extern void refreshPage13();       //página: peso1Met2
extern void refreshPage14();       //página: configDHTtemp
extern void refreshPage15();       //página: configDHThum
extern void refreshPage16();       //página: peso2Met1
extern void refreshPage17();       //página: peso2Met2
extern void refreshPage18();       //página: graficaTemp
extern void refreshPage19();       //página: graficaHum
extern void refreshPage20();       //página: graficaPeso1
extern void refreshPage21();       //página: graficaPeso2 
extern void refreshPage22(); 
extern void refreshPage23(); 
extern void refreshPage24(); 
extern void refreshPage25(); 
extern void refreshPage26(); 
extern void refreshPage27(); 
extern void refreshPage28(); 
extern void refreshPage29(); 
extern void refreshPage30(); 
extern void refreshPage31();     
extern void refreshPage32(); 
extern void refreshPage33(); 
extern void refreshPage34(); 
extern void refreshPage35(); 
extern void refreshPage36(); 
extern void refreshPage37(); 
extern void refreshPage38(); 
extern void refreshPage39(); 
extern void refreshPage40(); 
extern void refreshPage41();     
extern void refreshPage42(); 
extern void refreshPage43(); 
extern void refreshPage44(); 
extern void refreshPage45(); 
extern void refreshPage46(); 
extern void refreshPage47(); 
extern void refreshPage48(); 
extern void refreshPage49(); 
extern void refreshPage50(); 

// declaration of the function as weak
//_weak function are methods that can be overwritten by user function with same name, used to define vector tables, and default handlers

extern void refreshPage0() __attribute__((weak));        //página: main 
extern void refreshPage1() __attribute__((weak));        //página: hora 
extern void refreshPage2() __attribute__((weak));        //página: setTiempo
extern void refreshPage3() __attribute__((weak));        //página: setTemp
extern void refreshPage4() __attribute__((weak));        //página: configA
extern void refreshPage5() __attribute__((weak));        //página: configB
extern void refreshPage6() __attribute__((weak));        //página: configC
extern void refreshPage7() __attribute__((weak));        //página: configD
extern void refreshPage8() __attribute__((weak));        //página: configE
extern void refreshPage9() __attribute__((weak));        //página: configF
extern void refreshPage10() __attribute__((weak));       //página: Teclado
extern void refreshPage11() __attribute__((weak));       //página: numpad
extern void refreshPage12() __attribute__((weak));       //página: peso1Met1
extern void refreshPage13() __attribute__((weak));       //página: peso1Met2
extern void refreshPage14() __attribute__((weak));       //página: configDHTtemp
extern void refreshPage15() __attribute__((weak));       //página: configDHThum
extern void refreshPage16() __attribute__((weak));       //página: peso2Met1
extern void refreshPage17() __attribute__((weak));       //página: peso2Met2
extern void refreshPage18() __attribute__((weak));       //página: graficaTemp
extern void refreshPage19() __attribute__((weak));       //página: graficaHum
extern void refreshPage20() __attribute__((weak));       //página: graficaPeso1
extern void refreshPage21() __attribute__((weak));       //página: graficaPeso2 
extern void refreshPage22() __attribute__((weak));
extern void refreshPage23() __attribute__((weak));
extern void refreshPage24() __attribute__((weak));
extern void refreshPage25() __attribute__((weak));
extern void refreshPage26() __attribute__((weak));
extern void refreshPage27() __attribute__((weak));
extern void refreshPage28() __attribute__((weak));
extern void refreshPage29() __attribute__((weak));
extern void refreshPage30() __attribute__((weak));
extern void refreshPage31() __attribute__((weak));
extern void refreshPage32() __attribute__((weak));
extern void refreshPage33() __attribute__((weak));
extern void refreshPage34() __attribute__((weak));
extern void refreshPage35() __attribute__((weak));
extern void refreshPage36() __attribute__((weak));
extern void refreshPage37() __attribute__((weak));
extern void refreshPage38() __attribute__((weak));
extern void refreshPage39() __attribute__((weak));
extern void refreshPage40() __attribute__((weak));
extern void refreshPage41() __attribute__((weak));
extern void refreshPage42() __attribute__((weak));
extern void refreshPage43() __attribute__((weak));
extern void refreshPage44() __attribute__((weak));
extern void refreshPage45() __attribute__((weak));
extern void refreshPage46() __attribute__((weak));
extern void refreshPage47() __attribute__((weak));
extern void refreshPage48() __attribute__((weak));
extern void refreshPage49() __attribute__((weak));
extern void refreshPage50() __attribute__((weak));


#endif

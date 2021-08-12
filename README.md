# NextFil-Dehydrator
Smart filament dehydrator software with Nextion screens and Arduino based boards.

Nota: Este proyecto se desarrolla en ambos idiomas español e inglés, siendo el primero el idioma primario. \
*Note: This project is developed in both Spanish and English language, being the first one the primary one.*

## Objetivo
El objetivo de este proyecto es el desarrollo tando del hardware como del software para convertir un deshidratador de comida en un deshidratador adaptado a las necesidades de la desecación de filamento para la impresión 3D. Como objetivo en la sombra es colaborar al maravilloso ecosistema del Open Source o código abierto. Por ello estáis todos bienvenidos a comentar mi trabajo, colaborar en el propio desarrollo de código e ideas, y por supuesto probarlo y descubrir fallos y posibles mejoras del programa.

## Funciones básicas
- Resistencia. Control PID de la resistencia
- Ventilador. Control del ventilador AC de recirculación de aire en la cámara
- Sensor de temperatura y humedad (interior y exterior)
- Temporizador (RTC) 
- Programas personalizados y por defecto (temperatura y tiempo), EEPROM
- Báscula. Células de presión para el pesaje del filamento, corrección de temperatura.
- LED. Control de LED Neopixel
- Alarma. Señal acústica (bienvenida, programa, error y emergencia)
- Sensor de apertura de la tapa
- Puerto de serie, depuración
- Pantalla inteligente Nextion 3,5"
- Ploteo de gráficas. Temperatura, humedad y peso.
- Soporte multi lenguaje
- Posibilidad en un futuro de soportar otro tipo de pantalla más asequible
- Posibilidad de incorporar un RTC externo (el RTC usado es de la propia pantalla Nextion)

## Electrónica
Como procesador del programa usaremos un Arduino Pro Mini, como interfaz usaremos una pantalla inteligente Nextion y como hardware una placa PCB desarrollada especialmente para este proyecto y una serie de piezas 3D que serviran para el montaje en deshidratadores convencionales.

El hardware se basa en un Arduino Mini Pro 328 de 5V. Se utilizan la resistencia y el ventilador original que viene con el deshidratador comercial, ambos funcionando con AC o corriente alterna a 230V, ambos se controlan con dos relés de estado sólido SSR, con el amperaje adecuado. En la placa base se conectan todos los sensores y se deja la posibilidad de conectar también un RTC externo en caso que se use una pantalla sin RTC. La pantalla será en principio una Nextion, con una interfaz personalizada para esta aplicación. Tendremos dos sensores de humedad y temperatura para el interior y el exterior de la cámara. En principio podemos tener hasta 2 básculas (para 2 rollos de filamento), cada una controlada con un módulo HX711. Opcionalmente podemos incorporar luces LED programables (WS2812 o similar). 

[Enlace a la PCB](https://oshwlab.com/javicarrera/deshidratador-filamento_v1-1)

## Software
Estamos ante un proyecto que necesita 2 firmwares, uno de proceso y otro para la UI de la pantalla. Se intenta separar la funcionalidad de tal manera que el procesador de la pantalla se encargue solamente de los aspectos de la UI y que el módulo de Arduino procese el resto. De esta manera la comunicación entre uno y otro será exclusivamente de datos (en cuanto esto sea posible). Se prevee la posibilidad de tener otro tipo de pantalla -más asequible- en un futuro.
La interfaz o UI es totalmente personalizada y creada específicamente para este proyecto.

## Piezas impresas
En la fase de conversión del deshidratador comercial es necesario hacer cambios en la propia estructura del aparato, por ello es necesario diseñar todas las piezas necesarias para hacerlo funcional. Se intentará hacer dichas piezas modulares para que se puedan adaptar a la mayoria de aparatos que hay en el mercado. En mi caso y como prototipo utilizaré la versión más común y barata, que tiene forma cilíndrica con toda la electrónica en la parte inferior.

## Seguridad
Un apartado muy importante es el de las medidas de seguridad, ya que el proyecto utiliza corriente alterna a 230V y tiene una resistencia para calentar el aire. Por ello hay que hacer especial hincapié tanto en el software como el hardware. 

## Método de trabajo
Todo el proyecto se desarrolla en el entorno GitHub con licencia europea EUPL v1.2 de código abierto. Para programar utilizo Visual Studio Code y Platformio. Para el desarrollo de la interfaz utilizo el programa propio de Nextion. Los esquemas eléctronicos y diseño de PCB en EasyEDA.



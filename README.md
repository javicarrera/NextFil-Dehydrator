# NextFil-Dehydrator
Smart filament dehydrator software with Nextion screens and Arduino based boards.

Nota: Este proyecto se desarrolla en ambos idiomas español e inglés, siendo el primero el idioma primario. \
*Note: This project is developed in both Spanish and English language, being the first one the primary one.*

## Objetivo
El objetivo de este proyecto es el desarrollo tando del hardware como del software para convertir un deshidratador de fruta en un deshidratador adaptado a las necesidades de la desecación de filamento para impresoras 3D. Como objetivo en la sombra es colaborar al maravilloso ecosistema del Open Source. Por ello sois todos bienvenidos a comentar mi trabajo y colaborar en el propio desarrollo de código e ideas, y por supuesto de fallos del programa.

## Hardware
Como procesador del programa usaremos un Arduino Pro Mini, como interfaz usaremos una pantalla inteligente Nextion y como hardware una placa PCB desarrollada especialmente para este proyecto y una serie de piezas 3D que serviran para el montaje en deshidratadores convencionales.

Link a la PCB:
https://oshwlab.com/javicarrera/deshidratador-filamento_v1-1

## Software
Estamos ante un proyecto que necesita 2 firmwares, uno de proceso y otro de UI para la pantalla. Se intenta separar la funcionalidad de tal manera que el procesador de la pantalla se encargue solamente de los aspectos de la UI y que el módulo de Arduino procese el resto. De esta manera la comunicación entre uno y otro será exclusivamente de datos (en cuanto esto sea posible). Se prevee la posibilidad de tener otro tipo de pantalla -más asequible- en un futuro.
La interfaz o UI es totalmente personalizada y creada específicamente para este proyecto.

## Seguridad
Un apartado muy importante es el de las medidas de seguridad, ya que el proyecto utiliza corriente alterna a 230V y tiene una resistencia para calentar el aire.

## Método de trabajo
Todo el proyecto se desarrolla en GitHub con licencia EUPL v1.2 (open source). Para programar utilizo Visual Studio Code y Platformio.

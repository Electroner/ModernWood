# ModernWood keyboard ![Generic badge](https://img.shields.io/badge/Version-b2.0-brightgreen.svg) ![Generic badge](https://img.shields.io/github/last-commit/Electroner/ModernWood)


En este repositorio encontrarás desde los datasheet de cada parte que he usado hasta los archivos originales del AutoCAD, con todos los archivos descargados deberías ser capaz de poder crear y manufacturar este teclado sin ningún problema.

Este teclado usa un ESP32-S3 como controlador y un decodificador/Demultiplexor 74HC154D 4-a-16, Suficiente para conseguir un tiempo de polling igual o superior a los mejores teclados comerciales actuales (20kHz). Pruebas realizadas con el código actual. Además se ha decidido "prescindir" de un teclado numérico real, y lo que se ha hecho es conectar los números del numpad a los números de la parte superior del teclado, por lo que las teclas tanto del numpad como los de la parte superior son los mismos, al igual con el intro y "." (Ver Esquemática).

Para el cuerpo del teclado se ha optado por omitir de forma completa el Plate o panel superior, ya que dificulta el mantenimiento y el montaje. Se ha optado solo por poner una ventana de metacrilato en la parte de la electrónica para cubrirla de forma semicompleja y protegerla de golpes. La base del teclado es de madera. Esta será la que más guste al usuario, aunque se recomiendan jatoba, sucupira o mansonia.

Este además dispone de una pantalla donde se encontraran todas las configuraciones del teclado, además de la opción para habilitar o deshabilitar el funcionamiento por bluetooth.

## Imagenes

![TECLADO](https://github.com/Electroner/ModernWood/blob/main/Images/ModernLogo.png)

Vista Previa de la PCB:

![PLANO](https://github.com/Electroner/ModernWood/blob/main/Images/PCB/Board.png)

Schematic:

![PLANO](https://github.com/Electroner/ModernWood/blob/main/Images/PCB/Schematic.png)

### Piezas (Electronica)

```plaintext
$(curl -sSL https://raw.githubusercontent.com/Electroner/ModernWood/main/Electronics/PARTS.txt)
````

## Recursos Usados

-   [Alguna Investigacion y aprendizaje](https://github.com/w4ilun/pocket-keyboard)

Y varias paginas para datos concretos como funciones en eagle,ayuda con el bootloader y ejemplos de otros teclados.

-   [Ayuda con el bootloader](https://forum.arduino.cc/t/burning-bootloader-to-custom-board-atmega32u4/890015)
-   [Ayuda con el N-key](https://forum.arduino.cc/t/how-to-program-n-key-rollover-atmega32u4/938418)

## Software Usado

-   [Editor de Layaout Teclado](http://www.keyboard-layout-editor.com/)
-   [Autocad](https://www.autodesk.es/products/autocad/overview?term=1-YEAR&tab=subscription)
-   [FreeCad](https://www.freecadweb.org/)
-   [Eagle](https://www.autodesk.com/products/eagle/free-download)
-   [Componentes Eagle](https://componentsearchengine.com/)
-   [Produccion de la PCB](https://jlcpcb.com/)
-   [Compra de los componentes](https://lcsc.com/)
-   [Corte de metacrilato (Tienda Local)](https://ecoplasticlaser.com/)


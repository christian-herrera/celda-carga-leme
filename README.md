<h1 align="center">Celda de Carga<br/><br/>
<div align="center">
<img src="assets/images/leme.svg" style="max-width: 100%" width=200><br/>
<br/>


![GitHub last commit](https://img.shields.io/github/last-commit/christian-herrera/osc01-leme)
![Static Badge](https://img.shields.io/badge/version-v0.1-blue)


![GitHub License](https://img.shields.io/github/license/christian-herrera/osc01-leme?style=for-the-badge)





</div></h1>


# Descripción
El objetivo de este proyecto es crear una app que permita leer en vivo los valores de una celda de carga. La app se desarrollará con *App-Designer*, entorno de desarrollo del software **MatLab**. Para el Hardware se opta por un Arduino Nano en conjunto con el modulo *HX711*.

La App deberá ser capaz de realizar las siguientes acciones:
- Conectarse por puerto serie al Arduino, permitiendo elegir los bps y el puerto a utilizar.
- Configurar constantes necesarias para la utilización del modulo acondicionador.
- Leer parámetros guardados en la EEPROM del microcontrolador.
- Realizar calibraciones manuales o automáticas.
- Iniciar y detener el registro de valores, mostrando siempre el ultimo en un label especifico y agregando el mismo como un punto mas de la gráfica.
- Exportar e importar los valores registrados.
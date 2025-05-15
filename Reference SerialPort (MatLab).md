# Funciones de la Comunicación Serial
## Configuración del puerto
```matlab
s = serialport("COM3", 9600, "Timeout", 10);
configureTerminator(s, "LF");       % '\n'
configureTerminator(s, "CR");       % '\r'
```

## Limpieza de buffers
```matlab
flush(app.serial.connection, "input");
flush(app.serial.connection, "output");
```

## Enviar datos
```matlab
write(s, "Hola", "string")  % Envía un string
writeline(s, "LED ON")		% Envía con el terminador
```

## Recepcion de datos
```matlab
data = read(s, 5, "uint8")	% Recibe uint8_t
readline()					% Recibe string
readbytes()					% Lee todos los bytes disponibles
```

## Enviar y Recibir
Esta funcion envia el string `cmd1`, le adiciona el *terminator* configurado en la conexión, y espera la respuesta durante el *Timeout* configurado. Esta respuesta deberá tener el mismo *terminator*.
```matlab
response = writeread(s, "cmd1");
```
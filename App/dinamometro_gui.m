function dinamometro_gui
    % Crear GUI
    fig = uifigure('Name', 'Dinamómetro', 'Position', [100 100 400 300]);

    % Variables compartidas
    s = [];
    datos = [];
    tStart = 0;
    guardando = false;

    % ---- Botones ---- %
    btnConectar = uibutton(fig, 'Text', 'Conectar', ...
        'Position', [20 240 100 30], ...
        'ButtonPushedFcn', @(btn,event) conectar());

    btnTara = uibutton(fig, 'Text', 'Tara', ...
        'Position', [140 240 100 30], ...
        'ButtonPushedFcn', @(btn,event) tara());

    btnCalibrar = uibutton(fig, 'Text', 'Calibración', ...
    'Position', [260 240 100 30], ...
    'ButtonPushedFcn', @(btn,event) calibrar());

    btnCalibrarMan = uibutton(fig, 'Text', 'Calibración Man', ...
    'Position', [260 190 100 30], ...
    'ButtonPushedFcn', @(btn,event) calibrarMan());

    btnIniciar = uibutton(fig, 'Text', 'Iniciar adquisición', ...
        'Position', [20 190 220 30], ...
        'ButtonPushedFcn', @(btn,event) iniciarAdquisicion());

    btnDetener = uibutton(fig, 'Text', 'Detener', ...
        'Position', [20 150 220 30], ...
        'ButtonPushedFcn', @(btn,event) detenerAdquisicion());

    btnGraficar = uibutton(fig, 'Text', 'Generar figura', ...
        'Position', [20 100 220 30], ...
        'ButtonPushedFcn', @(btn,event) graficar());

    % Función conectar
    function conectar()
        puerto = 'COM9';  % Cambiar si es necesario
        baud = 57600;
        try
            s = serialport(puerto, baud);
            configureTerminator(s, "LF");
            flush(s);
            uialert(fig, 'Conectado al Arduino.', 'Éxito');
        catch
            uialert(fig, 'Error al conectar.', 'Error');
        end
    end

    % Función para enviar comando por serial
    function enviarComando(cmd)
        if isempty(s)
            uialert(fig, 'No conectado.', 'Error');
            return;
        end
        writeline(s, cmd);
    end

    % Función para iniciar adquisición
    function iniciarAdquisicion()
        flush(s);
        if isempty(s)
            uialert(fig, 'No conectado.', 'Error');
            return;
        end
        datos = [];
        tStart = tic;
        guardando = true;
        writeline(s, 'g');
        configureCallback(s, "terminator", @leerSerial);
    end

    % Función para leer datos entrantes
    function leerSerial(src, ~)
        if ~guardando, return; end
        linea = readline(src);
        
        kg = str2double(linea);
        t = toc(tStart);
        if ~isnan(kg)
            datos(end+1,:) = [t, kg]; %#ok<AGROW> 
            disp([t, kg])
        end

    end

    % Función para detener adquisición
    function detenerAdquisicion()
        if isempty(s), return; end
        guardando = false;
        writeline(s, 's');
        % Guardar CSV
        if ~isempty(datos)
            nombre = ['datos_' datestr(now, 'yyyymmdd_HHMMSS') '.csv'];
            [file, path] = uiputfile('*.csv', 'Guardar datos como', nombre);
            if ischar(file)
                fullfileOut = fullfile(path, file);
                T = array2table(datos, 'VariableNames', {'Tiempo_s', 'Fuerza_kg'});
                writetable(T, fullfileOut);
                uialert(fig, ['Datos guardados en: ' fullfileOut], 'Guardado');
            end
        end
    end

    % Función para graficar
    function graficar()
        if isempty(datos)
            uialert(fig, 'No hay datos.', 'Aviso');
            return;
        end
        f = figure;
        plot(datos(:,1), datos(:,2), 'b', 'DisplayName', 'kg'); hold on;
        xlabel('Tiempo [s]');
        ylabel('Fuerza');
        legend;
        grid on;
        title('Medición de fuerza en tiempo real');
    end

    function tara()
    if isempty(s)
        uialert(fig, 'No conectado.', 'Error');
        return;
    end
    flush(s);               % Limpiamos buffer por si hay basura
    writeline(s, 't');      % Enviamos comando
    pause(0.2);             % Esperamos breve tiempo para recibir respuesta
    if s.NumBytesAvailable > 0
        respuesta = readline(s);  % Leemos la respuesta
        uialert(fig, ['Arduino dice: ' strtrim(respuesta)], 'Tara');
    else
        uialert(fig, 'Sin respuesta del Arduino.', 'Aviso');
    end
end
function calibrar()
    if isempty(s)
        uialert(fig, 'No conectado.', 'Error');
        return;
    end
    flush(s);
    writeline(s, 'c');
    pause(1);  % Ajustá si el Arduino necesita más tiempo
    if s.NumBytesAvailable > 0
        respuesta = readline(s);
        uialert(fig, ['Arduino dice: ' strtrim(respuesta)], 'Calibración');
    else
        uialert(fig, 'Sin respuesta del Arduino.', 'Aviso');
    end

    while s.NumBytesAvailable == 0
        pause(0.1);
    end
    pause(1);
    respuesta = readline(s);
    uialert(fig, ['Arduino dice: ' strtrim(respuesta)], 'Calibración');

end


    function calibrarMan()
    if isempty(s)
        uialert(fig, 'No conectado.', 'Error');
        return;
    end
    flush(s);
    writeline(s, 'm');
    pause(1);  % Ajustá si el Arduino necesita más tiempo
    if s.NumBytesAvailable > 0
        respuesta = readline(s);
        uialert(fig, ['Arduino dice: ' strtrim(respuesta)], 'Calibración');
    else
        uialert(fig, 'Sin respuesta del Arduino.', 'Aviso');
    end

end


end

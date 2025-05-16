classdef utilidades
    methods(Static)

        function val = processRawData(str, expectedResponse)
            try
                % Verifico que contenga el '$' y que inicie con '#' y termine con '&'
                if ~contains(str, "$") || ~startsWith(str, "#") || ~endsWith(str, "&")
                    e.identifier = "progressRawData:NotValidResponse";
                    e.message = "Respuesta no válida: [" + str + "]";	
                    error(e);
                end

                % Quito el '#' y '&' de la respuesta y separo en dos partes usando el caracter '$'
                str = extractBetween(str, 2, strlength(str)-1);
                parts = split(str, "$");

                % Verifico la parte 1 que es el identificador
                if parts{1} ~= expectedResponse
                    e.identifier = "progressRawData:NotExpectedResponse";
                    e.message = "La respuesta no es la esperada";
                    error(e);
                end

                % Tomo el valor de la parte 2 y lo convierto a float
                val = str2double(parts{2});
                if isnan(val)
                    e.identifier = "progressRawData:checkValString";
                    e.message = "Respuesta no válida";
                    error(e);
                end
            catch e
               val = NaN;
            %    disp(e);
            end
        end





        function val = getParamFloat(serial, cmd, expectedResponse)
            % Envía un comando al serial y espera una respuesta con un cierto identificador
            % La respuesta tiene la estructura: '#IDENTIFICADOR$<valor float>&'
            % Si la respuesta no es válida devuelve NaN
            flush(serial, "input");
            flush(serial, "output");
            try
                % Envio el comando al serial
                resp = writeread(serial, cmd);
                % disp(resp);

                val = utilidades.processRawData(resp, expectedResponse);
                if isnan(val)
                    e.identifier = "getParamFloat:NotValidResponse";
                    e.message = "Respuesta no válida";
                    error(e);
                end

            catch e
               val = NaN;
            %    disp(e);
            end
        end



        function result = checkResponseFromSerial(serial, cmd, expectedResponse)
            flush(serial, "input");
            flush(serial, "output");
            % Envía un comando al serial y espera la respuesta que debe ser igual a la esperada
            try
                resp = writeread(serial, cmd);
                result = strcmp(resp, expectedResponse);
            catch e
               result = false;
               disp(e);
            %    disp("Error[checkResponseFromSerial]: " + e.message);
            end
        end
    end
end

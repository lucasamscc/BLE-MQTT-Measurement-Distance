# Sistema de Localização com Bluetooth Low Energy (BLE) e MQTT

Este projeto foi desenvolvido como trabalho final da disciplina de Redes do curso Tecnologia em Análise e Desenvolvimento de Sistemas, ofertado pela Universidade do Estado de Santa Catarina (UDESC), Joinville – SC – Brasil. 

Utiliza um microcontrolador ESP32 para criar um sistema de localização de objetos em um raio curto de até 10 metros, utilizando Bluetooth Low Energy (BLE). A presença do objeto é monitorada com base na intensidade do sinal RSSI e conecta-se à Internet das Coisas (IoT) via protocolo MQTT para comunicação e controle remoto.

## Funcionalidades

- **Monitoramento de Presença**: O sistema monitora a presença de objetos dentro do alcance de até 10 metros, utilizando o sinal RSSI para estimar a proximidade.
  
- **Comunicação via MQTT**: Utiliza o protocolo MQTT para enviar informações sobre a proximidade do objeto para um servidor MQTT centralizado, permitindo integração com outros dispositivos IoT.

## Componentes Utilizados

- Microcontrolador ESP32, que suporta BLE e WiFi integrados.
- Servidor MQTT para comunicação e controle remoto.

## Instalação e Configuração

1. **Pré-requisitos**:
   - Placa ESP32 compatível.
   - Ambiente de desenvolvimento Arduino IDE configurado com suporte ao ESP32.

2. **Configuração**:
   - Configure as credenciais WiFi (`ssid` e `password`) no código.
   - Ajuste os parâmetros de RSSI conforme necessário para seu ambiente.
   - Configure as variáveis MQTT (`mqttBroker`, `mqttUsername` e `mqttPassword`) com as informações do seu servidor MQTT.

3. **Instalação**:
   - Clone ou faça o download do repositório.
   - Abra o arquivo `.ino` no Arduino IDE.
   - Carregue o código para o seu ESP32.

## Uso

- Conecte o ESP32 à rede WiFi. O endereço IP será exibido no Monitor Serial.
- O sistema monitorará a presença de objetos dentro do alcance definido.
- Os dados de proximidade são enviados para o servidor MQTT configurado, permitindo monitoramento e controle remoto.

## Contribuições

Contribuições são bem-vindas! Para sugestões, melhorias ou correções, sinta-se à vontade para abrir uma issue ou enviar um pull request.

## Licença

Este projeto é licenciado sob a [MIT License](https://opensource.org/licenses/MIT).

Rega Automática com Arduíno

Rega com possibilidade de inclusão, por meio do usuário, dos tempos entre uma rega e outra e também, tempo em que a rega ficará ativa.

Materiais utilizados:
1x Arduíno UNO
1x Módulo Joystick
1x Tela LCD 16x2 com módulo IC2 
1x Touch Sensor (pode ser um botão)
1x Módulo Relay 1-way
1x Válvula Solenoide para água
Cabos em Geral


Conexões:
04 - Touch Sensor
10 - Módulo Relay
A1 - Vrx joystick
A2 - Vry joystick
A5 - SCL (LCD IC2)
A4 - SDA (LCD IC2)


Projeto se inicia mostrando na tela inicial a configuração, que pode ser alterada pelo usuário por meio do joystick
Joystick para cima aumenta o tempo em que a rega ficará ativa (com a válvula aberta regando). Tempo dado em Segundos
Joystick para baixo diminui esse tempo. Tempo dado em Segundos
Joystick para a direita aumenta o tempo entre uma rega em outra. Tempo dado em Minutos
Joystick para a esquerda diminui este tempo. Tempo dado em Minutos

após clicado o botão, em um primeiro momento será contabilizado no tempo até a próxima rega o tempo ANTERIORMENTE definido, isto fora desenvolvido para não haver conflitos entre uma configuração em meio a um intervalo de rega e o tempo já corrido.
Abaixo da tela ficada exposta a configuração atual e acima o tempo até a próxima rega, em minutos.
após a chegada em 01 (um) minuto, este valor inicia uma contagem regressiva de 60 a 0 para dar início à rega.
Quando a rega é iniciada, outra há a mudança para uma tela te contagem regressiva em segundos na segunda linha da tela, na primeira linha, fica demonstrado apenas a frase "ligado".


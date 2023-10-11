#include <Wire.h> // biblioteca que vem em conjunto com a LiquidCrystal
#include <LiquidCrystal_I2C.h> // Biblioteca para funcionamento do LCD, já tem por sua própria a escolha das portas digitais
const int valvula = 10; // porta analógica que vai até o relê
const int touch = 4; // porta analógica do botão touch
int configuracao = true; // Variável que será alterada com o click do botão
int ligado = false; // variável que será alterada com timer
int touchDigital; // variável que armazena o high ou low do botão touch
int escolhaRega = 5; // variável determinada por escolha do usuário pelo joystick, será o tempo que a válvula solenoide ficará aberta para regar
int contadorRega = escolhaRega; // variável para uso exclusivo do contador de tempo, para que não seja alterada a variável matriz(escolhaRega)
int escolhaTempo = 1; // variável determinada por escolha do usuário pelo joystick, será o tempo entre uma rega e outra
int contadorTempo = escolhaTempo; // variável para uso exclusivo do contador de tempo, para que não seja alterada a variável matriz(escolhaTempo)
int proxRega = escolhaTempo; // variável que será utilizada para mostrar ao usuário o tempo restante entre uma rega e outra
int proxRegaSec; // Variável que será utilizada para fazer a contagem dos segundos até a próxima rega
#define Vrx A1 // constante que define a porta digital do joystick de eixo X
#define Vry A2 // constante que define a porta digital do joystick de eixo Y
int vrx; // será um valor entre 0-100, definido pelo movimento do joystick em seu eixo X
int vry; // será um valor entre 0-100, definido pelo movimento do joystick em seu eixo Y
LiquidCrystal_I2C lcd(0x27, 16, 2); // Define o tamanho do LCD


void setup() {

  pinMode(valvula, OUTPUT); // RELÊ
  pinMode(touch, INPUT); // BOTÃO TOUCH
  lcd.init(); // Função da Biblioteca LiquidCrystal, para iniciar o LCD
  lcd.backlight(); // Função da Biblioteca para ligar a luz de fundo do LCD
  pinMode(Vrx, INPUT); // Joystick eixo X
  pinMode(Vry, INPUT); // Joystick eixo Y
}

int proxRegaSegundos(int n){// função para contar os segundos finais anteriores à rega (n será sempre 60)
  while (true){
            if (n>0){
              lcd.setCursor(0, 0);
              lcd.print("Prox Rega:");
              lcd.print(n);
              lcd.print("s");
              if (verificador(10) == false){
                return false;
                break;
              }
              n--;
            }
            else{
              return true;
              break;
            }
  }
}

int contador(int n){ //funçao para contar os segundos da rega (n será a escolha do usuário do tempo regando)
  while (true) {
        if (n > 0) {
          lcd.setCursor(6, 1);
          lcd.print(n);
          lcd.print(" s");  
          if (verificador(10) == false){
            return false;
            break;
          }
          n--;
        } 
        else {
          ligado = false;
          lcd.clear();
          return true;
          break;
        }
    }
}

int verificador(int n){ // função para fazer a verificação de se o botão está clicado a cada 100ms, retornando falso caso esteja clicado e true se não estiver (n será sempre 10, para fechar 1000ms ou 1s)
  while(true){
            if (touchDigital == HIGH) {
                lcd.clear();
                configuracao = true;
                return false;
                break;
            }
            if (n>0){
              delay(100);
              touchDigital = digitalRead(touch);
              n--;
            }else{
              return true;
              break;
            }
          }
}

int verificadorMinuto(int n){ // função utilizada para contar os minutos (n sempre será 60)
  while (true){
    if (n>0){
      if (verificador(10) == false){
        return false;
        break;
      }
      else{
        n--;
      }
    }
    else{
      return true;
      break;
    }
  }
}

void loop() {

  vrx = map(analogRead(Vrx), 0, 1023, 0, 101); // Muda o valor da variável eixo X de entre 0-1023 para 0-100
  vry = map(analogRead(Vry), 0, 1023, 0, 101); // Muda o valor da variável eixo Y de entre 0-1023 para 0-100
  touchDigital = digitalRead(touch); // Click do botão (high or low)



  if ((touchDigital == HIGH) && (configuracao == false)) { //ve se o botao está clicado e muda de falso para verdadeiro
    touchDigital = LOW; //abaixa instantaneamente o botão para não dar conflito
    configuracao = true; 
    lcd.clear(); 
  } 
  else if ((touchDigital == HIGH) && (configuracao == true)) {//ve se o botao está clicado e muda de verdadeiro para falso
    touchDigital = LOW; //abaixa instantaneamente o botão para não dar conflito
    configuracao = false;
    lcd.clear(); 
  }

  if (configuracao == true){ // verifica se está na tela de configuração, caso esteja, este código será rodado...
    lcd.setCursor(0,0);
    lcd.print("--Configurando--");
    lcd.setCursor(0, 1);
    lcd.print("Reg:");
    lcd.print(escolhaRega);
    lcd.print("s");
    contadorRega = escolhaRega;
    lcd.print(" Tmp:");
    lcd.print(escolhaTempo);
    lcd.print("m");

    if ((vrx >= 82) && (ligado == false)) { //verificação eixo X para cima
          escolhaRega += 1;
    } 
    if ((vrx <= 30) && (ligado == false)) { //verificação eixo X para baixo
          escolhaRega -= 1;
    } 
    if ((vry >= 52) && (ligado == false)) { //verificação eixo Y para cima
          escolhaTempo += 1;
    } 
    if ((vry <= 30) && (ligado == false)) { //verificação eixo Y para baixo
          escolhaTempo -= 1;
    }
  }
  else if (configuracao == false){ // caso não esteja na tela da configuração, o código rodado será este

    if (ligado == true) { // este código apenas será rodado apos o esgotamento do tempo entre uma rega e outra (é a função que faz regar)
      lcd.clear(); // limpa o LCD
      digitalWrite(valvula, HIGH);
      lcd.setCursor(4, 0);
      lcd.print("Ligado"); 
      delay(500); // Aguarda 0,5s para iniciar a contagem
      contador(contadorRega);
      
      proxRega = contadorTempo; // optei por trocar a escolha do usuário apenas após o encerramento do ciclo já iniciado, para não haver conflito entre o tempo ja corrido...
    } 
    else if (ligado == false) {
      contadorTempo = escolhaTempo;
      while (true){ // loop infinito para contar o tempo até a próxima rega...
        if (proxRega>0){ // verificador para quebrar ou não o loop
          digitalWrite(valvula, LOW);
          lcd.setCursor(0, 0);
          lcd.print("Prox Rega:");
          lcd.print(proxRega);
          lcd.print("m");
          lcd.setCursor(0, 1);
          lcd.print("Reg:");
          lcd.print(escolhaRega);
          lcd.print("s");
          contadorRega = escolhaRega;
          lcd.print(" Tmp:");
          lcd.print(contadorTempo);
          lcd.print("m");
          if (proxRega == 1){
            if (proxRegaSegundos(60) == false){
              break;
            }
            else{
              ligado = true;
              break;
            }
          }
          if (verificadorMinuto(60) == false){
            break;
          }
          else{
            proxRega--;
          }
        }
      }
    }
  }
  delay(200);
}

//INCLUSÃO DE BIBLIOTECA DO CONTROLE REMOTO
#include <IRremote.h> 

// PINAGEM DO MOTOR DIREITO
#define PIN_VELOCIDADE_MOTOR_DIREITO  9
#define PIN_1_MOTOR_DIREITO           11
#define PIN_2_MOTOR_DIREITO           10

// PINAGEM DO MOTOR ESQUERDO
#define PIN_VELOCIDADE_MOTOR_ESQUERDO 5
#define PIN_2_MOTOR_ESQUERDO          6
#define PIN_1_MOTOR_ESQUERDO          7

// PINAGEM DE SINAL DO RECEPTOR IR
#define RECV_PIN                      13
// TEMPO PADRÃO QUE IRA GIRAR OS MOTORES AO IREM PARA FRENTE E PARA TRÁS
#define TEMPO                         600

// VARIAVEL QUE CONTROL A VELOCIDADE. ELA PODE SER ALTERADA UTILIZANDO OS PINOS PWM DO L298N
int velocidade                        = 0;
// ESSA VARIAVEL AUXILIA PARA EQUILIBRAR A VELOCIDADE DE UM DOS MOTORES PARA O CASO DE 
//  UM DOS MOTORES RODAR MENOS QUE O OUTRO
int velocidade_ajuste                 = 20;

// ESSES SAO OS VALORES QUE APARECERAM NO CONTROLE REMOTO QUANDO EU FIZ O TESTE DE LEITURA DOS BOTOES
const long int BOTAO_CIMA             = 16736925; // HEX: FF629D
const long int BOTAO_BAIXO            = 16754775; // HEX: FFA857
const long int BOTAO_ESQUERDA         = 16720605; // HEX: FF22DD
const long int BOTAO_DIREITA          = 16761405; // HEX: FFC23D
const long int BOTAO_OK               = 16712445; // HEX: FF02FD
const long int BOTAO_1                = 16738455; // HEX: FF6897
const long int BOTAO_2                = 16750695; // HEX: FF9867
const long int BOTAO_3                = 16756815; // HEX: FFB04F
const long int BOTAO_ASTERISCO        = 16728765; // HEX: FF42BD
const long int BOTAO_0                = 16730805; // HEX: FF4AB5
const long int BOTAO_TRALHA           = 16732845; // HEX: FF52AD

// VELOCIDADE MINIMA, MEDIA E MAXIMA DOS MOTORES QUE EU DEFINI, ABAIXO DE 65 O MOTOR NAO TEM FORCA PARA RODAR
const int MIN_VELOCIDADE              = 75;
const int MED_VELOCIDADE              = 85;
const int MAX_VELOCIDADE              = 95;

// CRIA UMA INSTANCIA DA CLASSE QUE FAZ A LEITURA DO RECEPTOR IR
IRrecv irrecv(RECV_PIN); //PASSA O VALOR DO PINO PARA A FUNÇÃO irrecv

// VARIAVEL QUE SERA PASSADA COMO REFERENCIA E CONTERA O VALOR DO BOTAO PRESSIONADO PELO CONTROLE
decode_results results; //VARIAVEL QUE ARMAZENA OS RESULTADOS (SINAL IR RECEBIDO)

// ENUMERADOR PARA DEFINIR AS POSICOES
enum DIRECAO {
  FRENTE,
  TRAS,
  DIREITA,
  ESQUERDA,
  PARADO
};

DIRECAO direcao;

// MOVE PARA FRENTE E PARA
void mover_frente() {
  digitalWrite(PIN_1_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, HIGH);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  delay(TEMPO);
  parar();
}

// MOVE PARA TRAS E PARA
void mover_tras() {
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, HIGH);
  delay(TEMPO);
  parar();
}

// MOVE PARA A DIREITA E PARA
void mover_direita() {
  direcao = PARADO;
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, HIGH);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  // AQUI DEFINI DIVIDIR POR 3, CASO QUEIRA QUE O CARRO VIRE MAIS OU MENOS, DIMINUA ESSA DIVISAO POR 3 OU AUMENTE, RESPECTIVAMENTE
  delay(TEMPO / 3);
  parar();
}

// MOVE PARA A DIREITA E PARA
void mover_esquerda() {
  direcao = PARADO;
  digitalWrite(PIN_1_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  // AQUI DEFINI DIVIDIR POR 3, CASO QUEIRA QUE O CARRO VIRE MAIS OU MENOS, DIMINUA ESSA DIVISAO POR 3 OU AUMENTE, RESPECTIVAMENTE
  delay(TEMPO / 3);
  parar();
}

// PARA OS MOTORES
void parar() {
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  //delay(TEMPO);
}

void setup() {

  Serial.begin(9600); //INICIALIZA A SERIAL

  // CONFIGURA AS PINAGENS DO L298N
  // MOTOR DIREITO
  pinMode(PIN_VELOCIDADE_MOTOR_DIREITO, OUTPUT);
  pinMode(PIN_1_MOTOR_DIREITO, OUTPUT);
  pinMode(PIN_2_MOTOR_DIREITO, OUTPUT);
  // MOTOR ESQUERDO
  pinMode(PIN_VELOCIDADE_MOTOR_ESQUERDO, OUTPUT);
  pinMode(PIN_1_MOTOR_ESQUERDO, OUTPUT);
  pinMode(PIN_2_MOTOR_ESQUERDO, OUTPUT);

  // INICIO NA VELOCIDADE MINIMA
  velocidade                          = MIN_VELOCIDADE;

  // HABILITO A LEITURA DO RECEPTOR IR - IMPORTANTE, 
  // ESSA CLASSE INTERNAMENTE INVIABILIZA O USO DO PINO 3 DO ARDUINO.
  // PORTANTO NAO UTILIZE O PINO 3 LIGANDO NO L298N
  irrecv.enableIRIn(); //INICIALIZA A RECEPCAO DE SINAIS IR

}

void mover() {
  
  analogWrite(PIN_VELOCIDADE_MOTOR_DIREITO, velocidade);
  analogWrite(PIN_VELOCIDADE_MOTOR_ESQUERDO, velocidade + velocidade_ajuste);

  if (direcao == FRENTE) {
    mover_frente(); 
  } else if (direcao == TRAS) { 
    mover_tras();
  } else if (direcao == ESQUERDA) { 
    mover_esquerda();
  } else if (direcao == DIREITA) { 
    mover_direita();
  } else if (direcao == PARADO) { 
    parar();
  } 
}

void loop() {

  //CAPTURA O SINAL IR
  if (irrecv.decode(&results)) {

    // PRINTA NA SERIAL OS VALORES HEXADECIMAIS E DECIMAIS DOS BOTOES APERTADOS NO CONTROLE REMOTO
    // UTILIZE ESSES VALORES DECIMAIS PARA CONFIGURAR OS VALORES DE CADA BOTAO LA NO INICIO DO PROGRAMA
    Serial.print("Código HEX: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value, HEX); //IMPRIME EM FORMATO HEXADECIMAL
    Serial.print("Código DEC: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value, DEC); //IMPRIME EM FORMATO DECIMAL
    Serial.println(""); //QUEBRA DE LINHA NA SERIAL

    // A MINHA IDEIA AQUI ERA DEIXAR O VEICULO ANDANDO ATE ELE RECEBER O SINAL DE PARAR PELO CONTROLE.
    // PARA ISSO, RETIRE A LINHA DE DELAY() E PARAR() DENTRO DOS METODOS QUE MOVIMENTAR O MOTOR PARA FRENTE E PARA TRAS
    if (results.value == BOTAO_CIMA) {
      direcao = FRENTE;
    } else if (results.value == BOTAO_BAIXO) { 
      direcao = TRAS;
    } else if (results.value == BOTAO_ESQUERDA) { 
      direcao = ESQUERDA;
    } else if (results.value == BOTAO_DIREITA) { 
      direcao = DIREITA;
    } else if (results.value == BOTAO_OK) { 
      direcao = PARADO;
    } else if (results.value == BOTAO_1) { 
      velocidade = MIN_VELOCIDADE;
    } else if (results.value == BOTAO_2) { 
      velocidade = MED_VELOCIDADE;
    } else if (results.value == BOTAO_3) {
      velocidade = MAX_VELOCIDADE;
    } // if (results.value == BOTAO_CIMA) {

    mover();

    // LIBERA O RECEPTOR IR PARA LER UM NOVO VALOR
    irrecv.resume();

  } // if (irrecv.decode(&results))
  delay(100);
  
}
  
/* void loop() {
  if (!client.connected()) {
     Reconectar();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    value = random(20,50);
    snprintf (msg, 50, "%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Ligue4PIC", msg);
  }
} */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>

// WIFI
#define WIFI_SSID "Artur's Galaxy A52s 5G"
#define WIFI_SENHA "bffl3175"

void ConectarNoWifi();



// MQTT
#define MQTT_SERVIDOR "broker.mqtt-dashboard.com"
#define TOPICO_GRAFICOS "PIC_LIGUE_4:GRAFICOS"
#define TOPICO_JOGO "PIC_LIGUE_4:JOGO"

WiFiClient espClient;
PubSubClient client(espClient);

void RecebeInfo(char* topic, byte* payload, unsigned int length);
void ReconectarBroker();

// LIGUE 4
#define MAXLIN 6
#define MAXCOL 7
#define VAZIO ' '
#define JOGADOR1 'X'
#define JOGADOR2 'O'
#define PAREDE '|'
#define BASE '='

char tabuleiro[MAXLIN][MAXCOL];
int nJogadas = MAXLIN*MAXCOL;
char jogador = JOGADOR1;

enum EstadoJogo{
  FIM_DE_JOGO = 1,
  JOGADA_VALIDA = 0,
  JOGADA_INVALIDA = -1
};

void InicializaTabuleiro(char [MAXLIN][MAXCOL]);
void ImprimeTabuleiro(char [MAXLIN][MAXCOL]);
int JogadaValida(char [MAXLIN][MAXCOL], int col);
void InsereJogada(char [MAXLIN][MAXCOL], int col, char jogador);
EstadoJogo FazJogada(char [MAXLIN][MAXCOL], char);
int GanhouJogo(char [MAXLIN][MAXCOL], int);
int QuantosNaDirecao(char [MAXLIN][MAXCOL], int, int, int, int, char);



// IMPLEMENTAÇÃO DAS FUNÇÕES

void setup(){
  Serial.begin(9600);

  InicializaTabuleiro(tabuleiro);

  ConectarNoWifi();

  client.setServer(MQTT_SERVIDOR, 1883);
  client.setCallback(RecebeInfo);
}


void loop(){

  if (!client.connected()) {
     ReconectarBroker();
  }
  client.loop();

  EstadoJogo estadoJogo = FazJogada(tabuleiro,jogador);

  if(estadoJogo == JOGADA_INVALIDA) return;

  if(estadoJogo == FIM_DE_JOGO){
    char envio[] = {'F', jogador, ';', '\0'};

    Serial.print("JOGADOR ");
    Serial.print(jogador);
    Serial.println(" VENCEU");
    client.publish(TOPICO_JOGO, envio);
    delay(10000);
    InicializaTabuleiro(tabuleiro);
    
    


  }
  else if(!nJogadas){
    Serial.print("EMPATE!");
    delay(10000);
  }

  if(jogador==JOGADOR1) jogador=JOGADOR2;
  else jogador=JOGADOR1;
  // Serial.println("CORINTHIANS");
  
}




// WIFI

void ConectarNoWifi() {

  delay(10);

  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado!");

}




// MQTT

void RecebeInfo(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}


void ReconectarBroker() {

  while (!client.connected()) {
    Serial.print("Tentando Conexão");
    
    //Cria um clientId aleatório
    String clientId = "Ligue4Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado");
      client.publish("Ligue4PIC", "de quatro!!");
    } 
    else {
      Serial.print("falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println("Tentando novamente em 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

}




// LIGUE 4

void InicializaTabuleiro(char tabuleiro[MAXLIN][MAXCOL]){

  int i,j;
  for(i=0;i<MAXLIN;i++){
    for(j=0;j<MAXCOL;j++){
        tabuleiro[i][j]=VAZIO;
    }
  }

}


void ImprimeTabuleiro(char tabuleiro[MAXLIN][MAXCOL]){

    int i,j;
    for(i=0;i<MAXLIN;i++){
        Serial.print(PAREDE);
        for(j=0;j<MAXCOL;j++){
            Serial.print(tabuleiro[i][j]);
        }
        Serial.println(PAREDE);
    }
    for(i=0;i<9;i++) Serial.print(BASE); Serial.println("");

}


int JogadaValida(char tabuleiro[MAXLIN][MAXCOL],int col){

    return (col>=0 &&
            col<MAXCOL &&
            tabuleiro[0][col]==VAZIO);

}


void InsereJogada(char tabuleiro[MAXLIN][MAXCOL],int col,char jogador){

    int i,j;
    for(i=MAXLIN-1;i>=0;i--){
        if(tabuleiro[i][col]==VAZIO){
            tabuleiro[i][col]=jogador;
            break;
        }
    }

}


EstadoJogo FazJogada(char tabuleiro[MAXLIN][MAXCOL],char jogador){

    if(Serial.available() < 1) return JOGADA_INVALIDA;
    
    int coluna = Serial.read();
    coluna-='0';
    coluna--;

    if(JogadaValida(tabuleiro,coluna)){
      EnviaDados(jogador, tabuleiro, coluna);

      nJogadas--;
      InsereJogada(tabuleiro,coluna,jogador);

      ImprimeTabuleiro(tabuleiro);

      //Inverte Jogador

      if(GanhouJogo(tabuleiro,coluna)){
        return FIM_DE_JOGO;
      }else return JOGADA_VALIDA;

    }
    else return JOGADA_INVALIDA;

}

int GanhouJogo(char tabuleiro[MAXLIN][MAXCOL],int coluna){

    int i;
    char peca;
    //acha peça
    for(i=0;i<MAXLIN;i++){
        if(tabuleiro[i][coluna]!=VAZIO){
            peca = tabuleiro[i][coluna];
            break;
        }
    }
    int count;
    
    //horizontal
    count = QuantosNaDirecao(tabuleiro,0,1,i,coluna,peca);
    //printf("Horizontal = %d\n",count);
    if(count >=4) return 1;

    //vertical
    count = QuantosNaDirecao(tabuleiro,1,0,i,coluna,peca);
    //printf("Vertical = %d\n",count);
    
    if(count>=4)return 1;
    

    //diagonal negativa
    count = QuantosNaDirecao(tabuleiro,1,1,i,coluna,peca);
    //printf("Diagonal negativa = %d\n",count);
    if(count>=4)return 1;


    //diagonal positiva
    count = QuantosNaDirecao(tabuleiro,1,-1,i,coluna,peca);
    //printf("Diagonal positiva = %d\n",count);
    if(count>=4)return 1;

    return 0;

}


int QuantosNaDirecao(char tabuleiro[MAXLIN][MAXCOL], int deltaI, int deltaJ, int posI, int posJ, char jogador){

    int i,j, count = 0, reverse = 0;
    for(i=0, j=0;; i+=deltaI, j+=deltaJ){

        int novoI = posI+i, novoJ = posJ+j;

        int fora = (novoI<0 || novoI>=MAXLIN) || (novoJ<0 || novoJ>=MAXCOL);

        if(tabuleiro[novoI][novoJ] != jogador || fora){
            if(reverse) return count;
            reverse = 1;
            i = 0; j = 0;
            deltaI = -deltaI;
            deltaJ = -deltaJ;

        }else{
            count++;
        }
    }
    
}

void EnviaDados(char jogador, char tabuleiro[MAXLIN][MAXCOL], int coluna){
  char dados[47];

  sprintf(dados, "J");

  for(int i=0; i<MAXLIN; i++){
    for(int j=0; j<MAXCOL; j++){
      sprintf(dados, "%s%c", dados, tabuleiro[i][j]);
    }
  }
  sprintf(dados, "%s;%d%c", dados, coluna, jogador);

  client.publish(TOPICO_JOGO, dados);
}
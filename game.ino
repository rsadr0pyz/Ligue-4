#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
// ###################### Trocar aqui!!! ##################### 
const char* ssid = "PIC2-2.4G";
const char* password = "engcomp@ufes";
//Free mqtt server for testing
const char* mqtt_server = "broker.mqtt-dashboard.com";
//Local MQTT server - Tested using mosquitto mqtt for windows and linux
//const char* mqtt_server = "192.168.137.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Ligue4PIC", "de quatro!!");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
     reconnect();
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
}

#define MAXLIN 6
#define MAXCOL 7
#define VAZIO ' '
#define JOGADOR1 'X'
#define JOGADOR2 'O'
#define PAREDE '|'
#define BASE '='

void InicializaTabuleiro(char [MAXLIN][MAXCOL]);
void ImprimeTabuleiro(char [MAXLIN][MAXCOL]);
int JogadaValida(char [MAXLIN][MAXCOL], int col);
void InsereJogada(char [MAXLIN][MAXCOL], int col, char jogador);
int FazJogada(char [MAXLIN][MAXCOL], char);
int GanhouJogo(char [MAXLIN][MAXCOL], int);
int QuantosNaDirecao(char [MAXLIN][MAXCOL], int, int, int, int, char);

char tabuleiro[MAXLIN][MAXCOL];
int nJogadas = MAXLIN*MAXCOL;

void setup(){
  InicializaTabuleiro(tabuleiro);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

char jogador = JOGADOR1;
/* void loop(){

  
  if(Serial.available()>0){
    int acabouJogo = FazJogada(tabuleiro,jogador);
    nJogadas--;

    if(acabouJogo){
      Serial.print("JOGADOR ");
      Serial.print(jogador);
      Serial.println(" VENCEU");
      delay(10000);
    }
    else if(!nJogadas){
      Serial.print("EMPATE!");
      delay(10000);
    }

    if(jogador==JOGADOR1) jogador=JOGADOR2;
    else jogador=JOGADOR1;

    //Serial.println("CORINTHIANS");
  }
  
} */


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

int FazJogada(char tabuleiro[MAXLIN][MAXCOL],char jogador){
    int coluna;

    while(1){
      coluna=Serial.read();
      coluna-='0';
      coluna--;
      delay(1);

      if(JogadaValida(tabuleiro,coluna)) break;
    }
   
    InsereJogada(tabuleiro,coluna,jogador);

    ImprimeTabuleiro(tabuleiro);

    return GanhouJogo(tabuleiro,coluna);
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

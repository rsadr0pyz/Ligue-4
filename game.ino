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
}

char jogador = JOGADOR1;
void loop(){

  
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
  
}


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
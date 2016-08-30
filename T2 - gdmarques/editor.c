#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <string>
using namespace std;

#if defined(__linux__) || defined(__APPLE__)

#include <termios.h>



struct Editor{
  char *nome;     // nome do arquivo de entrada
  char **linhas;  // as linhas do texto
  int nlinhas;    // total de linhas do texto
  int lin1;       // linha do texto na 1a linha da tela 
  int col1;       // coluna do texto na 1a coluna da tela
  int lin2;       // linha do texto na última linha da tela
  int col2;       // coluna do texto na última coluna da tela
  // Abaixo estão as funções básicas do editor.
  // Se achar necessário, pode-se inserir outras funções.

  int MAX=0;


  void strlen(string str)
{
    int total=0;

    while( str[total] != '\0')
        total++;
    if(total>MAX)MAX=total;
}

  // inicia o editor
  void inicia(const char* nome){
lin1=0;
lin2=10;
col1=0;
col2=70;
nlinhas=0;

string line;

  ifstream arquivo (nome); // ifstream = padrão ios:in

  if(arquivo.is_open()){

    while (! arquivo.eof() ) //enquanto end of file for false continua

    {

      getline (arquivo,line); // como foi aberto em modo texto(padrão)
      strlen(line);
      nlinhas++;   
    } arquivo.close();

  }else cout << "Unable to open file"; 
linhas = new char*[MAX];
  for(int i=0; i<nlinhas;i++){
	linhas[i] = new char[MAX];
	}
}

  // destroi o editor
  void destroi(void){
for(auto i = 0; i < nlinhas; i++)
    delete[] linhas[i];
    delete[] linhas;
}

  // desenha o texto na tela
  void desenha(void){
    #if defined(__linux__)
    system("clear");
    #endif	
    for(int l=lin1; l<lin2&&l<nlinhas-1; l++){
  
      for(int c=col1;c<col2 && linhas[l][c]!='\0' && linhas[l][c]!='\n';c++){
	cout<<linhas[l][c];
       	
	}
	cout<<endl;
     
    }

  }

  // le um arquivo 'nome' e armazena na estrutura de dados
  void le_arquivo(const char* nome){
string line;
int aux=0;
  ifstream arquivo (nome); // ifstream = padrão ios:in

  if(arquivo.is_open()){

    while (! arquivo.eof() ) //enquanto end of file for false continua

    {

      getline (arquivo,line);
      
	for(int i=0; line[i]!= '\0';i++){
	linhas[aux][i]=line[i];
	if(line[i+1]=='\0')linhas[aux][i+1]='\0';
	}
	aux++;
    } arquivo.close();

  }else cout << "Unable to open file"; 

}

  // move o texto uma linha para cima
  void move_cima(void){
  if(lin1>0){
  lin1=lin1-1;
  lin2=lin2-1;
  desenha();
  }
}

  // move o texto uma linha para baixo
  void move_baixo(void){
 if(lin2<nlinhas){
  lin1=lin1+1;
  lin2=lin2+1;
  desenha();
  }
}

// move o texto uma coluna para direita
  void move_direita(void){
  if(col2<MAX){
  col1=col1+1;
  col2=col2+1;
  desenha();
  }
}

// move o texto uma coluna para esquerda
  void move_esquerda(void){
 if(col1>0){
  col1=col1-1;
  col2=col2-1;
  desenha();
  }
}
};




void termios_off(void)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t); 
  t.c_lflag &= ~ICANON; 
  t.c_lflag &= ~ECHO; 
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void termios_on(void)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= ICANON;
  t.c_lflag |= ECHO; 
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
#endif // __linux__

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#endif // WINDOWS

int main(void)
{
  char c;
  Editor ed;
  ed.inicia("entrada.txt");
  ed.le_arquivo("entrada.txt");
  ed.desenha();

#if defined(__linux__) || defined(__APPLE__)
  termios_off();
#endif
  while(true){
#if defined(__linux__) || defined(__APPLE__)
    c = std::getchar();
#elif defined(_WIN32) || defined(_WIN64)
    c = _getch();
#endif
    if(c == 'q' || c == 'Q')
      break;
    // adicione outros comandos abaixo
    if(c == 'w')
      ed.move_cima();
    if(c == 's')
      ed.move_baixo();
if(c == 'a')
      ed.move_esquerda();
if(c == 'd')
      ed.move_direita();
  }
#if defined(__linux__) || defined(__APPLE__)
  termios_on();
#endif
  ed.destroi();
  return 0;
}


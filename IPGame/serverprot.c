#include <stdlib.h>
#include "server.h"
#include <stdio.h>
#include <string.h>
#define jogadores 2
void checavalidespos(unsigned short posx,unsigned short posy,unsigned short passo,unsigned short* or);
int main() {
  int  pos[2];
  pos[0]=8;
  pos[1]=17;
  serverInit(2);
  char tecla;
  int old;
  char dif;
  puts("o servidor esta funcionando!!");
  while (1) 
  {
    int id = acceptConnection();
    if (id != NO_CONNECTION) {
      puts("hey, alguem se conectou\n");
    }
    struct msg_ret_t msgjog = recvMsg(pos);
    if (msgjog.status == MESSAGE_OK) {
      broadcast(pos,2*sizeof(int));
      fprintf(stderr,"posicao x:%d posicao y:%d\n",pos[0],pos[1]);      
    } 
  }
}

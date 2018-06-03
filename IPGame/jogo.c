#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
const int LARGURA_TELA = 960;
const int ALTURA_TELA = 703; //ðeclaro o tamanho das telas
const int passo = 5;  //declaro quantos passos ando
ALLEGRO_DISPLAY *janela = NULL; //ponteiro para a janela
ALLEGRO_AUDIO_STREAM *musica = NULL; //ponteiro para a musica
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;  //ponteiro para a fila de eventos
ALLEGRO_BITMAP *imagem = NULL;  //ponteiro para a imagem
ALLEGRO_BITMAP *grupo = NULL;   //lonteiro para o bitmap da imagem do grupo
ALLEGRO_BITMAP *fundo = NULL; //ponteiro do pano de fundo
ALLEGRO_BITMAP *quadrado = NULL;  //quadradinho q eh o jogador
void fadeout(int velocidade);  //função pra dar o fadeout
void fadein(ALLEGRO_BITMAP *imagem, int velocidade);  //função q dao fadein[
void checavalidespos(int posx,int posy,int passo,int* or);
void setAudio(char k[]); //função de audio
bool inicializar();  //função q inicializa 
int main(void){
    bool sair = false; //declaro a variavel sair
    if (!inicializar()){ //chamo inicializar, se der errado paro programa
        return -1;
    }
    int desenha = 1; //inicio desenha como 1
    int posx = 90, dir_x = passo; //seto a posição e a passada do personagem
    int posy = 270, dir_y = passo;
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer()); //começo a musica
    al_set_audio_stream_playing(musica, true); //comeca a musica
    fadein(fundo, 1); //a imagem do pano de fundo entra
    al_rest(3.0); //dica durante 3 segundos
    fadeout(1);//e sai
    grupo = al_load_bitmap("resources/group.jpeg"); //seta a imagem do grupo
    fadein(grupo, 1); //faz a aparecer a imagem do grupo
    al_rest(3.0); //€spera 3 segundos
    fadeout(1); //e sai
    imagem = al_load_bitmap("resources/aa.png");  //faz o download do mapa do jogo
    fadein(imagem, 1); //faz ela aparecer
	al_draw_bitmap(imagem, 0, 0, 0);  //coloca a imagem
	al_draw_bitmap(quadrado, posx, posy, 0); //desenha o quadradinho
	al_flip_display();// bota tudo isso para o jogador
 	setAudio("sounds/soundtest2.ogg");//começa a melhor musica possivel
    while (!sair){//entra no loop do jogo
        while (!al_is_event_queue_empty(fila_eventos)){//se  acontecer algo
            ALLEGRO_EVENT evento;   //declara variavel eveno
            al_wait_for_event(fila_eventos, &evento);//esse evento fica na fila
                 if (evento.keyboard.keycode == ALLEGRO_KEY_W){ //ægora ele checa se tem colisao, para cima
			        posy -= dir_y;
			        checavalidespos(posx,posy,-dir_y,&posy);
			        desenha = 1;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A){//para a esquerda
                    posx -= dir_x;
                    checavalidespos(posx,posy,-dir_x,&posx);
			        desenha=1;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_S){//para baixo
			        posy += dir_y;
			        checavalidespos(posx,posy,dir_y,&posy);
			        desenha=1;
			        
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_D){//para a direita
                    posx += dir_x;
			        checavalidespos(posx,posy,dir_x,&posx);
			        desenha=1;
                }
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){  //se n tiver evento, sai 
                sair = true;
            }
            if(desenha && al_is_event_queue_empty(fila_eventos)) {
            	//fprintf(stderr, "x: %i y: %i\n", posx, posy);
	            al_clear_to_color(al_map_rgb(0,0,0));

	            //desenha o quadrado na tela nas posicoes X e Y
	            al_draw_bitmap(imagem, 0, 0, 0);
	            al_draw_bitmap(quadrado, posx, posy, 0);
	            al_flip_display();

	            //zera flag para nao entrar aqui no proximo loop
	            desenha = 0;
        	}
        }
    }
    al_destroy_audio_stream(musica);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela); //tudo termina
    return 0;
}
 
void fadeout(int velocidade){
    ALLEGRO_BITMAP *buffer = NULL;
    buffer = al_create_bitmap(LARGURA_TELA, ALTURA_TELA);
    al_set_target_bitmap(buffer); 
    al_draw_bitmap(al_get_backbuffer(janela), 0, 0, 0); 
    al_set_target_bitmap(al_get_backbuffer(janela));
    if (velocidade <= 0){
        velocidade = 1;
    }
    else if (velocidade > 15){
        velocidade = 15;
    }
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade)
    {
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_tinted_bitmap(buffer, al_map_rgba(255 - alfa, 255 - alfa, 255 - alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); // Não é necessário caso haja controle de FPS
    }
 
    al_destroy_bitmap(buffer);
}
 
void fadein(ALLEGRO_BITMAP *imagem, int velocidade)
{
    if (velocidade < 0)
    {
        velocidade = 1;
    }
    else if (velocidade > 15)
    {
        velocidade = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); // Não é necessário caso haja controle de FPS
    }
}
bool inicializar(){ //inicializa tudo e checa se tudo deu crto
	al_init_image_addon();
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }
    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }
    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }
    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar a janela.\n");
        return false;
    }
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
    musica = al_load_audio_stream("sounds/soundtest.ogg", 4, 1024);
    if (!musica){
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }
 	al_set_window_title(janela, "IP GAME");
 	quadrado = al_create_bitmap(20, 20);
    if (!quadrado){
        fprintf(stderr, "Falha ao criar bitmap.\n");
        al_destroy_display(janela);
        return 0;
    }
    al_set_target_bitmap(quadrado);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(janela));
    fundo = al_load_bitmap("resources/cin.jpeg");
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    return true;
}

void setAudio(char k[]){ //comeca musiquinha
	al_set_audio_stream_playing(musica, false);
	musica = al_load_audio_stream(k, 4, 1024);
 	al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
 	al_set_audio_stream_playing(musica, true);
}
void checavalidespos(int posx,int posy,int passo,int* or){
    if(posx>=0&&posx<=960&&posy>=0&&posy<=70){
        *or-=passo;
    }
    if(posx>=0&&posx<=63&&posy>=70&&posy<=392){
        *or-=passo;
    }
    if(posx>=63&&posx<=145&&posy>=113&&posy<=233){
	    *or-=passo;
	}
	if(posx>=145&&posx<=287&&posy>=121&&posy<=287){
	    *or-=passo;
	}
	if(posx>=287&&posx<=401&&posy>=113&&posy<=233){
	    *or-=passo;
	}
	if(posx>=401&&posx<=463&&posy>=96&&posy<=135){
	    *or-=passo;
	}
	if(posx>=432&&posx<=546&&posy>=98&&posy<=234){
	    *or-=passo;
	}
	if(posx>=544&&posx<=606&&posy>=102&&posy<=132){
	    *or-=passo;
	}
	if(posx>=644&&posx<=860&&posy>=98&&posy<=130){
	    *or-=passo;
	}
	if(posx>=900&&posx<=960&&posy>=68&&posy<=232){
	    *or-=passo;
	}
	if(posx>=836&&posx<=862&&posy>=132&&posy<=232){
		*or-=passo;
	}
	if(posx>=866&&posx<=898&&posy>=194&&posy<=232){
		*or-=passo;
    }
	if(posx>=544&&posx<=814&&posy>=162&&posy<=232){
	    *or-=passo;
	}
	if(posx>=198&&posx<=236&&posy>=344&&posy<=386){
	    *or-=passo;
	}
	if(posx>=370&&posx<=492&&posy>=300&&posy<=426){
	    *or-=passo;
	}
	if(posx>=492&&posx<=660&&posy>=316&&posy<=424){
	    *or-=passo;
	}
	if(posx>=704&&posx<=866&&posy>=322&&posy<=388){
	    *or-=passo;
	}
	if(posx>=836&&posx<=864&&posy>=290&&posy<=324){
	    *or-=passo;
	}
	if(posx>=864&&posx<=960&&posy>=290&&posy<=354){
	    *or-=passo;
	}
	if(posx>=704&&posx<=862&&posy>=412&&posy<=422){
	    *or-=passo;
	}
	if(posx>=0&&posx<=960&&posy>=0&&posy<=70){
	    *or-=passo;
	}
	if(posx>=832&&posx<=864&&posy>=420&&posy<=448){
	    *or-=passo;
	}
	if(posx>=514&&posx<=862&&posy>=448&&posy<=520){
	    *or-=passo;
	}
	if(posx>=900&&posx<=960&&posy>=354&&posy<=703){
	    *or-=passo;
	}
	if(posx>=0&&posx<=66&&posy>=448&&posy<=703){
	    *or-=passo;
	}
	if(posx>=64&&posx<=324&&posy>=450&&posy<=528){
	    *or-=passo;
	}
	if(posx>=320&&posx<=386&&posy>=466&&posy<=602){
	    *or-=passo;
	}
	if(posx>=66&&posx<=960&&posy>=646&&posy<=703){
	    *or-=passo;
	}
	if(posx>=66&&posx<=158&&posy>=578&&posy<=584){
	    *or-=passo;
	}
	if(posx>=196&&posx<=324&&posy>=578&&posy<=584){
		*or-=passo;
	}
	if(posx>=388&&posx<=640&&posy>=578&&posy<=612){
		*or-=passo;
	}
	if(posx>=662&&posx<=812&&posy>=578&&posy<=612){
		*or-=passo;
	}
	if(posx<0||posx>=960||posy<0||posy>=703){
		*or-=passo;
	}
    if(posx>=868&&posx<=898&&posy>=606&&posy<=644){
	    *or-=passo;
	}
    if(posx<0||posx>=960||posy<0||posy>=703){
	    *or-=passo;
	}
}

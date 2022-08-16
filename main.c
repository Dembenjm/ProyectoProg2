#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include<allegro.h>
#include<time.h>
#define N 15
#define M 20
#define MAXBATERIAS 5
void mover_personaje(); //funci�n para mover el personaje
void mover_enemigo();
void cargar_bitmaps();
void importar_mapa();
void pintar_fondo();
void generarbaterias();
void agarrar_bateria();
void agarrar_llave();
void dibujar_personaje();
void indicador_bateria();
BITMAP *buffer; //se declara buffer como tipo bitmap, aqu� se almacenar�n las imagenes
BITMAP *fondo; //se declara un bitmap para almacenar el archivo de imagen del personaje
BITMAP *pared;
BITMAP *piso;
BITMAP *bateria;
BITMAP *bateriabmp;
BITMAP *luz;
BITMAP *luzbmp;
BITMAP *llave;
BITMAP *llavebmp;
BITMAP *puerta;
BITMAP *puertabmp;
struct personaje
{
      int encendida;
      int px; //variable para mover al personaje en eje x
      int py; //variable para mover al personaje en eje y
      int dir;
      int vel_pasos;
      int vida;
      int llave;
      BITMAP *personajebmp;
      BITMAP *personaje;
}player;
struct enemigos
{
      int activado;
      int px;
      int py;
      int dir;
      int vel_pasos;
      BITMAP *enemigobmp;
      BITMAP *enemigo;
}enemigo;
struct indicador
{
      BITMAP *carga;
      BITMAP *cargabmp;
}carga;
int mapa[N][M];
int ventana_w=800;
int ventana_h=600;
int tiempo_luz;
int cont_baterias=0;
int estado_puerta=0;
int detector_linterna=0;
int main()
{
      player.px=ventana_w/2;
      player.py=(ventana_h/2)+100;
      player.vida=1;
      player.llave=0;
      player.encendida=0;
      enemigo.activado=0;
      estado_puerta=0;
      detector_linterna=0;
      allegro_init();//macro que inicializa allegro
      install_keyboard();//instala el controlador de interrupciones del teclado allegro, se debe llamar a esta funci�n antes de usar cualquiera de las rutinas de entrada de teclado
      set_color_depth(32);//establece la profundidad de color que se utilizar� en las llamadas posteriores, profundidades validas: 8, 15 , 16, 24, 32bits
      set_gfx_mode(GFX_AUTODETECT_WINDOWED, ventana_w, ventana_h, 0, 0);//cambia al modo de graficos, se especifica la resolucion de pantalla que se desea y su tama�o minimo
      cargar_bitmaps();
      importar_mapa(mapa);
      generarbaterias(mapa);
      while (!key[KEY_ESC])//mientras la tecla que se presione sea distinta que esc se mantiene dentro del bucle y por lo tanto el juego se sigue ejecutando
      {
            mover_personaje();
            mover_enemigo();
            rest(1);
            pintar_fondo();//imprime matriz con objetos del mapa
            indicador_bateria();
            dibujar_personaje();
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //el buffer es dibujado en la pantalla
            clear_bitmap(buffer);
            if(mapa[(player.py+40)/40][player.px/40]==6)
            {
                  exit(-1);
            }
      }
      destroy_bitmap(player.personaje);
      destroy_bitmap(player.personajebmp);
      destroy_bitmap(enemigo.enemigo);
      destroy_bitmap(enemigo.enemigobmp);
      destroy_bitmap(buffer);
      destroy_bitmap(pared);
      destroy_bitmap(bateria);
      destroy_bitmap(bateriabmp);
      destroy_bitmap(luz);
      destroy_bitmap(luzbmp);
      destroy_bitmap(puerta);
      destroy_bitmap(puertabmp);
      return 0;
}
END_OF_MAIN(); //allegro requiere que se le indique donde termina el main

void cargar_bitmaps()
{
      buffer=create_bitmap(SCREEN_W,SCREEN_H);//el buffer es creado con el ancho y alto de la pantalla
      player.personajebmp=load_bitmap("personaje.bmp",NULL);
      player.personaje=create_bitmap(40,40);
      enemigo.enemigobmp=load_bitmap("enemigo.bmp",NULL);
      enemigo.enemigo=create_bitmap(40,40);
      pared=load_bitmap("pared.bmp",NULL);
      piso=load_bitmap("piso.bmp",NULL);
     // bateria=create_bitmap(40,40);
      bateriabmp=load_bitmap("bateria.bmp",NULL);
      luz=create_bitmap(40,120);
      luzbmp=load_bitmap("luz.bmp",NULL);
      carga.carga=create_bitmap(120,40);
      carga.cargabmp=load_bitmap("cargabmp.bmp",NULL);
      llave=create_bitmap(40,40);
      llavebmp=load_bitmap("llave.bmp",NULL);
      puerta=create_bitmap(40,40);
      puertabmp=load_bitmap("puerta.bmp",NULL);
}

void importar_mapa()
{
      int i,j;
      FILE *fdata; //es un tipo de estructura definida como ARCHIVO. Se considera un tipo de dato opaco ya que su implementaci�n est� oculta. No sabemos qu� constituye el tipo, solo usamos el puntero al tipo y la biblioteca conoce el interior del tipo y puede usar los datos.
      fdata=fopen("mapa.txt","r"); //abre el nombre de archivo al que apunta, por nombre de archivo usando el modo dado "r" Abre un archivo para lectura
      if(fdata==NULL)
      {
            printf("error");
            return;
      }
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  fscanf(fdata,"%d", &mapa[i][j]);
            }
      }
      fclose(fdata);
}

void pintar_fondo()
{
      int i,j;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]==0)//PISO
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]==1)//PARED
                  {
                        draw_sprite(buffer, pared, j*40, i*40);
                  }
                  if(mapa[i][j]==2)//BATERIA
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        draw_sprite(buffer, bateriabmp, j*40, i*40);
                  }
                  else if(mapa[(player.py+44)/40][player.px/40]==2||mapa[player.py/40][player.px/40]==2||mapa[player.py/40][(player.px+40)/40]==2)
                  {
                        agarrar_bateria(mapa);
                  }
                  if(mapa[i][j]==3)//PISO DETECTOR
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]==4)//ENEMIGO
                  {     //ACTIVAR ENEMIGO EN LAS COORDENADAS ACTUALES
                        if(mapa[player.py/40][player.px/40]==3&&enemigo.activado==0)
                        {
                              enemigo.activado=1;
                              enemigo.px=j*40;
                              enemigo.py=i*40;
                        }
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]==5)//LLAVE
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        if(mapa[(player.py+44)/40][player.px/40]==5||mapa[player.py/40][player.px/40]==5||mapa[player.py/40][(player.px+40)/40]==5)
                        {
                              agarrar_llave(mapa);
                        }
                        else
                        {
                              stretch_blit(llavebmp,llave,0,0,300,300,0,0,40,40);
                              draw_sprite(buffer, llave, j*40, i*40);
                        }
                  }
                  if(mapa[i][j]==6)//PUERTA
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        if(mapa[(player.py-4)/40][player.px/40]==6 && player.llave==1)//ABRE PUERTA
                        {
                              printf("estado_puerta=%d",estado_puerta);
                              estado_puerta=1;
                        }
                        if(estado_puerta==0)//CERRADA
                        {
                              stretch_blit(puertabmp,puerta,0,0,40,40,0,0,40,40);
                              draw_sprite(buffer,puerta,j*40,i*40);
                        }
                        if(estado_puerta==1)//ABIERTA
                        {
                              stretch_blit(puertabmp,puerta,40,0,40,40,0,0,40,40);
                              draw_sprite(buffer,puerta,j*40,i*40);
                        }
                  }
            }
      }
}

void dibujar_personaje()
{
      if(player.vida!=0)
      {
            stretch_blit(player.personajebmp, player.personaje, player.dir*300,0,300,300,0,0,40,40);
            draw_sprite(buffer, player.personaje,player.px,player.py);
            if(player.encendida==1)
            {
                  if(tiempo_luz!=0)//LINTERNA ENCENDIDA
                  {
                        tiempo_luz=tiempo_luz-1;
                        //printf("tiempo_luz=%d",tiempo_luz);
                        if(player.dir==0) //ARRIBA
                        {
                              triangle(buffer, player.px, player.py-120, player.px+20, player.py, player.px+40, player.py-120,makeacol(255,255,0,0));
                        }
                        else if(player.dir==1) //IZQUIERDA
                        {
                              triangle(buffer, player.px-120, player.py+40, player.px, player.py+20, player.px-120, player.py,makeacol(255,255,0,0));
                        }
                        else if(player.dir==2)//ABAJO
                        {
                              triangle(buffer, player.px+40, player.py+160, player.px+20, player.py+40, player.px, player.py+160,makeacol(255,255,0,0));
                        }
                        else //DERECHA
                        {
                              triangle(buffer, player.px+160, player.py, player.px+40, player.py+20, player.px+160, player.py+40,makeacol(255,255,0,0));
                        }
                  }
                  else{player.encendida=0;}
            }
      }
      else{}
      if(enemigo.activado==1)
      {
            stretch_blit(enemigo.enemigobmp, enemigo.enemigo, enemigo.dir*300,0,300,300,0,0,40,40);
            draw_sprite(buffer, enemigo.enemigo,enemigo.px,enemigo.py);
      }
}

void mover_personaje()
{     //MOVIMIENTO DEL PERSONAJE
      if(key[KEY_D]) //movimiento hacia la DERECHA cuando se presiona la tecla D
      {
            player.dir=3;
            if(mapa[(player.py+8)/40][(player.px+44)/40] != 1 && mapa[(player.py+32)/40][(player.px+44)/40] != 1) //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.px=player.px+player.vel_pasos;
                  if(key[KEY_LSHIFT])
                  {
                        player.vel_pasos=4;
                  }
                  else
                  {
                        player.vel_pasos=2;
                  }
            }
            else{}
            }

      else if(key[KEY_A]) //movimiento hacia la IZQUIERDA cuando se presiona la tecla A
      {
            player.dir=1;
            if(mapa[(player.py+8)/40][(player.px-4)/40] != 1 && mapa[(player.py+32)/40][(player.px-4)/40] != 1)
            {
                  player.px=player.px-player.vel_pasos;
                  if(key[KEY_LSHIFT])
                  {
                        player.vel_pasos=4;
                  }
                  else
                  {
                        player.vel_pasos=2;
                  }
            }
            else{}
      }
      else if(key[KEY_W]) //movimiento hacia ARRIBA cuando se presiona la tecla W
      {
            player.dir=0;
            if(mapa[(player.py-4)/40][player.px/40] != 1 && mapa[(player.py-4)/40][(player.px+40)/40] != 1)
            {
                  player.py=player.py-player.vel_pasos;
                  if(key[KEY_LSHIFT])
                  {
                        player.vel_pasos=4;
                  }
                  else
                  {
                        player.vel_pasos=2;
                  }
            }
            if(mapa[(player.py-4)/40][(player.px+10)/40]==6 && player.llave==1)
            {
                  player.py=player.py-player.vel_pasos;
                  if(key[KEY_LSHIFT])
                  {
                        player.vel_pasos=4;
                  }
                  else
                  {
                        player.vel_pasos=2;
                  }
            }
            else{}
      }
      else if(key[KEY_S]) //movimiento hacia ABAJO cuando se presiona la tecla S
      {
            player.dir=2;
            if(mapa[(player.py+40)/40][player.px/40] != 1 && mapa[(player.py+40)/40][(player.px+40)/40] != 1)
            {
                  player.py=player.py+player.vel_pasos;
                  if(key[KEY_LSHIFT])
                  {
                        player.vel_pasos=4;
                  }
                  else
                  {
                        player.vel_pasos=2;
                  }
            }
            else{}
      }
      //APUNTAR A DISTINTAS DIRECCIONES
      if(key[KEY_RIGHT])
      {
            player.dir=3;
      }
      if(key[KEY_LEFT])
      {
            player.dir=1;
      }
      if(key[KEY_UP])
      {
            player.dir=0;
      }
      if(key[KEY_DOWN])
      {
            player.dir=2;
      }
      //USAR LINTERNA
      if(key[KEY_SPACE] && cont_baterias>0 && player.encendida==0)
      {
            player.encendida=1;
            tiempo_luz=150;
            set_keyboard_rate(0,0);
            cont_baterias=cont_baterias-1;
            printf("cont_baterias=%d\n",cont_baterias);
            key[KEY_SPACE]=false;
      }

}

void mover_enemigo()
{
      enemigo.vel_pasos=1;
      if(enemigo.activado==1)
      {
            int distancia_y=(enemigo.py+40)/40-(player.py-120)/40;
            int distancia_x=(enemigo.px+40)/40-(player.px-120)/40;
            //printf("distancia_y=%d\n",distancia_y);
            //printf("distancia_x=%d\n",distancia_x);
            if(distancia_y>0&&distancia_y<4&&enemigo.px/40==player.px/40&&player.dir==0&&player.encendida==1) //DETECTAR LINTERNA HACIA ARRIBA
            {
                  printf("linterna en enemigo");
            }
            else if(distancia_y<7&&distancia_y>=4&&enemigo.px/40==player.px/40&&player.dir==2&&player.encendida==1) //DETECTAR LINTERNA HACIA ABAJO
            {
                  printf("linterna en enemigo");
            }
            else if(distancia_x>0&&distancia_x<4&&enemigo.py/40==player.py/40&&player.dir==1&&player.encendida==1) //DETECTAR LINTERNA HACIA LA IZQUIERDA
            {
                  printf("linterna en enemigo");
            }
            else if(distancia_x<7&&distancia_x>=4&&enemigo.py/40==player.py/40&&player.dir==1&&player.encendida==1) //DETECTAR LINTERNA HACIA LA DERECHA
            {
                  printf("linterna en enemigo");
            }
            else
            {
                  if(enemigo.px<player.px)
                  {
                        enemigo.px=enemigo.px+enemigo.vel_pasos;
                  }
                  if(enemigo.px>player.px)
                  {
                        enemigo.px=enemigo.px-enemigo.vel_pasos;
                  }
                  if(enemigo.py<player.py)
                  {
                        enemigo.py=enemigo.py+enemigo.vel_pasos;
                  }
                  if(enemigo.py>player.py)
                  {
                        enemigo.py=enemigo.py-enemigo.vel_pasos;
                  }
            }
      }
//      if((enemigo.py+4)/40==player.py/40 && enemigo.px/40==player.px/40 || (enemigo.px-4)/40==player.px/40 && enemigo.py/40==player.py/40)
//      {
//            player.vida=0;
//      }
}

void generarbaterias()
{
      int i,x,y;
      for(i=0;i<MAXBATERIAS;i++)
      {
            x=rand()%N;
            y=rand()%M;
            while(mapa[x][y]!=0)
            {
                  x=rand()%N;
                  y=rand()%M;
            }
            mapa[x][y]=2;
      }
}

void agarrar_bateria()
{
      mapa[(player.py+44)/40][player.px/40]=0;
      mapa[player.py/40][player.px/40]=0;
      mapa[player.py/40][(player.px+40)/40]=0;
      if(mapa[(player.py+44)/40][player.px/40]==0&&mapa[player.py/40][player.px/40]==0&&mapa[player.py/40][(player.px+40)/40]==0)
      {
            cont_baterias=cont_baterias+1;
      }
      printf("baterias=%d\n",cont_baterias);
}

void agarrar_llave()
{
      mapa[(player.py+44)/40][player.px/40]=0;
      mapa[player.py/40][player.px/40]=0;
      mapa[player.py/40][(player.px+40)/40]=0;
      if(mapa[(player.py+44)/40][player.px/40]==0&&mapa[player.py/40][player.px/40]==0&&mapa[player.py/40][(player.px+40)/40]==0)
      {
            player.llave=1;
      }
      printf("llave=%d\n",player.llave);
}

void indicador_bateria()
{
      stretch_blit(carga.cargabmp,carga.carga,120*cont_baterias,0,720,40,0,0,720,40);
      draw_sprite(buffer,carga.carga,670,10);
}


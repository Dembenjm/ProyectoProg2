//TIPO 1: Se mueve para todas las direcciones hacia el enemigo
//TIPO 2; Se mueve para todas las direcciones (sin colisiones)
//DARLE MAS OBSTACULOS O DINAMISMO (PORTALES)
//CREAR MENU DEL JUEGO

#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include<allegro.h>
#include<time.h>
#define N 15
#define M 21
#define L 11
#define MAXBATERIAS 5
#define MAXENEMIGOS 4

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

struct enemigo
{
      int tipo;
      int activado;
      int px;
      int py;
      int dir;
      int dirx;
      int diry;
      int vel_pasos;
      int colision;
      BITMAP *enemigobmp;
      BITMAP *enemigo;
}enemigos[MAXENEMIGOS]; //tipo de enemigo agregar

struct indicador
{
      BITMAP *carga;
      BITMAP *cargabmp;
}carga;

int inicializar_allegro(int ventana_w, int ventana_h);
int detectar_baterias(int cont_baterias);
int detector_enemigos();
int usar_linterna(int cont_baterias);
int detectar_puerta(int estado_puerta);
int detectar_linterna(int i);
int def_movimiento(int i);
int obtener_dir(int n);
void leer_archivo(char *nombre_archivo);
void importar_nivel(int nvl);
void inicializar_musica(int nvl);
void efectos_sonido(int n_efx);
void agarrar_objeto(int obj);
void mov_enem_horizontal(int i, int prox_pos_enemigox);
void mov_enem_vertical(int i, int prox_pos_enemigoy);
void detect_col_enemigo_player(int i);
int detec_punto_espacio(int planoX1,int planoX2,int planoY1, int planoY2, int puntoX, int puntoY);
void mover_personaje();
void tipos_enemigos();
void mover_enemigo();
void cargar_bitmaps();
void pintar_fondo();
void pintar_objetos(int estado_puerta);
void dibujar_personaje();
void dibujar_enemigo();
void indicador_bateria();
void destruir_bitmaps();
void crear_bitmaps();
void cargar_imagenes();
void cargar_musica();
void pos_inicialplayer();
void contar_llave();
void detectar_llaves();

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
MIDI *musica1;
MIDI *musica2;

//DECLARACI�N DE VARIABLES
char mapa[N][M];
int tiempo_luz;

int main()
{
      //DECLARACION DE VARIABLES LOCALES MAIN
      int cont=0;
      int ventana_w=800;
      int ventana_h=600;
      int cont_baterias=0;
      int estado_puerta=0;
      int mapa_importado=0;
      int nivel=0;

      //INICIALIZACION DE VARIABLES
      player.vida=1;
      player.llave=0;
      player.encendida=0;
      inicializar_allegro(ventana_w, ventana_h);
      crear_bitmaps();
      tipos_enemigos();
      cargar_imagenes();
      cargar_musica();

      //CICLO DEL JUEGO
      while (!key[KEY_ESC])//mientras la tecla que se presione sea distinta que esc se mantiene dentro del bucle y por lo tanto el juego se sigue ejecutando
      {
            if(nivel==0)
            {
                  if(key[KEY_J])
                  {
                        nivel=1;
                  }
            }
            else
            {
                  if(mapa_importado == 0)
                  {
                        // if(nivel==1)
                        // {
                        //       play_midi(musica1,1);
                        // }
                        // else if(nivel==2)
                        // {
                        //       play_midi(musica2,1);
                        // }
                        importar_nivel(nivel);
                        pos_inicialplayer();
                        mapa_importado=1;
                  }
                  else
                  {
                        pintar_fondo();
                        pintar_objetos(estado_puerta);
                        dibujar_personaje();
                        mover_personaje();
                        detectar_llaves();
                        detector_enemigos();
                        estado_puerta = detectar_puerta(estado_puerta);
                        dibujar_enemigo();
                        cont_baterias = detectar_baterias(cont_baterias);

                        if(key[KEY_SPACE] && cont_baterias>0 && player.encendida==0)
                        {
                              cont_baterias = usar_linterna(cont_baterias);
                        }

                        indicador_bateria();

                        if(cont%2==0)
                        {
                              mover_enemigo();
                        }

                        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //el buffer es dibujado en la pantalla
                        clear_bitmap(buffer);
                        rest(5);

                        if(mapa[(player.py+40)/40][player.px/40]=='6' && estado_puerta == 1)
                        {
                              nivel++;
                              mapa_importado=0;
                             // enemigo.activado=0;
                              player.llave=0;
                              estado_puerta=0;
                        }
                  }
            }
            cont++;
      }
      destruir_bitmaps();
      return 0;
}
END_OF_MAIN(); //allegro requiere que se le indique donde termina el main

int inicializar_allegro(int ventana_w, int ventana_h)
{
      allegro_init();//macro que inicializa allegro
      install_keyboard();//instala el controlador de interrupciones del teclado allegro, se debe llamar a esta funci�n antes de usar cualquiera de las rutinas de entrada de teclado
      set_color_depth(32);//establece la profundidad de color que se utilizar� en las llamadas posteriores, profundidades validas: 8, 15 , 16, 24, 32bits
      set_gfx_mode(GFX_AUTODETECT_WINDOWED, ventana_w, ventana_h, 0, 0);//cambia al modo de graficos, se especifica la resolucion de pantalla que se desea y su tama�o minimo
      if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
            allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
            return -1;
      }
      return 0;
}

void cargar_imagenes()
{
      int i;
      pared=load_bitmap("recursos/imagenes/pared.bmp",NULL);
      piso=load_bitmap("recursos/imagenes/piso.bmp",NULL);
      bateriabmp=load_bitmap("recursos/imagenes/bateria.bmp",NULL);
      player.personajebmp=load_bitmap("recursos/imagenes/personaje.bmp",NULL);
      luzbmp=load_bitmap("recursos/imagenes/luz.bmp",NULL);
      carga.cargabmp=load_bitmap("recursos/imagenes/cargabmp.bmp",NULL);
      llavebmp=load_bitmap("recursos/imagenes/llave.bmp",NULL);
      puertabmp=load_bitmap("recursos/imagenes/puerta.bmp",NULL);
      for(i=0;i<MAXENEMIGOS;i++)
      {
            if(enemigos[i].tipo == 0)
            {
                  enemigos[i].enemigobmp=load_bitmap("recursos/imagenes/enemigo0.bmp",NULL); //arreglar
            }

            else
            {
                  enemigos[i].enemigobmp=load_bitmap("recursos/imagenes/enemigo1.bmp",NULL); //arreglar
            }
      }
}

void cargar_musica()
{
      musica1 = load_midi("recursos/musica/musica1.mid");
      musica2 = load_midi("recursos/musica/musica2.mid");
}

void crear_bitmaps()
{
      int i;
      buffer=create_bitmap(SCREEN_W,SCREEN_H);//el buffer es creado con el ancho y alto de la pantalla
      player.personaje=create_bitmap(40,40);
      luz=create_bitmap(40,120);
      carga.carga=create_bitmap(120,40);
      llave=create_bitmap(40,40);
      puerta=create_bitmap(40,40);
      for(i=0;i<MAXENEMIGOS;i++)
      {
            enemigos[i].enemigo=create_bitmap(40,40);
      }
}

void importar_nivel(int nvl)
{
      char *nombre[L];
      if(nvl==1)
      {
         nombre[L]="niveles/nivel1.txt";
      }
      if(nvl==2)
      {
         nombre[L]="niveles/nivel2.txt";
      }
      if(nvl==3)
      {
         nombre[L]="niveles/nivel3.txt";
      }
      if(nvl==4)
      {
         nombre[L]="niveles/nivel4.txt";
      }
      leer_archivo(nombre[L]);
}

void leer_archivo(char *nombre_archivo)
{
      int i,j;
      FILE *fdata; //es un tipo de estructura definida como ARCHIVO. Se considera un tipo de dato opaco ya que su implementaci�n est� oculta. No sabemos qu� constituye el tipo, solo usamos el puntero al tipo y la biblioteca conoce el interior del tipo y puede usar los datos.
      fdata=fopen(nombre_archivo,"rt"); //abre el nombre de archivo al que apunta, por nombre de archivo usando el modo dado "r" Abre un archivo para lectura
      if(fdata==NULL)
      {
            printf("error");
            return;
      }
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  mapa[i][j] = fgetc(fdata);
            }
      }
      fclose(fdata);
}

void pintar_fondo()
{
      int i , j;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='0')//PISO
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]=='1')//PARED
                  {
                        draw_sprite(buffer, pared, j*40, i*40);
                  }
            }
      }
}

void pintar_objetos(int estado_puerta)
{
      int i,j,cont_enemigo;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='2')//BATERIA
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        draw_sprite(buffer, bateriabmp, j*40, i*40);
                  }
                  if(mapa[i][j]=='3')//PISO DETECTOR
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]=='4')//ENEMIGO
                  {
                        if(cont_enemigo<MAXENEMIGOS)
                        {
                              enemigos[cont_enemigo].activado=1;
                              enemigos[cont_enemigo].px=j*40;
                              enemigos[cont_enemigo].py=i*40;
                              mapa[i][j]='0';
                              cont_enemigo++;
                        }
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]=='5')//LLAVE
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        stretch_blit(llavebmp,llave,0,0,300,300,0,0,40,40);
                        draw_sprite(buffer, llave, j*40, i*40);
                  }
                  if(mapa[i][j]=='6')//PUERTA
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        if(i == 0) //ARRIBA
                        {
                              if(estado_puerta==0)//CERRADA
                              {
                                    stretch_blit(puertabmp,puerta,0,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                              else if(estado_puerta==1)//ABIERTA
                              {
                                    stretch_blit(puertabmp,puerta,40,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                        else if(j == 0)
                        {
                              if(estado_puerta==0)//CERRADA
                              {
                                    stretch_blit(puertabmp,puerta,40,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                              else if(estado_puerta==1)//ABIERTA
                              {
                                    stretch_blit(puertabmp,puerta,0,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                        // else if(i==N)
                        // {

                        // }
                        // else if(j==M)
                        // {

                        // }

                  }
                  if(mapa[i][j]=='7')//PUERTA CERRADA CON COLISION
                  {
                        stretch_blit(puertabmp,puerta,160,0,40,40,0,0,40,40);
                        draw_sprite(buffer,puerta,j*40,i*40);
                  }
                  if(mapa[i][j]=='8')//POSICION INICIAL PERSONAJE
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                  }
            }
      }
}

void pos_inicialplayer()
{
      int i, j;
      for(i=0;i<N;i++)
      {     for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='8')
                  {
                        player.py = i*40;
                        player.px = j*40;
                  }
            }
      }
}

int detector_enemigos()
{
      int i, j;
      for(i=0;i<N;i++)
      {     for(j=0;j<M;j++)
            {
                  if(mapa[player.py/40][player.px/40]=='3')
                  {
                        mapa[i][j]=0;
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
}

void mover_personaje(int estado_puerta)
{
      if(key[KEY_LSHIFT])
      {
            player.vel_pasos=4;
      }
      else
      {
            player.vel_pasos=2;
      }
      if(key[KEY_D]) //movimiento hacia la DERECHA cuando se presiona la tecla D
      {
            player.dir=3;
            if(mapa[(player.py+8)/40][(player.px+44)/40] != '1' && mapa[(player.py+32)/40][(player.px+44)/40] != '1') //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.px=player.px+player.vel_pasos;
            }
            else{}
            }

      else if(key[KEY_A]) //movimiento hacia la IZQUIERDA cuando se presiona la tecla A
      {
            player.dir=1;
            if(mapa[(player.py+8)/40][(player.px-4)/40] != '1' && mapa[(player.py+32)/40][(player.px-4)/40] != '1')
            {
                  player.px=player.px-player.vel_pasos;
            }
            else{}
      }
      else if(key[KEY_W]) //movimiento hacia ARRIBA cuando se presiona la tecla W
      {
            player.dir=0;
            if(mapa[(player.py-4)/40][(player.px+10)/40]!='1' && mapa[(player.py-4)/40][(player.px+30)/40]!='1')
            {
                  if(mapa[(player.py-4)/40][player.px/40]=='6' && mapa[(player.py-4)/40][(player.px+30)/40]=='6') //Arreglar
                  {
                        if(estado_puerta==1)
                        {
                              player.py=player.py-player.vel_pasos;
                        }
                  }
                  else
                  {
                        player.py=player.py-player.vel_pasos;
                  }
            }
            else{}
      }
      else if(key[KEY_S]) //movimiento hacia ABAJO cuando se presiona la tecla S
      {
            player.dir=2;
            if(mapa[(player.py+44)/40][(player.px+4)/40]!='1' && mapa[(player.py+44)/40][(player.px+30)/40]!='1')
            {
                  if(mapa[(player.py+44)/40][(player.px+4)/40]=='6' && mapa[(player.py+44)/40][(player.px+30)/40]=='6')
                  {
                        if(estado_puerta==1)
                        {
                              player.py=player.py+player.vel_pasos;
                        }
                  }
                  else if(mapa[(player.py+44)/40][(player.px+4)/40]=='7' && mapa[(player.py+44)/40][(player.px+30)/40]=='7')
                  {
                        if(estado_puerta==1)
                        {
                              player.py=player.py+player.vel_pasos;
                        }
                  }
                  else
                  {
                        player.py=player.py+player.vel_pasos;
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
}

void tipos_enemigos()
{
      int i=0;
      srand(time(NULL));
      for(i=0; i<MAXENEMIGOS; i++)
      {
            enemigos[i].tipo = rand() % 3;
            printf("enemigos[i].tipo = %d\n",enemigos[i].tipo);
      }
}

// void colision_enemigo0()
// {
//       int i;
//       srand(time(NULL));
//       for(i=0; i<MAXENEMIGOS; i++)
//       {
//             if(enemigos[i].tipo==0)
//             {
//                   enemigos[i].dir=rand()%2;
//             }
//       }
// }

void dibujar_enemigo()
{
      int i;
      for(i=0; i<MAXENEMIGOS; i++)
      {
            if(enemigos[i].activado==1)
            {
                  if(enemigos[i].tipo == 0)
                  {
                        stretch_blit(enemigos[i].enemigobmp, enemigos[i].enemigo, enemigos[i].dir*300,0,300,300,0,0,40,40);
                        draw_sprite(buffer, enemigos[i].enemigo,enemigos[i].px,enemigos[i].py);
                  }
                  else
                  {
                        stretch_blit(enemigos[i].enemigobmp, enemigos[i].enemigo, enemigos[i].dir*300,0,300,300,0,0,40,40);
                        draw_sprite(buffer, enemigos[i].enemigo,enemigos[i].px,enemigos[i].py);
                  }

            }
      }
}

int usar_linterna(int cont_baterias)
{
      player.encendida=1;
      tiempo_luz=150;
      set_keyboard_rate(0,0);
      cont_baterias=cont_baterias-1;
      printf("cont_baterias=%d\n",cont_baterias);
      key[KEY_SPACE]=false;
      return cont_baterias;
}

void mover_enemigo()
{
      int i, detectar_luz, distancia_x, distancia_y, prox_pos_enemigox, prox_pos_enemigoy, diferencia=4;;
      for(i=0; i < MAXENEMIGOS; i++)
      {
            if(enemigos[i].activado==1)
            {
                  detectar_luz = detectar_linterna(i);
                  if(player.encendida==1 && detectar_luz==1)
                  {
                        if(detectar_luz == 0)
                        {
                              enemigos[i].colision=1;
                        }
                  }
                  else
                  {
                        switch (enemigos[i].tipo)
                        {
                        case 0:
                              enemigos[i].vel_pasos=3;
                              if(enemigos[i].dirx==0)
                              {
                                    if(mapa[(enemigos[i].py)/40][(enemigos[i].px+44)/40] != '1') //Colisión Derecha
                                    {
                                          enemigos[i].px=enemigos[i].px+enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                           enemigos[i].dirx=1;
                                    }
                              }
                              else
                              {
                                    if(mapa[enemigos[i].py/40][(enemigos[i].px-4)/40] != '1') //Colisión Izquierda
                                    {
                                          enemigos[i].px=enemigos[i].px-enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                          enemigos[i].dirx=0;
                                    }
                              }
                              break;
                        case 1:
                              enemigos[i].vel_pasos=3;
                              if(enemigos[i].diry==0)
                              {
                                    if(mapa[(enemigos[i].py-4)/40][enemigos[i].px/40] != '1') //Colisión Arriba
                                    {
                                          enemigos[i].py=enemigos[i].py-enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                          enemigos[i].diry=1;
                                    }
                              }
                              else
                              {
                                    if(mapa[(enemigos[i].py+40)/40][enemigos[i].px/40] != '1') //Colisión Abajo
                                    {
                                          enemigos[i].py=enemigos[i].py+enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                          enemigos[i].diry=0;
                                    }
                              }
                              break;
                        case 2:
                              enemigos[i].vel_pasos=1;
                              distancia_x = enemigos[i].px - player.px;
                              distancia_y = enemigos[i].py - player.py;
                              enemigos[i].dirx = obtener_dir(distancia_x);
                              enemigos[i].diry = obtener_dir(distancia_y);
                              prox_pos_enemigox = enemigos[i].px - (enemigos[i].vel_pasos * enemigos[i].dirx);
                              prox_pos_enemigoy = enemigos[i].py - (enemigos[i].vel_pasos * enemigos[i].diry);
                              mov_enem_horizontal(i, prox_pos_enemigox);
                              mov_enem_vertical(i, prox_pos_enemigoy);
                              break;
                        case 3:
                              enemigos[i].vel_pasos=1;
                              distancia_x = enemigos[i].px - player.px;
                              distancia_y = enemigos[i].py - player.py;
                              enemigos[i].px -= enemigos[i].vel_pasos * obtener_dir(distancia_x);
                              enemigos[i].py -= enemigos[i].vel_pasos * obtener_dir(distancia_y);
                              break;
                        }
                  }
            }
            detect_col_enemigo_player(i);
      }
}

void detect_col_enemigo_player(int i)
{
      int enemigox1,enemigox2,enemigoy1,enemigoy2,diferenciaX,diferenciaY;
      diferenciaX = 10;
      diferenciaY = 10;
      enemigox1 = enemigos[i].px-diferenciaX;
      enemigox2 = enemigos[i].px+40-diferenciaX;
      enemigoy1 = enemigos[i].py+diferenciaY;
      enemigoy2 = enemigos[i].py+40+diferenciaY;
      if(detec_punto_espacio(enemigox1,enemigox2,enemigoy1,enemigoy2,player.px,player.py) || detec_punto_espacio(enemigox1,enemigox2,enemigoy1,enemigoy2,player.px+40,player.py) || detec_punto_espacio(enemigox1,enemigox2,enemigoy1,enemigoy2,player.px,player.py+40) || detec_punto_espacio(enemigox1,enemigox2,enemigoy1,enemigoy2,player.px+40,player.py+40))
      {
            player.vida=0;
            printf("MUERTO");
      }
}

int detec_punto_espacio(int planoX1,int planoX2,int planoY1, int planoY2, int puntoX, int puntoY)
{
      if(puntoX >= planoX1 && puntoX <= planoX2 && puntoY >= planoY1 && puntoY <= planoY2)
      {
            return 1;
      }
      return 0;
}

void mov_enem_horizontal(int i, int prox_pos_enemigox)
{
      int diferencia=4;
      if(mapa[(enemigos[i].py+diferencia)/40][(prox_pos_enemigox)/40] != '1' && mapa[(enemigos[i].py+diferencia)/40][(prox_pos_enemigox+40)/40] != '1' && mapa[(enemigos[i].py+40-diferencia)/40][(prox_pos_enemigox)/40] != '1' && mapa[(enemigos[i].py+40-diferencia)/40][(prox_pos_enemigox+40)/40] != '1')
      {
            enemigos[i].px -= enemigos[i].vel_pasos * enemigos[i].dirx;
      }
}

void mov_enem_vertical(int i, int prox_pos_enemigoy)
{
      int diferencia=4;
      if(mapa[(prox_pos_enemigoy)/40][(enemigos[i].px + diferencia)/40] != '1' && mapa[(prox_pos_enemigoy+40)/40][(enemigos[i].px + diferencia)/40] != '1' && mapa[(prox_pos_enemigoy)/40][(enemigos[i].px+40 - diferencia)/40] != '1' && mapa[(prox_pos_enemigoy+40)/40][(enemigos[i].px+40 - diferencia)/40] != '1')
      {
            enemigos[i].py -= enemigos[i].vel_pasos * enemigos[i].diry;
      }
}

int obtener_dir(int n)
{
      if(n < 0)
            return -1;
      if(n > 0)
            return 1;
      return 0;
}
void agarrar_objeto(int obj) //elemento de la matriz se transforma a 0
{
      if(mapa[(player.py+40)/40][player.px/40]==obj)
      {
            mapa[(player.py+40)/40][player.px/40]='0';
      }
      else if(mapa[player.py/40][(player.px+40)/40]==obj)
      {
            mapa[player.py/40][(player.px+40)/40]='0';
      }
      else if(mapa[player.py/40][(player.px-4)/40]==obj)
      {
            mapa[player.py/40][(player.px-4)/40]='0';
      }
      else if(mapa[(player.py-4)/40][player.px/40]==obj)
      {
            mapa[(player.py-4)/40][player.px/40]='0';
      }
}

int detectar_baterias(int cont_baterias)
{
      int i , j;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='2')
                  {
                        if(mapa[(player.py+40)/40][player.px/40]=='2'||mapa[player.py/40][(player.px+40)/40]=='2'||mapa[player.py/40][(player.px-4)/40]=='2'||mapa[(player.py-4)/40][player.px/40]=='2')
                        {
                              if(cont_baterias < MAXBATERIAS)
                              {
                                    agarrar_objeto('2');
                                    cont_baterias++;
                                    printf("cont_baterias=%d",cont_baterias);
                              }
                        }
                  }
            }
      }
      return cont_baterias;
}

int detectar_puerta(int estado_puerta)
{
      int i , j;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='6') //PUERTA
                  {
                        if(mapa[(player.py-4)/40][player.px/40]=='6' && player.llave==1)//ABRE PUERTA
                        {
                              printf("estado_puerta=%d",estado_puerta);
                              estado_puerta=1;
                              player.llave=0;
                        }
                  }
            }
      }
      return estado_puerta;
}

int detectar_linterna(int i)
{
      int distancia_x, distancia_y;
      distancia_x=(enemigos[i].px+40)/40-(player.px-120)/40;
      distancia_y=(enemigos[i].py+40)/40-(player.py-120)/40;
      if(enemigos[i].py/40==player.py/40)
      {
            if(distancia_x>0 && distancia_x<4 && player.dir==1)
            {
                  return 1;
            } //DETECTAR LINTERNA HACIA LA IZQUIERDA
            else if(distancia_x<=7 && distancia_x>=4 && player.dir==3)
            {
                  return 1;
            } //DETECTAR LINTERNA HACIA LA DERECHA
            else
            {
                  return 0;
            }
      }
      else if (enemigos[i].px/40==player.px/40)
      {
            if(distancia_y>0 && distancia_y<4 && player.dir==0)
            {
                  return 1;
            }//DETECTAR LINTERNA HACIA ARRIBA
            else if(distancia_y<=7 && distancia_y>=4 && player.dir==2)
            {
                  return 1;
            }
            else
            {
                  return 0;
            }
      }

}

void detectar_llaves()
{
      int i, j;
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='5')
                  {
                        if(mapa[(player.py+40)/40][player.px/40]=='5'||mapa[player.py/40][(player.px+40)/40]=='5'||mapa[player.py/40][(player.px-4)/40]=='5'||mapa[(player.py-4)/40][player.px/40]=='5')
                        {
                              agarrar_objeto('5');
                              player.llave=1;
                              printf("player.llave=%d",player.llave);
                        }
                  }
            }
      }
}

void indicador_bateria(int cont_baterias)
{
      if(cont_baterias < MAXBATERIAS)
      {
            stretch_blit(carga.cargabmp,carga.carga,120*cont_baterias,0,720,40,0,0,720,40);
            draw_sprite(buffer,carga.carga,670,10);
      }
      else
      {
            stretch_blit(carga.cargabmp,carga.carga,120*MAXBATERIAS,0,720,40,0,0,720,40);
            draw_sprite(buffer,carga.carga,670,10);
      }
}

void destruir_bitmaps()
{
      destroy_bitmap(player.personaje);
      destroy_bitmap(player.personajebmp);
      destroy_bitmap(buffer);
      destroy_bitmap(pared);
      destroy_bitmap(bateria);
      destroy_bitmap(bateriabmp);
      destroy_bitmap(luz);
      destroy_bitmap(luzbmp);
      destroy_bitmap(puerta);
      destroy_bitmap(puertabmp);
     // destroy_bitmap(enemigo.enemigo);
   //   destroy_bitmap(enemigo.enemigobmp);
}


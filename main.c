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
#define MAXVIDAS 3

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
      int dir_x;
      int dir_y;
      int vel_pasos;
      int colision;
      BITMAP *enemigobmp;
      BITMAP *enemigo;
}enemigos[MAXENEMIGOS]; //tipo de enemigo agregar

int inicializar_allegro(int ventana_w, int ventana_h);
void elegir_fondo(int nivel);
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
void agarrar_objeto(char obj);
void mov_enem_horizontal(int i, int prox_pos_enemigox);
void mov_enem_vertical(int i, int prox_pos_enemigoy);
int detect_col_enemigo_player(int i);
int detec_punto_espacio(int planoX1,int planoX2,int planoY1, int planoY2, int puntoX, int puntoY);
void mover_personaje();
void generar_tipos_enemigos();
void mover_enemigo();
void cargar_bitmaps();
void pintar_fondo(int ventana_w, int ventana_h);
void pintar_objetos(int estado_puerta);
void dibujar_personaje();
void dibujar_enemigo();
void indicador_bateria(int cont_baterias);
void indicador_vida();
void destruir_bitmaps();
void crear_bitmaps(int ventana_w, int ventana_h);
void cargar_imagenes();
void cargar_musica();
void pos_inicial_player();
void contar_llave();
void detectar_llaves();

BITMAP *buffer; //se declara buffer como tipo bitmap, aqu� se almacenar�n las imagenes
BITMAP *fondo; //se declara un bitmap para almacenar el archivo de imagen del personaje
BITMAP *pared;
BITMAP *piso;
BITMAP *bateria;
BITMAP *bateria_bmp;
BITMAP *luz;
BITMAP *luz_bmp;
BITMAP *llave;
BITMAP *llave_bmp;
BITMAP *puerta;
BITMAP *puerta_bmp;
BITMAP *vida;
BITMAP *vida_bmp;
BITMAP *indicador_bateria_bmp;
BITMAP *indicador_bateria_img;
BITMAP *pared_izq;
BITMAP *pared_der;
BITMAP *pared_arr;
BITMAP *pared_abj;
BITMAP *pared_hor;
BITMAP *pared_ver_der;
BITMAP *pared_ver_izq;
BITMAP *pared_sup_izq;
BITMAP *pared_sup_der;
BITMAP *pared_inf_izq;
BITMAP *pared_inf_der;
BITMAP *pared_esquinas;
BITMAP *pared_esquinas_bmp;
BITMAP *menu;
MIDI *musica_1;
MIDI *musica_2;

//DECLARACI�N DE VARIABLES
char mapa[N][M];
int tiempo_luz;
int tiempo_dano;
int tiempo_juego;

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
      player.vida=3;
      player.llave=0;
      player.encendida=0;
      inicializar_allegro(ventana_w, ventana_h);
      crear_bitmaps(ventana_w, ventana_h);
      generar_tipos_enemigos();
      cargar_imagenes();
      cargar_musica();
      //CICLO DEL JUEGO
      while (!key[KEY_ESC])//mientras la tecla que se presione sea distinta que esc se mantiene dentro del bucle y por lo tanto el juego se sigue ejecutando
      {
            if(nivel==0)
            {
                  blit(menu,screen,0,0,0,0,ventana_w,ventana_h);
                  if(key[KEY_ENTER])
                  {
                        nivel=1;
                        tiempo_juego=0;
                  }
            }
            else
            {
                  if(mapa_importado == 0)
                  {
                        // if(nivel==1)
                        // {
                        //       play_midi(musica_1,1);
                        // }
                        // else if(nivel==2)
                        // {
                        //       play_midi(musica_2,1);
                        // }
                        importar_nivel(nivel);
                        elegir_fondo(nivel);
                        pos_inicial_player();
                        mapa_importado=1;
                  }
                  else
                  {
                        pintar_fondo(ventana_w,ventana_h);
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

                        indicador_bateria(cont_baterias);
                        indicador_vida();

                        if(cont%2==0)
                        {
                              mover_enemigo();
                        }
                        if(player.vida==0)
                        {
                              printf("MUERTO\n");
                        }
                        textprintf(buffer, font,320,15,255,"%d",tiempo_juego);
                        tiempo_juego++;
                        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //el buffer es dibujado en la pantalla
                        clear_bitmap(buffer);
                        rest(10);
                        if(estado_puerta == 1)
                        {
                              if((mapa[(player.py+40)/40][player.px/40]=='2' && player.dir==0) || (mapa[player.py/40][(player.px+40)/40]=='2' && player.dir==1) || (mapa[player.py/40][player.px/40]=='2' && player.dir==3) || (mapa[player.py/40][player.px/40]=='2'&& player.dir==2))
                              {
                                    nivel++;
                                    mapa_importado=0;
                              // enemigo.activado=0;
                                    player.llave=0;
                                    estado_puerta=0;
                              }
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

void crear_bitmaps(int ventana_w, int ventana_h)
{
      int i;
      buffer=create_bitmap(SCREEN_W,SCREEN_H);//el buffer es creado con el ancho y alto de la pantalla
      fondo=create_bitmap(ventana_w, ventana_h);
      player.personaje=create_bitmap(40,40);
      luz=create_bitmap(40,120);
      bateria=create_bitmap(120,40);
      vida=create_bitmap(120,40);
      llave=create_bitmap(40,40);
      puerta=create_bitmap(40,40);
      indicador_bateria_bmp=create_bitmap(120,40);
      pared_esquinas_bmp = create_bitmap(40,40);
      for(i=0;i<MAXENEMIGOS;i++)
      {
            enemigos[i].enemigo=create_bitmap(40,40);
      }
}

void cargar_imagenes()
{
      int i;
      pared_der=load_bitmap("recursos/imagenes/pared_der.bmp",NULL);
      pared_izq=load_bitmap("recursos/imagenes/pared_izq.bmp",NULL);
      pared_arr=load_bitmap("recursos/imagenes/pared_arr.bmp",NULL);
      pared_abj=load_bitmap("recursos/imagenes/pared_abj.bmp",NULL);
      pared_sup_izq=load_bitmap("recursos/imagenes/pared_sup_izq.bmp",NULL);
      pared_sup_der=load_bitmap("recursos/imagenes/pared_sup_der.bmp",NULL);
      pared_inf_izq=load_bitmap("recursos/imagenes/pared_inf_izq.bmp",NULL);
      pared_inf_der=load_bitmap("recursos/imagenes/pared_inf_der.bmp",NULL);
      pared_hor=load_bitmap("recursos/imagenes/pared_hor.bmp",NULL);
      pared_ver_izq=load_bitmap("recursos/imagenes/pared_ver_izq.bmp",NULL);
      pared_ver_der=load_bitmap("recursos/imagenes/pared_ver_der.bmp",NULL);
      pared = load_bitmap("recursos/imagenes/pared.bmp",NULL);
      piso=load_bitmap("recursos/imagenes/piso.bmp",NULL);
      bateria_bmp=load_bitmap("recursos/imagenes/bateria.bmp",NULL);
      player.personajebmp=load_bitmap("recursos/imagenes/personaje.bmp",NULL);
      luz_bmp=load_bitmap("recursos/imagenes/luz.bmp",NULL);
      llave_bmp=load_bitmap("recursos/imagenes/llave.bmp",NULL);
      puerta_bmp=load_bitmap("recursos/imagenes/puerta.bmp",NULL);
      vida_bmp=load_bitmap("recursos/imagenes/indicador_vida.bmp",NULL);
      indicador_bateria_img=load_bitmap("recursos/imagenes/indicador_bateria.bmp",NULL);
      pared_esquinas = load_bitmap("recursos/imagenes/pared_esquinas.bmp",NULL);
      menu= load_bitmap("recursos/imagenes/menu.bmp",NULL);
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
      musica_1 = load_midi("recursos/musica/musica_1.mid");
      musica_2 = load_midi("recursos/musica/musica_2.mid");
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

void elegir_fondo(int nivel)
{
      switch (nivel)
      {
      case 1:
            fondo = load_bitmap("recursos/imagenes/fondo1.bmp",NULL);
            break;
      case 2:
            fondo = load_bitmap("recursos/imagenes/fondo2.bmp",NULL);
            break;
      }

}

void pintar_fondo(int ventana_w, int ventana_h)
{
      int i , j;
      blit(fondo, buffer, 0, 0, 0, 0, ventana_w, ventana_h);
      for(i=0;i<N;i++)
      {
            for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='7') //PARED HORIZONTAL TRANSPARENCIA
                  {
                        draw_sprite(buffer,pared_hor,j*40,i*40);
                  }
                  if(mapa[i][j]=='1')//PARED ABAJO,ARRIBA,DERECHA,IZQUIERDA
                  {
                        if(i==0)
                        {
                              draw_sprite(buffer,pared_abj,j*40,i*40);
                        }
                        else if(i==N-1)
                        {
                              draw_sprite(buffer,pared_arr,j*40,i*40);
                        }
                        else if(j==0)
                        {
                              draw_sprite(buffer,pared_der,j*40,i*40);
                        }
                        else if(j==M-2)
                        {
                              draw_sprite(buffer,pared_izq,j*40,i*40);
                        }
                  }
                  if(mapa[i][j]=='4') //PARED ESQUINAS EXTREMOS
                  {
                        if(i==0 && j==0)
                        {
                              draw_sprite(buffer,pared_sup_izq,j*40,i*40);
                        }
                        else if(i==0 && j==M-2)
                        {
                              draw_sprite(buffer,pared_sup_der,j*40,i*40);
                        }
                        else if(i==N-1 && j==0)
                        {
                              draw_sprite(buffer,pared_inf_izq,j*40,i*40);
                        }
                        else if(i==N-1 && j==M-2)
                        {
                              draw_sprite(buffer,pared_inf_der,j*40,i*40);
                        }
                  }
                  if(mapa[i][j]=='6') //PARED VERTICAL IZQ
                  {
                        draw_sprite(buffer,pared_ver_izq,j*40,i*40);
                  }
                  if(mapa[i][j]=='5') //PARED VERTICAL DER
                  {
                        draw_sprite(buffer,pared_ver_der,j*40,i*40);
                  }
                  if(mapa[i][j]=='8') //ESQUINA UNION SUPERIOR DER
                  {
                        stretch_blit(pared_esquinas,pared_esquinas_bmp,0,0,40,40,0,0,40,40);
                        draw_sprite(buffer,pared_esquinas_bmp,j*40,i*40);
                  }
                  if(mapa[i][j]=='9') //ESQUINA UNION SUPERIOR IZQ
                  {
                        stretch_blit(pared_esquinas,pared_esquinas_bmp,40,0,40,40,0,0,40,40);
                        draw_sprite(buffer,pared_esquinas_bmp,j*40,i*40);
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
                  if(mapa[i][j]=='B')//BATERIA
                  {
                        //draw_sprite(buffer, piso, j*40, i*40);
                        draw_sprite(buffer, bateria_bmp, j*40, i*40);
                  }
                  if(mapa[i][j]=='D')//PISO DETECTOR
                  {
                        //draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]=='E')//ENEMIGO
                  {
                        if(cont_enemigo<MAXENEMIGOS)
                        {
                              enemigos[cont_enemigo].activado=1;
                              enemigos[cont_enemigo].px=j*40;
                              enemigos[cont_enemigo].py=i*40;
                              mapa[i][j]='0';
                              cont_enemigo++;
                        }
                        //draw_sprite(buffer, piso, j*40, i*40);
                  }
                  if(mapa[i][j]=='L')//LLAVE
                  {
                        //draw_sprite(buffer, piso, j*40, i*40);
                        stretch_blit(llave_bmp,llave,0,0,300,300,0,0,40,40);
                        draw_sprite(buffer, llave, j*40, i*40);
                  }
                  if(mapa[i][j]=='2')//PUERTA
                  {
                       // draw_sprite(buffer, piso, j*40, i*40);
                        if(i==0)
                        {
                              if(estado_puerta==0) /*puerta cerrada*/
                              {
                                    stretch_blit(puerta_bmp,puerta,40,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde abajo hacia arriba*/
                              }
                              else /*puerta abierta*/
                              {
                                    stretch_blit(puerta_bmp,puerta,120,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                        else if(i==N-1)
                        {
                              if(estado_puerta==0)
                              {
                                    stretch_blit(puerta_bmp,puerta,0,0,40,40,0,0,40,40); /*pinta puerta con dibujo desde arriba hacia abajo*/
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                              else
                              {
                                    stretch_blit(puerta_bmp,puerta,120,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                        else if(j==0)
                        {
                              if(estado_puerta==0) /*puerta cerrada*/
                              {
                                    stretch_blit(puerta_bmp,puerta,120,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde derecha hacia izquierda*/
                              }
                              else /*puerta abierta*/
                              {
                                    stretch_blit(puerta_bmp,puerta,0,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                        else if(j==M-2)
                        {
                              if(estado_puerta==0) /*puerta cerrada*/
                              {
                                    stretch_blit(puerta_bmp,puerta,80,0,40,40,0,0,40,40);
                                    draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde derecha hacia izquierda*/
                              }
                              else /*puerta abierta*/
                              {
                                    stretch_blit(puerta_bmp,puerta,40,0,40,40,0,0,40,40);
                                     draw_sprite(buffer,puerta,j*40,i*40);
                              }
                        }
                  }
                  if(mapa[i][j]=='3')//PUERTA CERRADA CON COLISION
                  {
                        draw_sprite(buffer, piso, j*40, i*40);
                        if(i==0)
                        {
                              stretch_blit(puerta_bmp,puerta,40,0,40,40,0,0,40,40);
                              draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde abajo hacia arriba*/
                        }
                        else if(i==N-1)
                        {
                              stretch_blit(puerta_bmp,puerta,0,0,40,40,0,0,40,40); /*pinta puerta con dibujo desde arriba hacia abajo*/
                              draw_sprite(buffer,puerta,j*40,i*40);
                        }
                        else if(j==0)
                        {
                              stretch_blit(puerta_bmp,puerta,120,0,40,40,0,0,40,40);
                              draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde derecha hacia izquierda*/

                        }
                        else if(j==M-2)
                        {
                              stretch_blit(puerta_bmp,puerta,80,0,40,40,0,0,40,40);
                              draw_sprite(buffer,puerta,j*40,i*40); /*pinta puerta con dibujo desde derecha hacia izquierda*/
                        }
                  }
            }
      }
}

void pos_inicial_player()
{
      int i, j;
      for(i=0;i<N;i++)
      {     for(j=0;j<M;j++)
            {
                  if(mapa[i][j]=='P')
                  {
                        player.py = i*40;
                        player.px = j*40;
                        mapa[i][j]='0';
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
            if(mapa[(player.py+8)/40][(player.px+44)/40] != '1' && mapa[(player.py+32)/40][(player.px+44)/40] != '1' && mapa[(player.py+4)/40][(player.px+44)/40]!='2' && mapa[(player.py+36)/40][(player.px+44)/40]!='2'&& mapa[(player.py+8)/40][(player.px+6)/40] != '5' && mapa[(player.py+32)/40][(player.px+6)/40] != '5'&& mapa[(player.py+8)/40][(player.px+44)/40] != '6' && mapa[(player.py+32)/40][(player.px+44)/40] != '6') //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.px=player.px+player.vel_pasos;
            }
            else if(mapa[(player.py+4)/40][(player.px+44)/40]=='2' && mapa[(player.py+36)/40][(player.px+44)/40]=='2' && estado_puerta==1)
            {
                  player.px=player.px+player.vel_pasos;
            }
      }
      else if(key[KEY_A]) //movimiento hacia la IZQUIERDA cuando se presiona la tecla A
      {
            player.dir=1;
            if(mapa[(player.py+8)/40][(player.px-4)/40] != '1' && mapa[(player.py+32)/40][(player.px-4)/40] != '1' && mapa[(player.py+4)/40][(player.px-4)/40]!='2' && mapa[(player.py+36)/40][(player.px-4)/40]!='2'&& mapa[(player.py+8)/40][(player.px-4)/40] != '5' && mapa[(player.py+32)/40][(player.px+6)/40] != '5'&& mapa[(player.py+8)/40][(player.px+20)/40] != '6' && mapa[(player.py+32)/40][(player.px+20)/40] != '6') //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.px=player.px-player.vel_pasos;
            }
            else if(mapa[(player.py+4)/40][(player.px-4)/40]=='2' && mapa[(player.py+36)/40][(player.px-4)/40]=='2' && estado_puerta==1)
            {
                  player.px=player.px-player.vel_pasos;
            }
      }
      else if(key[KEY_W]) //movimiento hacia ARRIBA cuando se presiona la tecla W
      {
            player.dir=0;
            if(mapa[(player.py-4)/40][(player.px+10)/40]!='1' && mapa[(player.py-4)/40][(player.px+30)/40]!='1' && mapa[(player.py-4)/40][(player.px+10)/40]!='2' && mapa[(player.py-4)/40][(player.px+30)/40]!='2'&& mapa[(player.py-4)/40][(player.px+10)/40]!='6' && mapa[(player.py+40)/40][(player.px+10)/40]!='6' && mapa[(player.py-4)/40][(player.px+10)/40]!='7' && mapa[(player.py-4)/40][(player.px+30)/40]!='7') //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.py=player.py-player.vel_pasos;
            }
            else if(mapa[(player.py-4)/40][player.px/40]=='2' && mapa[(player.py-4)/40][(player.px+30)/40]=='2' && estado_puerta==1)
            {
                  player.py=player.py-player.vel_pasos;
            }
      }
      else if(key[KEY_S]) //movimiento hacia ABAJO cuando se presiona la tecla S
      {
            player.dir=2;
            if(mapa[(player.py+44)/40][(player.px+4)/40]!='1' && mapa[(player.py+44)/40][(player.px+30)/40]!='1' && mapa[(player.py+44)/40][player.px/40]!='2' && mapa[(player.py+44)/40][(player.px+30)/40]!='2'&& mapa[(player.py+44)/40][player.px/40]!='3' && mapa[(player.py+44)/40][(player.px+30)/40]!='3') //si la posici�n del mapa con posicion en las coordenadas del jugador sean distintas de X (pared)
            {
                  player.py=player.py+player.vel_pasos;
            }
            else if(mapa[(player.py+44)/40][player.px/40]=='2' && mapa[(player.py+44)/40][(player.px+30)/40]=='2' && estado_puerta==1)
            {
                  player.py=player.py+player.vel_pasos;
            }
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

void generar_tipos_enemigos()
{
      int i=0;
      srand(time(NULL));
      for(i=0; i<MAXENEMIGOS; i++)
      {
            enemigos[i].tipo = rand() % 3;
            printf("enemigos[i].tipo = %d\n",enemigos[i].tipo);
      }
}

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
      int i, detectar_luz, distancia_x, distancia_y, prox_pos_enemigox, prox_pos_enemigoy;
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
                              if(enemigos[i].dir_x==0)
                              {
                                    if(mapa[(enemigos[i].py)/40][(enemigos[i].px+44)/40] != '1') //Colisión Derecha
                                    {
                                          enemigos[i].px=enemigos[i].px+enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                           enemigos[i].dir_x=1;
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
                                          enemigos[i].dir_x=0;
                                    }
                              }
                              break;
                        case 1:
                              enemigos[i].vel_pasos=3;
                              if(enemigos[i].dir_y==0)
                              {
                                    if(mapa[(enemigos[i].py-4)/40][enemigos[i].px/40] != '1') //Colisión Arriba
                                    {
                                          enemigos[i].py=enemigos[i].py-enemigos[i].vel_pasos;
                                    }
                                    else
                                    {
                                          enemigos[i].dir_y=1;
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
                                          enemigos[i].dir_y=0;
                                    }
                              }
                              break;
                        case 2:
                              enemigos[i].vel_pasos=1;
                              distancia_x = enemigos[i].px - player.px;
                              distancia_y = enemigos[i].py - player.py;
                              enemigos[i].dir_x = obtener_dir(distancia_x);
                              enemigos[i].dir_y = obtener_dir(distancia_y);
                              prox_pos_enemigox = enemigos[i].px - (enemigos[i].vel_pasos * enemigos[i].dir_x);
                              prox_pos_enemigoy = enemigos[i].py - (enemigos[i].vel_pasos * enemigos[i].dir_y);
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

int detect_col_enemigo_player(int i)
{
      int enemigo_x1,enemigo_x2,enemigo_y1,enemigo_y2,diferencia_x,diferencia_y;
      diferencia_x = 10;
      diferencia_y = 10;
      enemigo_x1 = enemigos[i].px-diferencia_x;
      enemigo_x2 = enemigos[i].px+40-diferencia_x;
      enemigo_y1 = enemigos[i].py+diferencia_y;
      enemigo_y2 = enemigos[i].py+40+diferencia_y;
      if(detec_punto_espacio(enemigo_x1,enemigo_x2,enemigo_y1,enemigo_y2,player.px,player.py) || detec_punto_espacio(enemigo_x1,enemigo_x2,enemigo_y1,enemigo_y2,player.px+40,player.py) || detec_punto_espacio(enemigo_x1,enemigo_x2,enemigo_y1,enemigo_y2,player.px,player.py+40) || detec_punto_espacio(enemigo_x1,enemigo_x2,enemigo_y1,enemigo_y2,player.px+40,player.py+40))
      {
            if(tiempo_dano == 0 && player.vida>0)
            {
                  player.vida=player.vida - 1;
                  tiempo_dano=80;
            }
      }
}

int detec_punto_espacio(int plano_x1,int plano_x2,int plano_y1, int plano_y2, int punto_x, int punto_y)
{
      if(punto_x >= plano_x1 && punto_x <= plano_x2 && punto_y >= plano_y1 && punto_y <= plano_y2)
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
            enemigos[i].px -= enemigos[i].vel_pasos * enemigos[i].dir_x;
      }
}

void mov_enem_vertical(int i, int prox_pos_enemigoy)
{
      int diferencia=4;
      if(mapa[(prox_pos_enemigoy)/40][(enemigos[i].px + diferencia)/40] != '1' && mapa[(prox_pos_enemigoy+40)/40][(enemigos[i].px + diferencia)/40] != '1' && mapa[(prox_pos_enemigoy)/40][(enemigos[i].px+40 - diferencia)/40] != '1' && mapa[(prox_pos_enemigoy+40)/40][(enemigos[i].px+40 - diferencia)/40] != '1')
      {
            enemigos[i].py -= enemigos[i].vel_pasos * enemigos[i].dir_y;
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
void agarrar_objeto(char obj) //elemento de la matriz se transforma a 0
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
                  if(mapa[i][j]=='B')
                  {
                        if(mapa[(player.py+40)/40][player.px/40]=='B'||mapa[player.py/40][(player.px+40)/40]=='B'||mapa[player.py/40][(player.px-4)/40]=='B'||mapa[(player.py-4)/40][player.px/40]=='B')
                        {
                              if(cont_baterias < MAXBATERIAS)
                              {
                                    agarrar_objeto('B');
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
                  if(mapa[i][j]=='2') //PUERTA
                  {
                        if(player.llave==1)
                        {
                              if(mapa[(player.py-4)/40][player.px/40]=='2' || mapa[(player.py+44)/40][player.px/40]=='2' || mapa[(player.py)/40][(player.px-4)/40]=='2' || mapa[(player.py)/40][(player.px+44)/40]=='2')//ABRE PUERTA
                              {
                                    printf("estado_puerta=%d",estado_puerta);
                                    estado_puerta=1;
                                    player.llave=0;
                              }
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
                  if(mapa[i][j]=='L')
                  {
                        if(mapa[(player.py+40)/40][player.px/40]=='L'||mapa[player.py/40][(player.px+40)/40]=='L'||mapa[player.py/40][(player.px-4)/40]=='L'||mapa[(player.py-4)/40][player.px/40]=='L')
                        {
                              agarrar_objeto('L');
                              player.llave=1;
                              printf("player.llave=%d",player.llave);
                        }
                  }
            }
      }
}

void indicador_bateria(int cont_baterias)
{
      int pos_x=180;
      int pos_y=0;
      switch (cont_baterias)
      {
      case 0:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,0,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      case 1:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,120,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      case 2:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,240,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      case 3:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,360,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      case 4:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,480,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      case 5:
            stretch_blit(indicador_bateria_img,indicador_bateria_bmp,600,0,720,40,0,0,720,40);
            draw_sprite(buffer,indicador_bateria_bmp,pos_x,pos_y);
            break;
      }
}
void indicador_vida()
{
      int pos_x=40;
      int pos_y=0;
      switch (player.vida)
      {
      case 0:
            stretch_blit(vida_bmp,vida,0,0,360,40,0,0,360,40);
            draw_sprite(buffer,vida,pos_x,pos_y);
            break;
      case 1:
            stretch_blit(vida_bmp,vida,120,0,360,40,0,0,360,40);
            draw_sprite(buffer,vida,pos_x,pos_y);
            break;
      case 2:
            stretch_blit(vida_bmp,vida,240,0,360,40,0,0,360,40);
            draw_sprite(buffer,vida,pos_x,pos_y);
            break;
      case 3:
            stretch_blit(vida_bmp,vida,360,0,360,40,0,0,360,40);
            draw_sprite(buffer,vida,pos_x,pos_y);
            break;
      }
}

void destruir_bitmaps()
{
      destroy_bitmap(player.personaje);
      destroy_bitmap(player.personajebmp);
      destroy_bitmap(buffer);
      destroy_bitmap(pared);
      destroy_bitmap(bateria);
      destroy_bitmap(bateria_bmp);
      destroy_bitmap(luz);
      destroy_bitmap(luz_bmp);
      destroy_bitmap(puerta);
      destroy_bitmap(puerta_bmp);
     // destroy_bitmap(enemigo.enemigo);
   //   destroy_bitmap(enemigo.enemigobmp);
}




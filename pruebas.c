void importar_nivel(int nvl)
{
      char *nombre[11];
      if(nvl==1)
      {
         nombre[11]="nivel1.txt";
      }
      if(nvl==2)
      {
         nombre[11]="nivel2.txt";
      }
      leer_archivo(nombre[11]);
}

void leer_archivo(char *nombre_archivo)
{
      int i,j;
      FILE *fdata; //es un tipo de estructura definida como ARCHIVO. Se considera un tipo de dato opaco ya que su implementación está oculta. No sabemos qué constituye el tipo, solo usamos el puntero al tipo y la biblioteca conoce el interior del tipo y puede usar los datos.
      fdata=fopen(nombre_archivo,"r"); //abre el nombre de archivo al que apunta, por nombre de archivo usando el modo dado "r" Abre un archivo para lectura
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
                  if(mapa[i][j]==8)//COORDENADAS DE INICIO PERSONAJE EN MAPA
                  {
                        pos_inicialx=j*40;
                        pos_inicialy=i*40;
                  }
            }
      }
      fclose(fdata);
}

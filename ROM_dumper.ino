//Codigo para leer y/o escribir una ram y comunicarse por consola serie
// 
// comandos:
// eco<LF> -> debe responder "ok<LF>"
// r:xxxx<LF> donde xxxx= numero hexa entre 0000 y FFFF, -> debe responder yy<LF>, yy=numero hexa entre 0 y FF
// w:xxxx:yy<LF> dodne xxxx = numero hexa entre 0000 y FFFF, yy=numero hexa entre 0 y FF, debe responder "ok<LF>
// v<LF>, debe contestar la version de soft: "v0.1<LF>"

#define VERSION "v0.2"

#define pin_A0  20
#define pin_A1  21
#define pin_A2  22
#define pin_A3  23
#define pin_A4  24
#define pin_A5  25
#define pin_A6  26
#define pin_A7  27
#define pin_A8  20
#define pin_A9  21
#define pin_A10  36
#define pin_A11  37
#define pin_A12  38
#define pin_A13  39
#define pin_A14  40
#define pin_A15  41

#define pin_D0  28
#define pin_D1  29
#define pin_D2  30
#define pin_D3  31
#define pin_D4  32
#define pin_D5  33
#define pin_D6  34
#define pin_D7  35

#define pin_xOE_LATCH 42
#define pin_xWR_RAM   43
#define pin_xOE_RAM   44
//---------------------- defines y ctes Puerto serial ---------------
#define BUFFER_UART 18              //longitud buffer UART
#define TIMEOUT_UART  5             //cuantos ms de timeout
#define INTERVALO_TRANSMISION 1000  //transmite cada x milisecs
char recibidos[BUFFER_UART];        //buffer de caracteres recibidos
char caracter;                      //caracter recibido
char indice=0;                      //indice de caracteres recibidos
uint8_t do_parse=0;
uint8_t cant_recibidos;             //este va a mantener el numero de caracteres recibidos 
unsigned long timeout_caracteres=0; //con esto voy a manejar un timeout entre caracteres recibidos
unsigned long timeout_transmision=0;  //para transmitir cada x tiempo
char *arg_uno, *arg_dos, *arg_tres, *arg_cuatro, *arg_cinco;         //buffer temporal multiuso
char *copia_puntero;
char **endptr;
uint16_t num_temporal;  //numero temporal para hacer barbaridades impunemente

//comandos
const char CMD_ECO[] PROGMEM="eco"; //comando 1  e: eco, si recibe 'eco' manda "ok"
const char CMD_R[] PROGMEM="r";   //comando 2  r: read
const char CMD_W[] PROGMEM="w";   //comando 3  w: write
const char CMD_G[] PROGMEM="g";   //comando 4  g: goto
const char CMD_V[] PROGMEM="v";   //version

void setup() 
{
digitalWrite(pin_xOE_LATCH, LOW);   // run
digitalWrite(pin_xWR_RAM, HIGH);    //no voy a scribir
digitalWrite(pin_xOE_RAM, LOW);     //salida de ram habilitada
  
pinMode(pin_A0, INPUT);
pinMode(pin_A1, INPUT);
pinMode(pin_A2, INPUT);
pinMode(pin_A3, INPUT);
pinMode(pin_A4, INPUT);
pinMode(pin_A5, INPUT);
pinMode(pin_A6, INPUT);
pinMode(pin_A7, INPUT);
pinMode(pin_A8, INPUT);
pinMode(pin_A9, INPUT);
pinMode(pin_A10, INPUT);
pinMode(pin_A11, INPUT);
pinMode(pin_A12, INPUT);
pinMode(pin_A13, INPUT);
pinMode(pin_A14, INPUT);
pinMode(pin_A15, INPUT);

pinMode(pin_xOE_LATCH, OUTPUT);
pinMode(pin_xWR_RAM, OUTPUT);
pinMode(pin_xOE_RAM, OUTPUT);

pinMode(pin_D0, INPUT);
pinMode(pin_D1, INPUT);
pinMode(pin_D2, INPUT);
pinMode(pin_D3, INPUT);
pinMode(pin_D4, INPUT);
pinMode(pin_D5, INPUT);
pinMode(pin_D6, INPUT);
pinMode(pin_D7, INPUT);

Serial.begin(9600);
Serial.println("Ok");
}

//-------- rutina que chequea num en arreglo y convierte si no es mayor que coso ---------
int atoi_check(char *arreglo)
{
uint8_t contador=0;

while (arreglo[contador]!='\0')
  {
  if (arreglo[contador]>='0' && arreglo[contador]<='9') contador++;
  else {contador=0; break;}
  }
if (contador!=0 && strlen(arreglo)<5) return(atoi(arreglo));  //si el arreglo tiene menos de 5 lugares (9999) convierto. Si no, me jui.
else return (0xFFFF);
}

//============== Funcion para leer una direccion de la RAM ===============
uint8_t lee_ram(uint16_t address)
{
uint8_t data;
digitalWrite(pin_xOE_LATCH, HIGH);  //deshabilito LATCH
pinMode(pin_A0, OUTPUT);
pinMode(pin_A1, OUTPUT);
pinMode(pin_A2, OUTPUT);
pinMode(pin_A3, OUTPUT);
pinMode(pin_A4, OUTPUT);
pinMode(pin_A5, OUTPUT);
pinMode(pin_A6, OUTPUT);
pinMode(pin_A7, OUTPUT);
pinMode(pin_A8, OUTPUT);
pinMode(pin_A9, OUTPUT);
pinMode(pin_A10, OUTPUT);
pinMode(pin_A11, OUTPUT);
pinMode(pin_A12, OUTPUT);
pinMode(pin_A13, OUTPUT);
pinMode(pin_A14, OUTPUT);
pinMode(pin_A15, OUTPUT);
if((address & 0x01)!=0) digitalWrite(pin_A0, HIGH); else digitalWrite(pin_A0, LOW);
if((address & 0x02)!=0) digitalWrite(pin_A1, HIGH); else digitalWrite(pin_A1, LOW);
if((address & 0x04)!=0) digitalWrite(pin_A2, HIGH); else digitalWrite(pin_A2, LOW);
if((address & 0x08)!=0) digitalWrite(pin_A3, HIGH); else digitalWrite(pin_A3, LOW);
if((address & 0x10)!=0) digitalWrite(pin_A4, HIGH); else digitalWrite(pin_A4, LOW);
if((address & 0x20)!=0) digitalWrite(pin_A5, HIGH); else digitalWrite(pin_A5, LOW);
if((address & 0x40)!=0) digitalWrite(pin_A6, HIGH); else digitalWrite(pin_A6, LOW);
if((address & 0x80)!=0) digitalWrite(pin_A7, HIGH); else digitalWrite(pin_A7, LOW);   
if((address & 0x100)!=0) digitalWrite(pin_A8,  HIGH); else digitalWrite(pin_A8, LOW);
if((address & 0x200)!=0) digitalWrite(pin_A9,  HIGH); else digitalWrite(pin_A9, LOW);
if((address & 0x400)!=0) digitalWrite(pin_A10, HIGH); else digitalWrite(pin_A10, LOW);
if((address & 0x800)!=0) digitalWrite(pin_A11, HIGH); else digitalWrite(pin_A11, LOW);
if((address & 0x1000)!=0) digitalWrite(pin_A12, HIGH); else digitalWrite(pin_A12, LOW);
if((address & 0x2000)!=0) digitalWrite(pin_A13, HIGH); else digitalWrite(pin_A13, LOW);
if((address & 0x4000)!=0) digitalWrite(pin_A14, HIGH); else digitalWrite(pin_A14, LOW);
//hago la composicion de data
if((digitalRead(pin_D0)!=0)) data  = 0x01; else data  =  0x00;
if((digitalRead(pin_D1)!=0)) data |= 0x02; else data &= ~0x02;
if((digitalRead(pin_D2)!=0)) data |= 0x04; else data &= ~0x04;
if((digitalRead(pin_D3)!=0)) data |= 0x08; else data &= ~0x08;
if((digitalRead(pin_D4)!=0)) data |= 0x10; else data &= ~0x10;
if((digitalRead(pin_D5)!=0)) data |= 0x20; else data &= ~0x20;
if((digitalRead(pin_D6)!=0)) data |= 0x40; else data &= ~0x40;
if((digitalRead(pin_D7)!=0)) data |= 0x80; else data &= ~0x80;
pinMode(pin_A0, INPUT);
pinMode(pin_A1, INPUT);
pinMode(pin_A2, INPUT);
pinMode(pin_A3, INPUT);
pinMode(pin_A4, INPUT);
pinMode(pin_A5, INPUT);
pinMode(pin_A6, INPUT);
pinMode(pin_A7, INPUT);
pinMode(pin_A8, INPUT);
pinMode(pin_A9, INPUT);
pinMode(pin_A10, INPUT);
pinMode(pin_A11, INPUT);
pinMode(pin_A12, INPUT);
pinMode(pin_A13, INPUT);
pinMode(pin_A14, INPUT);
pinMode(pin_A15, INPUT);
digitalWrite(pin_xOE_LATCH, LOW);  //habilito LATCH
return(data);
}

//=========== Funcion para escribir una direccion de la RAM ===============
void escribe_ram(uint16_t address, uint8_t dato)
{
digitalWrite(pin_xOE_LATCH, HIGH);  //deshabilito LATCH
digitalWrite(pin_xOE_RAM, HIGH);    //deshabilito salida RAM
pinMode(pin_A0, OUTPUT);  //hago todo salida
pinMode(pin_A1, OUTPUT);
pinMode(pin_A2, OUTPUT);
pinMode(pin_A3, OUTPUT);
pinMode(pin_A4, OUTPUT);
pinMode(pin_A5, OUTPUT);
pinMode(pin_A6, OUTPUT);
pinMode(pin_A7, OUTPUT);
pinMode(pin_A8, OUTPUT);
pinMode(pin_A9, OUTPUT);
pinMode(pin_A10, OUTPUT);
pinMode(pin_A11, OUTPUT);
pinMode(pin_A12, OUTPUT);
pinMode(pin_A13, OUTPUT);
pinMode(pin_A14, OUTPUT);
pinMode(pin_A15, OUTPUT);
pinMode(pin_D0, OUTPUT);
pinMode(pin_D1, OUTPUT);
pinMode(pin_D2, OUTPUT);
pinMode(pin_D3, OUTPUT);
pinMode(pin_D4, OUTPUT);
pinMode(pin_D5, OUTPUT);
pinMode(pin_D6, OUTPUT);
pinMode(pin_D7, OUTPUT);
if((address & 0x01)!=0) digitalWrite(pin_A0, HIGH); else digitalWrite(pin_A0, LOW);
if((address & 0x02)!=0) digitalWrite(pin_A1, HIGH); else digitalWrite(pin_A1, LOW);
if((address & 0x04)!=0) digitalWrite(pin_A2, HIGH); else digitalWrite(pin_A2, LOW);
if((address & 0x08)!=0) digitalWrite(pin_A3, HIGH); else digitalWrite(pin_A3, LOW);
if((address & 0x10)!=0) digitalWrite(pin_A4, HIGH); else digitalWrite(pin_A4, LOW);
if((address & 0x20)!=0) digitalWrite(pin_A5, HIGH); else digitalWrite(pin_A5, LOW);
if((address & 0x40)!=0) digitalWrite(pin_A6, HIGH); else digitalWrite(pin_A6, LOW);
if((address & 0x80)!=0) digitalWrite(pin_A7, HIGH); else digitalWrite(pin_A7, LOW);   
if((address & 0x100)!=0) digitalWrite(pin_A8,  HIGH); else digitalWrite(pin_A8, LOW);
if((address & 0x200)!=0) digitalWrite(pin_A9,  HIGH); else digitalWrite(pin_A9, LOW);
if((address & 0x400)!=0) digitalWrite(pin_A10, HIGH); else digitalWrite(pin_A10, LOW);
if((address & 0x800)!=0) digitalWrite(pin_A11, HIGH); else digitalWrite(pin_A11, LOW);
if((address & 0x1000)!=0) digitalWrite(pin_A12, HIGH); else digitalWrite(pin_A12, LOW);
if((address & 0x2000)!=0) digitalWrite(pin_A13, HIGH); else digitalWrite(pin_A13, LOW);
if((address & 0x4000)!=0) digitalWrite(pin_A14, HIGH); else digitalWrite(pin_A14, LOW);
if ((dato & 0x01) !=0) digitalWrite(pin_D0, HIGH); else digitalWrite(pin_D0, LOW);
if ((dato & 0x02) !=0) digitalWrite(pin_D1, HIGH); else digitalWrite(pin_D1, LOW);
if ((dato & 0x04) !=0) digitalWrite(pin_D2, HIGH); else digitalWrite(pin_D2, LOW);
if ((dato & 0x08) !=0) digitalWrite(pin_D3, HIGH); else digitalWrite(pin_D3, LOW);
if ((dato & 0x10) !=0) digitalWrite(pin_D4, HIGH); else digitalWrite(pin_D4, LOW);
if ((dato & 0x20) !=0) digitalWrite(pin_D5, HIGH); else digitalWrite(pin_D5, LOW);
if ((dato & 0x40) !=0) digitalWrite(pin_D6, HIGH); else digitalWrite(pin_D6, LOW);
if ((dato & 0x80) !=0) digitalWrite(pin_D7, HIGH); else digitalWrite(pin_D7, LOW);
delay(1); //esto no hace falta, pero quiero evitar bit spilling y carreras y coso
digitalWrite(pin_xWR_RAM, LOW);
delay(1);
digitalWrite(pin_xWR_RAM, HIGH);  //pulso para escribir RAM
 

pinMode(pin_A0, INPUT); //hgo todo entrada
pinMode(pin_A1, INPUT);
pinMode(pin_A2, INPUT);
pinMode(pin_A3, INPUT);
pinMode(pin_A4, INPUT);
pinMode(pin_A5, INPUT);
pinMode(pin_A6, INPUT);
pinMode(pin_A7, INPUT);
pinMode(pin_A8, INPUT);
pinMode(pin_A9, INPUT);
pinMode(pin_A10, INPUT);
pinMode(pin_A11, INPUT);
pinMode(pin_A12, INPUT);
pinMode(pin_A13, INPUT);
pinMode(pin_A14, INPUT);
pinMode(pin_A15, INPUT);
pinMode(pin_D0, INPUT);
pinMode(pin_D1, INPUT);
pinMode(pin_D2, INPUT);
pinMode(pin_D3, INPUT);
pinMode(pin_D4, INPUT);
pinMode(pin_D5, INPUT);
pinMode(pin_D6, INPUT);
pinMode(pin_D7, INPUT);
digitalWrite(pin_xOE_LATCH, LOW);  //deshabilito LATCH
digitalWrite(pin_xOE_RAM, LOW);    //deshabilito salida RAM
}


//********************************** void loop *************************************
void loop() 
{
uint8_t datos[256];
uint8_t numero;
uint16_t filas, columnas;
uint8_t in_data, out_data;
uint16_t out_add;

while(1)
  {
 // ################################### Rutina de envio y recepcion serial #############################
    // ---- Recepcion y bufereo de caracteres -------
 if ((Serial.available()>0)) //si hay algun acaracter en el puerto serial...lo descargo
  {
  timeout_caracteres=millis()+TIMEOUT_UART; //actualizo el tiempo en el que recibi el ultimo caracter
  if((caracter=Serial.read())=='\n')        //leo el caracter recibido
    {
    recibidos[indice]='\0';  
    do_parse=1;
    indice=0;
    }
  else 
    {
    recibidos[indice]=caracter;
    if((++indice)>=BUFFER_UART-1) indice=BUFFER_UART-1;
    strnlen(recibidos, BUFFER_UART-1);
    }
  }
 else if((millis()>=timeout_caracteres) && indice!=0)
    {
    indice=0;
    recibidos[indice]='\0'; 
    do_parse=0;           //por las dudas 
    }
    
  // ########################## parsing de lo que acaban de mandar ##################################### 
 if(do_parse!=0)  // tengo que hacer el parseo
    {
    do_parse=0;
    copia_puntero=recibidos;                                  //creo una copia del puntero para poder usar sin corromperlo
    arg_uno= strtok_r(copia_puntero,",:\n", &copia_puntero);   //busco el primer token, coma o \n

    if(arg_uno != NULL)  //si es NULL no se encontro ningun token, apunta al primer char de la cadena
      {
      //*arg_uno='X';    //ahora este puntero apunta a la primera letra del arreglo y está terminado en \0
      arg_dos= strtok_r(copia_puntero,",:\n", &copia_puntero); //busco el segundo token, coma o \n
      if (arg_dos != NULL) // si estoy aca es campo1,campo2<\n>
        {      
        arg_tres= strtok_r(copia_puntero,",:\n", &copia_puntero);  //busco el tercer token, coma o \n  
        if (arg_tres!=NULL)               // si estoy aca se trata de tres campos, campo1,campo2,campo3
          {
          if (strcmp_P(arg_uno,CMD_W)==0 ) // tengo tres campos, el primero es el comando Write?
             {
             out_add=(int)strtol(arg_dos, endptr, 16);     //convierto el primer campo que debe ser address
             out_data = (uint8_t)strtol(arg_tres, endptr, 16); //convierto el segundo campo, data
             escribe_ram(out_add, out_data);
             Serial.println("ok");
             }      
          }
        else    //se trata de una cadena de dos campos, M1,215<CR>
          {
          if (strcmp_P(arg_uno,CMD_R)==0)  //tengo dos campos, el primero es igual a REad?
            {
            out_add=(int)strtol(arg_dos, endptr, 16);
            in_data=lee_ram(out_add);     //leo el dato de la direccion de la RAM
            Serial.println(in_data, HEX);
            }
          else if (strcmp_P(arg_uno,CMD_G)==0)  // comando = goto
            {
            Serial.println("ok");
            //TODO 
            }
          }//cierre else argdos
        }//cierre if argdos!=nul
      else    //, ->se trata de una cadena de un solo campo: M0<CR>
        {
        if (strcmp_P(recibidos,CMD_ECO)==0)  //ejecuto si el campo=ECO
            { 
            Serial.println("ok");  //el comando es eco, envío un eco.  
            }
        else if (strcmp_P(recibidos,CMD_V)==0)  //ejecuto si el campo=VERSION
            { 
            Serial.println(VERSION);  //el comando es version.  
            }   
        }//cierre else un solo campo
      }// if arguno!=nul
    else    //primer token esta vacio, o sea no hay primer token
      {
      }
    }//cierre if do parse

  }// cierre while 1
//##################################################################################


while(1); // While eterno

}

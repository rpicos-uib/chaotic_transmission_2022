

//Se inicializan las condiciones iniciales 
//de las ecuaciones diferenciales
float vt11, vt12, vt13;
float vt21, vt22, vt23;
int Ft1, Ft2, Ft3;
float v0;
int M;
int h;
int MXOR;


//Se inicializan los coeficientes de las 
//ecuaciones diferenciales
float a11, a12, a13, a14, a15, a16;
float b11, b12, b13, b14;

//Inicialización de variables que permiten
//sincronizar y llevar a cabo la comunicación
int contadorM;
bool MensajeEnviado;
bool ReceptorDisponible;


//Creamos el Mensaje
char Mensaje[] = "En un lugar de la Mancha, de cuyo nombre no quiero acordarme, no ha mucho tiempo que vivía un hidalgo de los de lanza en astillero, adarga antigua, rocín flaco y galgo corredor.Una olla de algo más vaca que carnero, salpicón las más noches, duelos y quebrantos los sábados, lantejas los viernes, algún palomino de añadidura los domingos, consumían las tres partes de su hacienda. El resto della concluían sayo de velarte, calzas de velludo para las fiestas, con sus pantuflos de lo mesmo, y los días de entresemana se honraba con su vellorí de lo más fino. Tenía en su casa una ama que pasaba de los cuarenta, y una sobrina que no llegaba a los veinte, y un mozo de campo y plaza, que así ensillaba el rocín como tomaba la podadera. Frisaba la edad de nuestro hidalgo con los cincuenta años; era de complexión recia, seco de carnes, enjuto de rostro, gran madrugador y amigo de la caza. Quieren decir que tenía el sobrenombre de Quijada, o Quesada, que en esto hay alguna diferencia en los autores que deste caso escriben; aunque por conjeturas verosímiles se deja entender que se llamaba Quijana. Pero esto importa poco a nuestro cuento: basta que en la narración dél no se salga un punto de la verdad.";
int X;
byte LetraBinario;





void setup() {
  // start serial port at 115200 bps:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //se dan valores iniciales a la ecuacion recurrente
  vt12 = 0;
  vt13 = 0;
  vt22 = 0.75;
  vt23 = vt22-0.25*0;
  v0 = 0.35;
  
  Ft2 = 0;
  Ft3 = 0;

  //se dan valores a los coeficientes de 
  //la ecuación recurrente
  a11 = 421.0 / 425;
  a12 = 4.0 / 1275;
  a13 = 12.0 / 25;
  a14 = 4.0 / 27;
  a15 = 16.0 / 15;
  a16 = 16.0 / 45;

  b11 = 12.0 / 25;
  b12 = 4.0 / 25;
  b13 = 421.0 / 425;
  b14 = 4.0 / 1275;


  //se dan valores a las variables del sistema
  contadorM = 1;
  M = 0;
  MensajeEnviado = 0;
  ReceptorDisponible = 0;
  
  //Preparamos los pines para su respectivo
  //modo de funcionamiento y enviamos un 0
  //al receptor para que este a la espera de
  //un mensaje
  pinMode(30,OUTPUT);
  pinMode(34,INPUT);
  pinMode(32,OUTPUT);
  digitalWrite(32,MensajeEnviado);

  //se obtiene la longitud del mensaje
  X = strlen(Mensaje);
  
  delay(10000);
}

void loop() {  

 
 int i = 0;
 contadorM = 0;
 M = 1;
 
 while (i < X){
  
  LetraBinario = byte(Mensaje[i]);
  Serial.print((char)LetraBinario);
  int a = 0;
  
  while ( a < 8){

    if (ReceptorDisponible = digitalRead (34)){
      
      /************ Se elige si enviar el mensaje o la inicializacion de este **************/
      
        if (contadorM < 24){
          if (contadorM%8 == 0){
            M = 1-M;
            }
          contadorM++;
        }else{
          if ((LetraBinario & 128) == 128){
            M = 1;
            }else{
              M = 0; 
              }
          LetraBinario <<= 1;
          a++;  
          }
      
      /************** Ecuaciones Transmisor *******************/
        vt11 = a11 * vt12 + a12 * vt13 + a13 * vt22;
        vt11 = vt11 - a14 * vt23 + a15 * Ft2 - a16 * Ft3;
        vt21 = -b11 * vt12 + b12 * vt13;
        vt21 = vt21 + b13 * vt22 + b14 * vt23;
        
        h = (v0 < vt21) ? 1 : 0;
      
        MXOR = (M^h);
        Ft1 = MXOR;
        
        
        // Se actualizan los valores
        vt23 = vt22;
        vt22 = vt21;
      
        vt13 = vt12;
        vt12 = vt11;
      
        Ft3 = Ft2;
        Ft2 = Ft1;
    
      
      /************ Comprobacion ***************/
//        Serial.print(contadorM);
//        Serial.print(":   El mensaje original es  ");
//        Serial.print(M);
//        Serial.print("  El mensaje enviado es ");
//        Serial.print(Ft1);
//        Serial.println(); 
      
      
      /************ envío del mensaje ***************/
      
        digitalWrite(30,Ft1);//Envía el mensaje
        MensajeEnviado = 1;
        digitalWrite(32,MensajeEnviado); //Le indica al receptor que hay un mensaje disponible
        delay(10);      

    
        //Se le informa al receptor que espere hasta el proximo mensaje
        MensajeEnviado = 0;  
        digitalWrite(32,MensajeEnviado);
       
    }
    
   }
   if (Mensaje[i]== '.'){Serial.println();}
   i++;
   delay(10);
   
  }
  Serial.println();
}





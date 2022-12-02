

//Se inicializan las condiciones iniciales 
//de las ecuaciones diferenciales
float vr11, vr12, vr13;
float vr21, vr22, vr23;
int Fr1, Fr2, Fr3;
float v0;
int Mr;
int hr;
int MXOR;

//Se inicializan los coeficientes de las 
//ecuaciones diferenciales
float a11, a12, a13, a14, a15, a16;
float b11, b12, b13, b14;

//Inicialización de variables que permiten
//sincronizar y llevar a cabo la comunicación
int NumeroMensaje;
bool MensajeDisponible;
bool ReceptorDisponible;

//Variables decodificación mensaje
char Mensaje[200];
int X;
byte LetraBinario;
byte LetraCodificada;

void setup() {
  // start serial port at 115200 bps:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //se dan valores iniciales a la ecuacion recurrente
  vr12 = 0;
  vr13 = 0;
  vr22 = 0.75;
  vr23 = vr22-0.25*0;
  v0 = 0.35;
  
  Fr2 = 0;
  Fr3 = 0;

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
  Mr = 0;
  NumeroMensaje = 0;
  MensajeDisponible = 0;
  ReceptorDisponible = 1;
  LetraCodificada = 0;

  pinMode(30,INPUT);
  pinMode(34,OUTPUT);
  pinMode(32,INPUT);
  delay(1000);

  digitalWrite(34,ReceptorDisponible);
  delay(5000);
}

void loop() {

  // Se comprueba que haya un mensaje disponible en el transmisor
  // en caso de haber mensaje disponible lo lee y desencripta
    
  if (MensajeDisponible = digitalRead(32)){
    ReceptorDisponible = 0;
    digitalWrite(34,ReceptorDisponible);
    delay(10);
   
    /***** Lectura Mensaje ********/
   
    Fr1 = digitalRead(30); 


    /********** Ecuaciones Receptor **********/
    
    vr11 = a11 * vr12 + a12 * vr13 + a13 * vr22;
    vr11 = vr11 - a14 * vr23 + a15 * Fr2 - a16 * Fr3;
    vr21 = -b11 * vr12 + b12 * vr13;
    vr21 = vr21 + b13 * vr22 + b14 * vr23;

    hr = (v0 < vr21) ? 1 : 0;
    MXOR = (Fr1^hr);
    Mr = MXOR;

    // Se actualizan los valores
          
    vr23 = vr22;
    vr22 = vr21;

    vr13 = vr12;
    vr12 = vr11;

    Fr3 = Fr2;
    Fr2 = Fr1;
     
  /************ Comprobacion ***************/
  
  //    Serial.print(NumeroMensaje);
  //    Serial.print(":   El mensaje original es  ");
  //    Serial.print(Mr);
  //    Serial.print("  El mensaje recibido es  ");
  //    Serial.print(Fr1);
  //    Serial.println(); 
        
  /************ Se almacena el mensaje ***************/
      
    LetraBinario = LetraBinario | Mr;
    LetraCodificada = LetraCodificada | Fr1;
    
    if (NumeroMensaje < 7){
      LetraBinario <<= 1;
      LetraCodificada <<= 1;
    }
    
    NumeroMensaje++;
    //Serial.print(LetraBinario);
    
    ReceptorDisponible = 1;
    digitalWrite(34,ReceptorDisponible);
    delay(10);

     if (NumeroMensaje%8 == 0){

        if (LetraBinario != 255 && LetraBinario != 0){
          Serial.print((char)LetraCodificada);
          if((char)LetraBinario == '.'){Serial.println();}
          //Serial.print(" ");
          
        }else{
          Serial.println();        
          }

        NumeroMensaje = 0;
        LetraBinario = 0;
        LetraCodificada = 0;
      }
 
     
   }

}

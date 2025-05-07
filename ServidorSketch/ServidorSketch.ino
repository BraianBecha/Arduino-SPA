
#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <ArduinoJson.h>


const PROGMEM char moduloIndex[][250] = { "<!DOCTYPE html><html>    <head>        <meta charset='utf-8'>  <title>Arduino Server</title>  <meta name='viewport' content='width=device-width, initial-scale=1'>      ",
"  </head>  <body> <header>    <strong>Arduino Server</strong>  </header>  <div  class='menu-nav'>",
"    <table class='table-menu'> <tr> <th> <button id='home'><b>Home</b></button></th>   <th>  <button id='b1'> <b>Table</b></button> </th>  <th>  <button id='b2'> <b>Links</b></button> </th> </tr>  </table>  </div>",
" <div class='container'>    </div>  <footer> &copy; 2025   </footer>   <script>   const botonHome = document.getElementById('home');",
"   const botonB1 = document.getElementById('b1');        const botonB2 = document.getElementById('b2');   const divContainer = document.getElementsByClassName('container');                      funcionHome();",
"   addStyle();   botonB1.onclick =funcionb1;        botonHome.onclick=funcionHome; function funcionb1(){ fetch('http://192.168.1.25/data')",
"  .then(response => response.json())  .then(data => {    console.log(data);    headTab = ['Pin number' , 'Value '];",
"tablaAnalogicos = construirTabla( data.analog.pinName.length , headTab.length , data.analog.pinName, data.analog.pinValue ,'Analog Sample', headTab);",
"tablaDigitales = construirTabla( data.digital.pinName.length , headTab.length , data.digital.pinName, data.digital.pinValue ,  'Digital Sample', headTab);tablas = tablaAnalogicos + tablaDigitales;",
"const divContainer = document.getElementsByClassName('container');divContainer[0].innerHTML = ' '; divContainer[0].innerHTML = ` ${tablas}`;                                })                                        };",
" function funcionHome(){   console.log('se llamo a funcionHome');  const divContainer = document.getElementsByClassName('container');  divContainer[0].innerHTML = ''; ",
" divContainer[0].innerHTML = ` <h1> Data Measurement Interface </h1>  <p> Lorem ipsum dolor sit amet consectetur adipisicing elit. ",
" Ea minima expedita ipsa neque dicta sunt nobis libero magni fuga.    Quis molestiae error esse repudiandae dolor quae iste impedit natus deserunt. </p>         `;            }",
"  function construirTabla(fi, co, arrNom, arrDat, t , th){ fi = arrNom.length;  co=th.length;   re = '<table> ';                    re += (`<h2> ${t} </h2>`);",
" if(fi>0 && co>0){ re+= '<tr> ';  for(k=0; k<th.length ;k++){                                re+=`<th> ${th[k]} </th> `;   };                    re+= '</tr> ';",
"  for(i=0;i<fi; i++){  re+=('<tr> ');  re+=(`<td> ${arrNom[i]} </td> <td> ${arrDat[i]} </td>`);                    ",
"  re+=('</tr>');  };  } re.concat('</table>');  return re; };    function addStyle(){",
" console.log('llamada a addStyle');    const stE = document.createElement('style'); stE.innerText=`<style>h1 {font-size: 2em;margin-bottom: 0.5em;}  body {color: #434f54;",
"margin: 0;padding: 20px;font-family: sans-serif;line-height: 1.6;max-width: 600px;margin-left: auto;margin-right: auto;height:100%;} .container{ color: #434f54;  margin: 5em auto;  max-width: 1170px;",
"  min-height: 35em;  }h1 {font-size: 2em;margin-bottom: 0.5em;} p {margin-bottom: 1.5em;font-size: 1em;} form {display: flex;flex-direction: column;gap: 10px;} input, textarea, button {font-size: 1em;",
"padding: 10px;border: 1px solid #ccc;border-radius: 5px;} button {cursor: pointer;border: none;} header, footer {text-align: center;padding: 15px 0;color: #fff;} header {border-bottom: 1px solid #fff;margin-bottom: 20px;",
"background-color: #008184;} footer {background-color: #008184;border-top: 1px solid #ccc;margin-top: 30px;font-size: 0.9em;} table, th, td {border:1px solid  #434f54;border-radius: 0.3em;} table{width:100%; border-radius: 0.5em;",
"background-color: #71b3b4;padding: 0.2em;} .table-menu{background-color: #fff;color: #434f54; width: 99.5%;border-radius: 0%;} td{text-align: center;} .table-menu  button,th {outline: none;border: none;background: transparent;",
"appearance: none;  } @media (max-width: 480px) {body {padding: 15px;} h1 {font-size: 1.5em;} input, textarea, button {font-size: 0.95em;padding: 8px;} }  </style>`; document.head.appendChild(stE); };",
" </script>  </body>  </html>"

};


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 25);
EthernetServer server(80);

 int inAnalog[]={ 12 , 13 , 14 , 15, 3};
 int inDigit[]={ 53 , 51 , 50 , 49};

void setup() {

 bool flag1=true;
 int k=0;
 char* ptrModuloHead[]={""};
 char* ptrModuloCuerpoGenerico[] ={""};
  Serial.begin(9600);  
  Serial.println("Ethernet WebServer");

  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); 
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  
  EthernetClient client = server.available(); 
  
  if (client) {
    Serial.println("new client");     
    boolean currentLineIsBlank = true;   
      if (client.available()) {
        String request ="";
        int k=0;
        char c;
        while (k<30){
          c = client.read();
          request += c;
           k++;
           };
           Serial.println("request es");
      Serial.println(request);
      int casus = analizarRequest(request);
      darRespuesta(client , casus);
       
      };
    
    delay(1000);   
    client.stop();
    Serial.println("client disconnected");
  };
};
    
    int analizarRequest(String request)
    {
      int r=0;
      bool flag=false;

      if((request.startsWith("GET HTTP") || request.startsWith("GET / HTTP"))&& flag==false){
            Serial.println("solicitud index");
            r = 1; 
             Serial.println(r);
            flag=true; 
                          
      };
     
      if(request.startsWith("GET /data HTTP")&& flag==false ){
            Serial.println("solicitud de json");
             r = 2; 
             flag=true;       
                
      };
      if(!((request.startsWith("GET HTTP") || request.startsWith("GET /index HTTP") || request.startsWith("GET / HTTP")  ))&& flag==false){
        Serial.println("404 not found");
        r = 0;
        flag=true;
      };
    Serial.println(r);
      return r;
      };



        void darRespuesta(EthernetClient& clie, int casus){

          switch (casus){

            case 0: clie.println("404 not found");
            break;

            case 1: enviarIndex(clie); //index 
            break;

            case 2: enviarJson(clie); //JSON
            break;

          };
         


         }

         void enviarIndex(EthernetClient& clie){
          char buffer[255];
            clie.println("HTTP/1.1 200 OK");
            clie.println("Content-Type: text/html");
            clie.println('\n');           
                                              
            for( int i=0; i<(sizeof(moduloIndex)/sizeof(moduloIndex[0])) ; i++){     
                
               Serial.println("for1");

               strcpy_P(buffer, moduloIndex[i]);   
               clie.println(buffer);
               
              };

            clie.println('\n');

            
         };

         void enviarJson(EthernetClient& clie){
          String cResp= construirJson();
          
            clie.println("HTTP/1.1 200 OK");
            clie.println("Content-Type: application/json");           
            clie.println('\n');
            clie.println(cResp);
            

         };


        String construirJson(){
           JsonDocument resp;
           int medida;
           String strResp = "";
           int sizAn = sizeof(inAnalog)/sizeof(inAnalog[0]);
           int sizDig= sizeof(inDigit)/sizeof(inDigit[0]);
          

           for(int i=0; i<sizAn ; i++){        
            String numElem = String(inAnalog[i]);     
            resp["analog"]["pinName"][i]="A"+numElem;
             resp["analog"]["pinValue"][i]= analogRead(inAnalog[i])*5/1023;

           };

           for(int j=0; j<sizDig;j++){
             String numElem = String(inDigit[j]);
            resp["digital"]["pinName"][j]= "D"+ numElem;
            resp["digital"]["pinValue"][j]=  digitalRead(inDigit[j]);
           };

           
            serializeJson(resp, strResp);
            Serial.println(strResp);

          return strResp;
         };


#include "Shell.cpp"
#include <windows.h>
#include <iostream>

void Dialogo(char *p, COLORREF **a, int m, int n);
void Dialogo2(char *e, COLORREF **b, int r, int s);

#define nRens 1200
#define nCols 600

Window ventana("Proyecto final",nRens,nCols);

#define DialogoX 50
#define DialogoY 450 
#define NameposX 50
#define NameposY 420
#define cuentox  150
#define cuentoy 250

int main(){
	
	/*Defino variables importantes*/
	int nr,nc,nr2,nc2,a=125,b=125;
	
	
	/*Defino la Matriz del escenario 1*/
	COLORREF **mat;
	DimensionesBmp("Paisaje Introductorio.bmp",&nr,&nc);
	
	mat = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	
	for(int k=0; k<nc;k++){
		mat[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	
	/*Deino variables importantes para inicializar el programa*/
	int x,y,i=1,counter=0;
	
	/*Inicio del programa*/
	int resultado = (int) MessageYesNo("Desea experimentar una experiencia virtual");
	
	/*Leer el primer mapa*/
	LeeBmpColor(mat,"Paisaje Introductorio.bmp");
	
	/*Leer el segundo mapa
	LeeBmpColor(mat2,"Escenario-1.bmp");*/
	
	
	
	//ventana.PlotCBitmap(mat, nc, nr, 0,0,1);
	
	if (resultado == 1){
	
	ventana.printf(DialogoX,DialogoY,"Hola my Darling...");
	
	}
	else{
		ventana.printf("El usuario presiono No");
		return 0;
	}
	
	/*ESCENA 1: PROLOGO*/
	
	char soundfile[] = "ost.wav";
    PlaySound((LPCSTR)soundfile,NULL,SND_FILENAME | SND_ASYNC);	

Dialogo("¿Como estas?",mat,nr,nc);

Dialogo("Veo, que no la has pasado muy bien estos dias... ", mat, nr, nc);

Dialogo("Si, te puedo entender",mat, nr,nc);

Dialogo("No te proecupes my Darling, todo va a estar muy bien, Cree en mi", mat, nr, nc);

Dialogo("Pronto muy pronto volveremos a vernos", mat, nr, nc);

Dialogo("Recuerdas aquel día en que me llevaste a ver las estrellas", mat, nr,nc);

Dialogo("Ese día lo atesoro en el fondo de mi corazon", mat, nr, nc);

Dialogo("De verdad estoy muy agradecida, ", mat, nr,nc);

Dialogo("..................................", mat, nr,nc);


/*Defino Matriz 2*/


/*Defino la Matriz del escenario 1*/
	COLORREF **vec;
	DimensionesBmp("Escenario-1.bmp",&nr,&nc);
	
	vec = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	
	for(int k=0; k<nc;k++){
		vec[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}

     LeeBmpColor(vec,"Escenario-1.bmp");
    
    Dialogo("¿Pasa algo my darling?",vec,nr,nc);
    
    Dialogo("Ultimamente te he visto... algo....",vec,nr,nc);
    
    Dialogo("        ",vec,nr,nc);
    
    Dialogo("Disperso",vec,nr,nc);
    
    Dialogo("Creo que puedo ayudarte a sentir mejor",vec,nr,nc);

    Dialogo("Te contare una historia ",vec,nr,nc);
    
    Dialogo("Esta historia me la llego a contar una vez mi abuelo",vec,nr,nc);
    
    Dialogo("Todavia me acuerdo perfectamente", vec,nr,nc);
    
    Dialogo("No te preocupes my darling si quieres podemos parar aqui en este lugar", vec,nr,nc);
    
    Dialogo2("(La bicicleta se detuvo, era una noche de verano como cualquier otra)",mat,nr,nc);
    
    Dialogo2("(Las estrellas tiritaban en el cielo vasto y hermoso)",mat,nr,nc);
    
    Dialogo2("(El sonido de las cigarras en verano resonaban en el alba)", mat, nr, nc);
    
    Dialogo2("(Ella se veía hermosa, bajo la luz de las estrellas)", mat, nr,nc);
    
    Dialogo2("(Quien iba a pensar que despues de tanta tragedia en el mundo)", mat,nr,nc);
    
    Dialogo2("(Yo seria feliz con ella)", mat,nr,nc);
    
    Dialogo("Ven my darling, recostemonos en esta colina y admiremos las estrellas", vec,nr,nc);
    
    Dialogo("Recuestate en mis brazos, y disfruta de la historia que hoy te voy a contar", vec,nr,nc);
    
    Dialogo("Erase una vez...", vec,nr,nc);
    
    
    /*Defino Matriz 3*/
    
/*Defino la Matriz del escenario 3*/


    COLORREF **tai;
    DimensionesBmp("Escenario-Intro.bmp",&nr,&nc);
    
    tai=(COLORREF**)malloc(nc*sizeof(COLORREF));
    
    for(int k=0; k<nc;k++){
    	tai[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(tai,"Escenario-Intro.bmp");
	
	Dialogo2("",tai,nr,nc);
	
	Dialogo2("Hace mucho tiempo, antes de la gran guerra",tai,nr,nc);
	
	Dialogo2("Antes de las pestes", tai,nr,nc);
	
	Dialogo2("Antes de las crisis economicas", tai,nr,nc);
	
	Dialogo2("Antes de las pandemias", tai, nr, nc);
	
	Dialogo2("Antes de que el mundo purgara todos sus males", tai,nr,nc);
	
	Dialogo2("(A medida de que Lisa iba contando la historia yo empezaba a cerrar los ojos imaginandome aquellas historias)", tai, nr,nc);
	
	Dialogo2("(El tiempo se detiene y la historia comienza a sentirse)", tai, nr,nc);
	
	Dialogo("Bienvenido seas my darling, a este viaje astral", tai,nr,nc);
	
	Dialogo("Desde aqui podras sentir mi historia, que con tanto amor prepare para ti", tai,nr,nc);
	
	COLORREF **ast;
	
	DimensionesBmp("Apocalipsis.bmp",&nr,&nc);
	
	ast=(COLORREF**)malloc(nc*sizeof(COLORREF));
	
	for(int k=0; k<nc;k++){
		ast[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(ast,"Apocalipsis.bmp");
	
	Dialogo("Cuando mi abuelo vivia en esta ciudad, reinaba una tanquilidad",ast,nr,nc);
	
	Dialogo("A veces la sociedad era caotica en aquellos tiempos", ast,nr,nc);
	
	Dialogo("Pero usualmente las personas vivian de la rutina",ast,nr,nc);
	
	Dialogo("Las personas de aquellos tiempos no valoraban mas que las cosas materiales", ast,nr,nc);
	
	Dialogo("Las cosas materiales que el consumismo les ofrecia", ast,nr,nc);
	
	Dialogo("Las personas de aquellos tiempos no valoraban lo mas importante",ast,nr,nc);
	
	Dialogo2("OLVIDARON AMAR",tai,nr,nc);
	
	Dialogo2("OLVIDARON SENTIR QUE ESTABAN VIVOS", tai,nr,nc);
	
	Dialogo("Mi abuelo vivio en tiempos interesantes", ast, nr,nc);
	
	Dialogo("La sociedad del pasado queria los beneficios de la ciencia", ast,nr,nc);
	
	Dialogo("Pero aquellas personas no querian asumir la responsablidad de la ciencia", ast,nr,nc);
	
	COLORREF **way;
	
	DimensionesBmp("Hojas.bmp",&nr,&nc);
	
	way=(COLORREF**)malloc(nc*sizeof(COLORREF));
	
	for(int k=0; k<nc;k++){
		way[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(way,"Hojas.bmp");
	
	Dialogo2("Pero bebieron del caliz del poder", way,nr,nc);
	
	Dialogo2("Y en vez de enfocarse en las cosas bellas del mundo", way,nr,nc);
	
	Dialogo2("Empezaron a destruirse entre ellos mismos", way, nr,nc);
	
	Dialogo2("(Quede impresionado en como Lisa me llevaba a traves de hermosos paisajes)", way,nr,nc);
	
	Dialogo2("(Lisa caminaba a traves de mis sueños, era un hermoso paisaje)", way,nr,nc);
	
	Dialogo2("(Por primera vez Lisa me estaba enseñando fragmentos de sus recuerdos)", way,nr,nc);
	
	Dialogo("Asi es my Darling", way,nr,nc);
	
	COLORREF **travel;
	
	DimensionesBmp("Bosques-personaje.bmp",&nr,&nc);
	
	travel=(COLORREF**)malloc(nc*sizeof(COLORREF));
	
	for(int k=0; k<nc;k++){
		travel[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(travel,"Bosques-personaje.bmp");
	
	Dialogo("Yo creci aqui",travel,nr,nc);
	
	Dialogo("Antes de la humanidad cayera", travel, nr,nc);
	
	Dialogo("Producto de su propia ambicion", travel,nr,nc);
	
	Dialogo("Mi abuelo me refugio en estos bosques", travel,nr,nc);
	
	Dialogo("El solia estar arrepentido de que su ciencia fuera usada para el uso militar", travel, nr,nc);
	
	Dialogo("Usualmente lo veia muy triste caminando en estos bosques", way,nr,nc);
	
	Dialogo("Pero estaba determinado a cambiar la situación", way,nr,nc);
	
	Dialogo2("(Cuando Lisa termino de hablar tenia muchisima curiosidad)", way,nr,nc);
	
	Dialogo2("(Lisa..., ella era la nieta de *************)", way,nr,nc);
	
	Dialogo2("(El gran cientifico que sacrifico todo por enmendar sus pecados)", way,nr,nc);
	
	Dialogo2("(Quien salvo a la humanidad de su propia ambicion)", way,nr,nc);
	
	Dialogo("Mi abuelo trabajaba dia y noche", way,nr,nc);
	
	Dialogo("Lo veia desvelarse hasta tarde", travel,nr,nc);
	
	Dialogo("Cuando vio su obra terminada, me regalo este lindo uniforme", travel,nr,nc);
	
	Dialogo("Lo ultimo que me dijo fue", travel,nr,nc);
	
	COLORREF **sky;
	
	DimensionesBmp("The-world.bmp",&nr,&nc);
	
	sky=(COLORREF**)malloc(nc*sizeof(COLORREF));
	
	for(int k=0; k<nc;k++){
		sky[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(sky,"The-world.bmp");
	
	Dialogo2("**Cada vez que el mundo te ponga aprueba**", sky,nr,nc);
	
	Dialogo2("**Usualmente sera muy dificil salir del hoyo**", sky,nr,nc);
	
	Dialogo2("**Y no te lo digo para deprimirte**", sky,nr,nc);
	
	Dialogo2("**Va a haber un dia en donde te sientas decaida**", sky,nr,nc);
	
	Dialogo2("**Habra situaciones en las cuales la vida te derrote multiples veces**", sky,nr,nc);
	
	Dialogo2("**No le vas a ganar al mundo**", sky, nr,nc);
	
	Dialogo2("**Es muy dificil, y solo te causaras dolor a ti misma**", sky,nr,nc);
	
	Dialogo2("**Pero cuando la vida te llegue a derrotar, levantate...**", sky,nr,nc);
	
	Dialogo2("**Levantate**", sky,nr,nc);
	
	Dialogo2("**Levantate aunque sea de rodillas primero, pero levantate**", sky, nr,nc);
	
	Dialogo2("**A partir de ahora vas a vivir una vida muy dificil**", sky,nr,nc);
	
	Dialogo2("**Pero te juro que va a ser una experiencia maravillosa**", sky,nr,nc);
	
	Dialogo2("**Pero recuerda esto mi querida Lisa**", sky,nr,nc);
	
	Dialogo2("**Si algun dia te sientes decaida, solo recuerda a tu querido abuelo**", sky,nr,nc);
	
	Dialogo2("**Que a partir de mañana se ira al cielo para ver a tu abuela**", sky,nr,nc);
	
	Dialogo2("**Si un dia llegas a amar a alguien de verdad, recuerdale lo mucho y especial que es**", sky,nr,nc);
	
	Dialogo2("**Y motivalo a seguir adelante, asi como tu seguiras adelante... sin mi**", sky,nr,nc);
	
	COLORREF **drm;
	
	DimensionesBmp("Star-sad.bmp",&nr,&nc);
	
	drm=(COLORREF**)malloc(nc*sizeof(COLORREF));
	
	for(int k=0; k<nc;k++){
		drm[k]=(COLORREF*)malloc(nc*sizeof(COLORREF));
	}
	
	LeeBmpColor(drm,"Star-sad.bmp");
	
	Dialogo2("(Lisa queria demasiado a su abuelo)", drm,nr,nc);
	
	/*Dialogo2("(Sentia un calor inconmesurable en mi pecho)", drm,nr,nc);
	
	Dialogo2("(Compartiendome sus recuerdos con tal dulzura)", drm,nr,nc); */
	
	Dialogo2("Lisa era la chica mas hermosa que habia visto", drm,nr,nc);
	
	Dialogo2("La mas amable", drm,nr,nc);
	
	Dialogo2("La mas tierna", drm,nr,nc);
	
	Dialogo2("Y la mas feliz en toda la tierra", drm,nr,nc);
	
	Dialogo2("(Su hermoso cabello negro era como aquel cielo estrellado que tanto visualice)", sky,nr,nc);
	
	Dialogo2("(Sus hermosos ojos dislumbraban como estrellas al alba)", sky,nr,nc);
	
	Dialogo2("(Aquella tranquilidad que me trasmitia)", way,nr,nc);
	
	Dialogo2("(Eran como aquellos arboles)", way,nr,nc);
	
	Dialogo2("(Oh... Arboles de la barranca, como han enverdecido)", way,nr,nc);
	
	Dialogo2("(Sus palabaras me acaban de dar vida)", sky, nr,nc);
	
	Dialogo2("",sky,nr,nc);
	
	Dialogo2("***Abrí mis ojos lentamente***", sky,nr,nc);
	
	Dialogo2("", mat,nr,nc);
	
	Dialogo2("(Los hermosos ojos de Lisa me veian con atencion)", vec,nr,nc);
	
	Dialogo2("(Bajo este cielo infinito)", mat,nr,nc);
	
	Dialogo2("(Bajo estas luces)", mat,nr,nc);
	
	Dialogo2("(Este cielo infinito que impera)", sky,nr,nc);
	
	Dialogo2("",sky,nr,nc);
	
	Dialogo("No te vayas a rendir nunca en la vida my Darling",vec,nr,nc);
	
	Dialogo("Recuerda", vec,nr,nc);
	
	Dialogo("Levantate aunque sea de rodillas", vec,nr,nc);
	
	Dialogo("Y si algo te agobia my darling", vec,nr,nc);
	
	Dialogo("Recuerda que tu familia te ama", vec,nr,nc);
	
	Dialogo("Tus amigos te aman", vec,nr,nc);
	
	Dialogo("Y sobretodo y lo mas importante", sky,nr,nc);
	
	Dialogo("Yo...", sky,nr,nc);
	
	Dialogo("Te amo!", vec,nr,nc);
	
	Dialogo("", mat,nr,nc);
	
	Dialogo2("*******NOVELA VISUAL CREADA POR JUAN ANGEL LOPEZ DELGADILLO*****************",mat,nr,nc);
	
	Dialogo2("************FIN*************",sky,nr,nc);
	
	Dialogo2("",sky,nr,nc);


	for(int k=0; k<nc; k++){
		free(mat[k]);
	}
	
	free(mat);
	
	/*--------------------------*/
	
	for(int k=0; k<nc; k++){
		free(vec[k]);
	}
 free(vec);
 
 
 /*--------------------------*/
 for(int k=0; k<nc;k++){
 	free(tai[k]);
 }
free(tai);

/*----------------------------*/

for(int k=0; k<nc; k++){
	free(ast[k]);
}
free(ast);

/*-----------------------*/

for(int k=0; k<nc; k++){
	free(way[k]);
}

free(way);

/*------------------------*/

for(int k=0; k<nc; k++){
	free(travel[k]);
}

free(travel);

/*----------------------*/

for(int k=0; k<nc; k++){
	free(sky[k]);
}
free(sky);

/*----------------------*/

for(int k=0; k<nc; k++){
	free(drm[k]);
}
free(drm);


/*
while(i){
	Sleep(100);
    int click =MouseHit(&x,&y);	
    	if(click==1){
    		ventana.Cls();
    		ventana.printf(DialogoX,DialogoY,"Todo va a estar bien");
    		Sleep(500);	
    		i=0;
	}
}
*/

/*i=1;
while(i){
	Sleep(100);
    int click =MouseHit(&x,&y);	
    	if(click==1){
    		ventana.Cls();
    		ventana.printf(DialogoX,DialogoY,"Solo tienes que creer en ti amigo :D");
    		Sleep(500);	
    		i=0;
	}
}
*/
	
	return MainLoop();
}



void Dialogo(char *e, COLORREF **b, int r, int s){
	
	int x,y,i=1;
while(i){
	Sleep(200);
    int click = MouseHit(&x,&y);	
    	if(click==1){
    		ventana.Cls();
    		ventana.PlotCBitmap(b, r, s, 0,0,1);
    		ventana.printf(NameposX,NameposY,"Lisa:");
    		ventana.printf(DialogoX,DialogoY,"%s",e);
    		Sleep(500);	
    		i=0;
	}
}
	
}


void Dialogo2(char *p, COLORREF **a, int m, int n){
	
		int x,y,i=1;
    while(i){
	Sleep(200);
    int click = MouseHit(&x,&y);	
    	if(click==1){
    		ventana.Cls();
    		ventana.PlotCBitmap(a, m, n, 0,0,0.99);
    		ventana.printf(cuentox,cuentoy,"%s",p);
    		Sleep(500);	
    		i=0;
	}
}
	
}




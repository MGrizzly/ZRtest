//functions

char area, photos[3];
int flare;

float  POI[3][3], position[3], velocity[3];


//sphere

void goTo(float * p){
    float a[3];
    sub(a,p,position);
    if (mag(a)<=0.1f||
    (mag(velocity)>0.005f&&angleBetween(velocity,a)>0.05f)){
    	api.setPositionTarget(p);
    	d((" 67 "));
    }else{
        div(a,a, 5);
        api.setVelocityTarget(a);
    }
}
int timeTo(float *target_pos){
    float a[3];
    sub(a,target_pos,position);
    return 2*sqrtf(210*mag(a))+1;
}
void update(){
    float tmp[12];
    int i;
    api.getMyZRState(tmp);
    for(i=0;i<3;i++)
        position[i]=tmp[i];
    for(i=0;i<3;i++)
        velocity[i]=tmp[i+3]; 
    
    float ra=mag(position);
    area='n'; //none
    if(position[0]>=0&&abs(position[1])<=0.2f&&abs(position[2])<=0.2f)
        area='s'; //shadow zone
    else if(ra<0.42f)
        area='i'; //inner zone
    else if(ra<0.53f)
        area='o'; //outer zone
    
    flare=game.getNextFlare();
    if(flare==-1)  flare=31;
    
    for(int i=0;i<3;i++){
        game.getPOILoc(tmp, i);
        POI[i][0]=tmp[0];
        POI[i][1]=tmp[1];
        POI[i][2]=tmp[2];
    }
    if(api.getTime()%60==0){
        for(i=0;i<3;i++)
            photos[i]='-';
    }
}

void take(int id){
    float a[3];
    sub(a,position, POI[id]);
    if(angleBetween(a,POI[id])<=(area=='i'?0.8f:0.4f)&&game.alignLine(id)&&photos[id]!=area&&photos[id]!='b'){
        int memoryvalid=game.getMemoryFilled();
    	game.takePic(id);
    	if(memoryvalid+1==game.getMemoryFilled()){
    		if(photos[id]=='i'||photos[id]=='o')
    			photos[id]='b';
    		else
    			photos[id]=area;
    	}
    }
}

float out[3]; //used in avoidAst
float * avoidAst(float * target){
	float ad[3], tmp[3];
	sub(ad,target, position);
	clone(tmp, position);
	clone(out,target);
	float min=100, len;
	div(ad,ad,100);
	for(int i=0;i<100;i++){
		add(tmp,tmp, ad);
		len=mag(tmp);
		if(len<=0.33f&&len<min){
			clone(out, tmp);
			setMag(out,out, 0.35f);
			min=len;
		}
	}
	return out;
}

void zPOI(float *a, int i){
    a[0]=0;
    a[1]=POI[i][1];
    a[2]=sqrtf(0.04f-POI[i][1]*POI[i][1]);
}
void iPOI(float *wPOI, int id){
  setMag(wPOI,POI[id], 0.42f);
  int tm=0;
  do{
      rotateY(wPOI,wPOI,-0.1f);
      tm++;
  }
  while((timeTo(wPOI)+2-tm)>0.5f);
}

//Contains all the display routines. 
void sequencer(byte pat[], byte method[], int times[], byte length){
//  /*
//  METHOD
//  0: RandomNoise (disregards sequence, 'times is density (-1 is default)')
//  1: Scroll Left
//  2: Scroll Right
//  3: Scroll Up
//  4: Scroll Down
//  5: Flash
//  6: Set
//  */  
uint8_t temp[16][2];


  for (int i=0;i<length;i++){
    //Load values from PROGMEM...Does this work?
    for (int j=0;j<16;j++){
      for (int k=0;k<2;k++){
        temp[j][k]=pgm_read_byte(&(patterns[pat[i]][j][k]));
      }  
    }
    
    switch(method[i]){
      case 0://RANDOM NOISE
        randomNoise(-1,times[i]);
      break;
      case 1://SCROLL LEFT
        patternScrollLeft(temp,times[i]);
      break;
      case 2://SCROLL RIGHT
        patternScrollRight(temp,times[i]);
      break;
      case 3://Scroll UP
        patternScrollUp(temp,times[i]);
      break;
      case 4://Scroll DOWN
        patternScrollDown(temp,times[i]);
      break;
      case 5://Flash
        patternFlash(temp,times[i]);
      break;
      case 6:
        patternSet(temp,times[i]);
      break;
    }//End switch
  }//End for 
}

/*
RANDOM NOISE
*/
void randomNoise(int density,int del){
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while((endtime-starttime)<=del){
      if(density==-1)density=30;//This looks the best CRAY 
      //---Fill the pattern with random on and offs
        for(int i=0;i<density;i++){
          bitWrite(curPat[(int)random(0,16)][(int)random(0,2)],(int)random(0,8),(int)random(0,2));
        }
      clearPattern();
      endtime=millis();
  }
}
/*
Pattern Flash, nodelete
*/
void patternSet(byte nPat[16][2], int del){
byte n[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    n[i][j]=nPat[i][j];  
  }
}
    setPattern(n);
    delay(del);

}

/*
PATTERN FLASH
*/
void patternFlash(byte nPat[16][2], int del){
byte n[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    n[i][j]=nPat[i][j];  
  }
}
    clearPattern();
    delay(del);  
    setPattern(n);
    delay(del);

}
/*
PATTERN SCROLL UP
*/
void patternScrollUp(byte nPat[16][2],int del){
  byte in[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    in[i][j]=nPat[i][j];  
  }
}
  for (int k=0;k<16;k++){
    for (int i=0; i<15; i++){
        for(int j=0; j<2; j++){
          curPat[i][j]=curPat[i+1][j];
        }
    }
    for (int j=0;j<2;j++){
      curPat[15][j]=in[k][j];
    }
    delay(del);
  }
return;
}
/*
PATTERN SCROLL DOWN
*/
void patternScrollDown(byte nPat[16][2], int del){
byte in[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    in[i][j]=nPat[i][j];  
  }
}
  for (int k=15;k>=0;k--){
    for (int i=15; i>0; i--){
        for(int j=0; j<2; j++){
          curPat[i][j]=curPat[i-1][j];
        }
    }
    for (int j=0;j<2;j++){
      curPat[0][j]=in[k][j];
    }
    delay(del);
  }
return;
}
/*
PATTERN SCROLL LEFT
*/
void patternScrollLeft(uint8_t nPat[16][2], int del){
//  byte nextPattern[16][2];
//  //Fill nextPattern with the one we want. We need this so we can shift the bits left.
//  for (int i=0; i<16; i++){
//    for(int j=0; j<2; j++){
//      nextPattern[i][j]=in[i][j];
//    }
//  }
byte nextPattern[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    nextPattern[i][j]=nPat[i][j];  
  }
}
  for (int j=0; j<16; j++){//Do this 16times, as there are 16 columns.
    for (int i=0; i<16; i++){//For each row
      //Shift the bits left by one, compensating for the fact that we are using 2 separate bytes.
      curPat[i][0]=curPat[i][0]<<1; 
      bitWrite(curPat[i][0],0,bitRead(curPat[i][1],7));
      curPat[i][1]=curPat[i][1]<<1;
      //Read in the bit that comes from the next pattern and shift the next pattern left. 
      bitWrite(curPat[i][1],0,bitRead(nextPattern[i][0],7));//Read in the next bit
      nextPattern[i][0]=nextPattern[i][0]<<1;
      bitWrite(nextPattern[i][0],0,bitRead(nextPattern[i][1],7));
      nextPattern[i][1]=nextPattern[i][1]<<1;      
    }  
      delay(del);
  }
  return;
}
/*
PATTERN SCROLL RIGHT
*/
void patternScrollRight(byte nPat[16][2], int del){
//  byte nextPattern[16][2];
//  //Fill nextPattern with the one we want. We need this so we can shift the bits left.
//  for (int i=0; i<16; i++){
//    for(int j=0; j<2; j++){
//      nextPattern[i][j]=patterns[n][i][j];
//    }
//  }
byte nextPattern[16][2];
for (int i=0;i<16;i++){
  for (int j=0;j<2;j++){
    nextPattern[i][j]=nPat[i][j];  
  }
}
  for (int j=15; j>=0; j--){//Do this 16times, as there are 16 columns.
    for (int i=15; i>=0; i--){//For each row
      //Shift the bits right by one, compensating for the fact that we are using 2 separate bytes.
      curPat[i][1]=curPat[i][1]>>1; 
      bitWrite(curPat[i][1],7,bitRead(curPat[i][0],0));
      curPat[i][0]=curPat[i][0]>>1;
      //Read in the bit that comes from the next pattern and shift the next pattern left. 
      bitWrite(curPat[i][0],7,bitRead(nextPattern[i][1],0));//Read in the next bit
      nextPattern[i][1]=nextPattern[i][1]>>1;
      bitWrite(nextPattern[i][1],7,bitRead(nextPattern[i][0],0));
      nextPattern[i][0]=nextPattern[i][0]>>1;      
    }  
      delay(del);
  }
  return;
}

void lines(int del){
  clearPattern();
  for (int i=0;i<16;i++){
    for(int j=0;j<2;j++){
      for(int k=7;k>=0;k--){
        bitWrite(curPat[i][j],k,1);
        delay(del);
      }
    }
  }
}

void spiral(int del,int mode){
/*
MODE: 1 - Spiral On
MODE: 2 - Spiral Off
*/  

//if (mode==1){clearPattern();}

  for (int r=0;r<8;r++){
//Top 
    for(int j=0;j<2;j++){
      if (j==1){
        for (int k=7;k>=r;k--){
        bitWrite(curPat[r][j],k,mode);
        delay(del);
        }     
      }
      else{
        for (int k=7-r;k>=0;k--){
          bitWrite(curPat[r][j],k,mode);
          delay(del);
        }
      }
    }
 //Right side
      for (int i=r;i<16-r;i++){
        bitWrite(curPat[i][1],r,mode);
        delay(del);
      }
 //Bottom     
     for(int j=1;j>=0;j--){
       if (j==1){
        for (int k=r;k<8;k++){
          bitWrite(curPat[15-r][j],k,mode);
          delay(del);
        }   
       }
       else{
          for (int k=0;k<8-r;k++){
          bitWrite(curPat[15-r][j],k,mode);
          delay(del);
        }
       }
       
    }
//Left Side
     for (int i=15-r;i>=r;i--){
        bitWrite(curPat[i][0],7-r,mode);
        delay(del);
      }
 
  }
}

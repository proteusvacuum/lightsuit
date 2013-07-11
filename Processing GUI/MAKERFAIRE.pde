import processing.serial.*;

//Buttons
int numButtons = 9;
ImageButtons[] buttons = new ImageButtons[numButtons];
ImageButtons uploadButton;
//Keyboard input

PFont fontA;
List<Byte> output;

PImage maker;
PImage[] images = new PImage[numButtons];
PImage uploadImage;
//Serial
Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

void setup()
{
  size(450, 250); 
  loadImages();
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600); 
  
  fontA = loadFont("Ziggurat-HTF-Black-32.vlw");

//  PImage[] buttonImages = {
//    loadImage("car.bmp"), loadImage("dolla.bmp"),loadImage("eyecentre.bmp"),loadImage("goose.bmp"),loadImage("heart.bmp"),loadImage("house.bmp"),loadImage("sadface.bmp"),loadImage("sun.bmp"),loadImage("thumb.bmp")
//  };
  // = loadImage("upload.bmp");
  
  for (int i = 0;i<numButtons;i++){
    int x = i*(width-30)/numButtons + 15;
    int y = 175;
    int w = images[i].width;
    int h = images[i].height;
    buttons[i] = new ImageButtons(x, y, w, h, images[i], images[i], images[i]);
  }
  uploadButton = new ImageButtons(15,205,uploadImage.width,uploadImage.height, uploadImage,uploadImage,uploadImage);
  
  output = new ArrayList();        
}
void loadImages(){
  images[0] = loadImage("car.bmp");
  images[1] =loadImage("dolla.bmp");
  images[2] = loadImage("eyecentre.bmp");
  images[3] =loadImage("goose.bmp");
  images[4] =loadImage("heart.bmp");
  images[5] =loadImage("house.bmp");
  images[6] =loadImage("sadface.bmp");
  images[7] =loadImage("sun.bmp");
  images[8] =loadImage("thumb.bmp");
  
  uploadImage = loadImage("upload.bmp");
  
  maker = loadImage("maker.jpg");
}

void draw()
{
  background(102, 102, 102);
  // Draw banner
  image(maker,15,15,width-30,100-45);
  //Draw buttons
  for (int i=0;i<numButtons;i++){
    buttons[i].update();
    if (buttons[i].pressed){
//      println("BOTTON" + i);
      addCharacter(i);
    }
    buttons[i].display();
  }
  uploadButton.update();
  uploadButton.display();
  if (uploadButton.pressed){
    sendToSuit();
  }
  
  
  textFont(fontA, 15);
  text("Type some words:", 15, 100);
  pushStyle();
  fill(0);
  stroke(255);
  rect(12,115,width-30,20);
  popStyle();
  
  //Display input:
  //is it text? display it as a letter.
 
  for (int i=0;i<output.size();i++){
    if (output.get(i)<=26){ // then it is a letter
      if(output.get(i)>=1 && output.get(i) <=26){
        char a = (char)(output.get(i)+64);
        text(a,15+15*i,130);
      }
      if (output.get(i)>=97 && output.get(i) <=172){
        text((output.get(i)+96),15+15*i,130);
      }
    }
    else{// it's a picture
        switch(output.get(i)){
          case 31:
            image(images[0],15+15*i,116);
            break;
          case 28:
            image(images[1],15+15*i,116);
            break;
          case 40:
            image(images[2],15+15*i,116);
            break;            
          case 43:
            image(images[3],15+15*i,116);
            break; 
           case 27:
            image(images[4],15+15*i,116);
            break;
          case 32:
            image(images[5],15+15*i,116);
            break; 
          case 29:
            image(images[6],15+15*i,116);
            break; 
          case 30:
            image(images[7],15+15*i,116);
            break; 
          case 33:
            image(images[8],15+15*i,116);
            break;             
        }
    }
      
  }
  //text(keyboardInput,15,130);
  
  text("Add some sweet characters:", 15, 160);
  textFont(fontA, 10);
  text("Upload!",15,235);   
  
  if ( myPort.available() > 0) {  // If data is available,
    char hello = (char)myPort.read();
    print(hello);         // read it and store it in val
  }
  
}

void keyPressed(){
 // keyboardInput = keyboardInput+key;
  //println((int)key);
  if (keyCode == BACKSPACE){
    //keyboardInput = "";
    output.clear();
  }
  parseInput(key);
}

void parseInput(char _in){
  int in = (int)_in;
  if (output.size()<=25){
    if (in == 32){
      output.add((byte)0);
    }
    else if (in>=65 && in <=90){
      output.add((byte)(in - 64));
    }
    else if (in>=97 && in <=172){
      output.add((byte)(in - 96));
    }
  //  for (int i=0;i<output.size();i++){
  //    println(output.get(i));
  //  }
  }
}
void addCharacter(int img){
  if (output.size()<=25){
    switch(img){
      case 0: //car
        output.add((byte)31);
        break;
       case 1: //dola
         output.add((byte)28);
         break;
       case 2: //eye
         output.add((byte)40);
         break;
       case 3: //goose
         output.add((byte)43);
         break;  
       case 4: //heart
         output.add((byte)27);
         break;  
       case 5: //house
         output.add((byte)32);
         break; 
       case 6: //sad
         output.add((byte)29);
         break; 
       case 7: //sun
         output.add((byte)30);
         break;     
       case 8: //thumbs
         output.add((byte)33);
         break;        
    }
  }
}

void sendToSuit(){
//  myPort.write(126); //start comms
if (output.size()>0){
  myPort.write(output.size()); //tell it how many. 
 //println("OUTPUT SIZE: " + output.size());
  for (int i=0; i<output.size();i++){

    myPort.write((byte)output.get(i));
  }
  myPort.write(127); //end comms
//  for (int i=0;i<output.size();i++){
//    println((byte)output.get(i));
//  }
//output.clear(); //delete output
}
}

void mouseReleased(){
  for (int i=0;i<numButtons;i++){
    if (buttons[i].over) 
      buttons[i].released = true;
  }
  if (uploadButton.over) 
    uploadButton.released = true;  
}

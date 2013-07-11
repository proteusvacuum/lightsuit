class Button
{
  int x, y;
  int w, h;
  color basecolor, highlightcolor;
  color currentcolor;
  boolean over = false;
  boolean pressed = false;   
  boolean released = true;
  
  void pressed() {       
    
    if(over && mousePressed&&released) {                  
      pressed = true;
      released = false;
    } else {
      pressed = false;
    }    
  }
  
  boolean overRect(int x, int y, int width, int height) {
  if (mouseX >= x && mouseX <= x+width && 
      mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}
}



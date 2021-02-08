/*
 * image_to_bytearray.pde
 *
 * Copyright (c) 2021 murabi10_c1815
 */

PImage img;
 
void setup() {
  
  String output = "";
  img = loadImage("test.jpg");
  
  output+=("#include <stdio.h>\n\nuint8_t Image[] = {\n");
    
  for (int a = 0; a < height; a++) {
    
    for (int i = 0; i < width; i+=2) {
      
      int bw = get_pixel(i, a) << 4;
      bw |= get_pixel(i+1, a);
      
      output+=("0x" + hex(bw, 2) + ((a+1 >= height && i+1 >= width) ? "" : ", "));
      
    }
    output+="\n";
  }
  
  output+=("};");
  saveStrings( "Image.h", output.split("\n", 0)) ;
}
 
int get_pixel(int i, int a) {
  
    int c = img.get(i, a);
    
    float r, g, b;
    r = (c >>16) & 0xFF;
    g = (c >> 8) & 0xFF;
    b =  c       & 0xFF;
    
    return floor((((0.3 * r) + (0.59 * g) + (0.11 * b)) * 16) / 256);
    
}

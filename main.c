#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.c"
#include "matrix.c"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  
  struct matrix * edges;
  struct matrix * transform;

  //clear_screen(s);

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  
  /*
  color c; 
  c.red = 255;
  c.blue = 100;
  c.green = 180;

  printf("\nOriginal: \n"); 
  print_matrix(transform); 

  printf("\nCreating translation matrix with x+=1,y+=2,z+=3 \n"); 
  transform = make_translate(1, 2, 3);
  print_matrix(transform);
 
  printf("\nEdges matrix: \n");
  
  edges->m[0][0] = 10;
  edges->m[1][3] = 5;
  edges->m[2][0] = 2;
  edges->m[3][3] = 1;
  edges->m[1][1] = 4;
  edges->m[3][2] = 7;
  edges->m[2][2] = 1;
  

  edges->m[0][0] = 1;
  edges->m[1][0] = 3;
  edges->m[2][0] = 1;
  edges->m[3][0] = 1;
  
  print_matrix(edges);

  printf("\nTranslating edges: \n");
  matrix_mult(transform, edges);
  print_matrix(edges);

  
  printf("\nTesting scaling:\n"); 
  transform =  make_scale(3, 25, 1);
  print_matrix(transform);
  
  printf("\nScaling edges: \n");
  matrix_mult(transform, edges);
  print_matrix(edges);
  
  */
  
 
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  
  free_matrix( edges );
  free_matrix( transform );
}  

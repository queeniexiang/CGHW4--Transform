#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    //LINE
    if (! strcmp(line, "line")) {
      double x0, y0, z0, x1, y1, z1;
      //Grabs next line of the coordinates to be added 
      fgets(line, 255, f);

      //Stores the coordinates into variables 
      sscanf(line, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);

      //Adds coordinates to the matrix
      add_edge(edges, x0, y0, z0, x1, y1, z1);
    }

    //IDENT
    if (! strcmp(line, "ident")) {
      ident(transform);
    }

    //SCALE
    if (! strcmp(line, "scale")) {
      //Grabs next line of scaling values
      fgets(line, 255, f);

      //Stores scaling factor into variables
      double sx, sy, sz;
      sscanf(line, "%lf %lf %lf", &sx, &sy, &sz);

      
      struct matrix * temp;
      temp = make_scale(sx, sy, sz);
      matrix_mult(temp, transform);
      free_matrix(temp);
    }

    //TRANSLATE
    if (! strcmp(line, "translate")) {
      //Grabs next line of translate values
      fgets(line, 255, f);

      //Stores scaling factor into variables
      double tx, ty, tz;
      sscanf(line, "%lf %lf %lf", &tx, &ty, &tz);

      
      struct matrix * temp;
      temp = make_translate(tx, ty, tz);
      matrix_mult(temp, transform);
      free_matrix(temp);
    }

    
    //ROTATE
    if (! strcmp(line, "rotate")) {
      //Grabs next line of rotate values
      fgets(line, 255, f);

      //Creates temporary matrix 
      struct matrix * temp; 

      //Stores axis and theta values into variables
      char axis[1]; 
      double theta;
      sscanf(line, "%s %lf", &axis, &theta);

      //If rotating about the x axis
      if (! strcmp(axis, "x")) {
	temp = make_rotX(theta);
	matrix_mult(temp, transform);
      }

      //If rotating about the y axis
      if (! strcmp(axis, "y")) {
	temp = make_rotY(theta);
	matrix_mult(temp, transform);
      }

      //If rotating about the z axis
      if (! strcmp(axis, "z")) {
	temp = make_rotZ(theta);
	matrix_mult(temp, transform);
      }

      free(temp);
    }

    //APPLY
    if (! strcmp(line, "apply")) {
      matrix_mult(transform, edges);
    }

    //DISPLAY
    if (! strcmp(line, "display")) {

      //Clears screen 
      clear_screen(s);

      //Sets color
      color c;
      c.red = 240;
      c.blue = 150;
      c.green = 100;

      //Draws lines 
      draw_lines(edges, s, c);

      //Displays image drawn 
      display(s);
    }

    //SAVE
    if (! strcmp(line, "save")) {
      //Grabs name of the file 
      fgets(line, 255, f);

      //Clears screen 
      clear_screen(s);

      //Sets color
      color c;
      c.red = 240;
      c.blue = 150;
      c.green = 100;

      //Draws lines 
      draw_lines(edges, s, c);

      //Saves the drawn image into a file
      save_extension(s, line); 
    }

    //QUIT
    if (! strcmp(line, "quit")) {
      exit(0); 
    }  
      
  }
  
}


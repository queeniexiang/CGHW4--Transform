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

  struct matrix * temp; 
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
    else if (! strcmp(line, "ident")) {
      ident(transform);
    }

    //SCALE
    else if (! strcmp(line, "scale")) {
      //Grabs next line of scaling values
      fgets(line, 255, f);

      //Stores scaling factor into variables
      double sx, sy, sz;
      sscanf(line, "%lf %lf %lf", &sx, &sy, &sz);

      
      temp = make_scale(sx, sy, sz);
      matrix_mult(temp, transform);
      //free_matrix(temp);
    }

    //TRANSLATE
    else if (! strcmp(line, "move")) {
      //Grabs next line of translate values
      fgets(line, 255, f);

      //Stores scaling factor into variables
      double tx, ty, tz;
      sscanf(line, "%lf %lf %lf", &tx, &ty, &tz);

     
      temp = make_translate(tx, ty, tz);
      matrix_mult(temp, transform);
      //free_matrix(temp);
    }

    
    //ROTATE
    else if (! strcmp(line, "rotate")) {
      //printf("Rotating\n"); 
      //printf("Grabbing\n"); 
      //Grabs next line of rotate values
      fgets(line, 255, f);

      //printf("Storing\n"); 
      //Stores axis and theta values into variables
      char axis[1]; 
      double theta;

      sscanf(line, "%s %lf", axis, &theta);
      //printf("axis: %s\n", axis);
      //printf("theta: %lf\n", theta); 

      //If rotating about the x axis
      if (axis[0] == 'x') {
	//printf("x\n"); 
	temp = make_rotX(theta);
      }

      //If rotating about the y axis
      else if (axis[0] == 'y') {
	//printf("y\n"); 
	temp = make_rotY(theta);
      }

      //If rotating about the z axis
      else if (axis[0] == 'z') {
	//printf("z\n"); 
	temp = make_rotZ(theta);
      }
      matrix_mult(temp, transform);
      //free_matrix(temp);
    }

    //APPLY
    else if (! strcmp(line, "apply")) {
      matrix_mult(transform, edges);
    }

    //DISPLAY
    else if (! strcmp(line, "display")) {

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
    else if (! strcmp(line, "save")) {
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
    else if (! strcmp(line, "quit")) {
      free_matrix(temp);
      exit(0); 
    }  
      
  }
  
}


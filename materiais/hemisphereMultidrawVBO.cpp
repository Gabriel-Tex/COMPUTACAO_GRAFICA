//////////////////////////////////////////////////////////////////////////////////////          
// hemisphereMultidrawVBO.cpp
//
// This program builds on hemisphereMultidraw.cpp, storing the vertex and index arrays
// in buffer objects.
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
//#  include <GL/glew.h>
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#pragma comment(lib, "glew64.lib") // Compiler directive to include the GLEW library.
#endif

#define PI              3.14159265
#define RADIUS          5.0       // Radius of hemisphere.
#define LONG_SLICES     4         // Number of longitudinal slices.
#define LAT_SLICES      3         // Number of latitudinal slices.
#define VERTICES        0         // Vertex buffer id.
#define INDICES         1         // Indices buffer id.

using namespace std;

// Globals.
// Angles to rotate hemisphere.
static float        Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

// Vertex array of sample points on the hemisphere.
static float        vertices[3*(LONG_SLICES + 1) * (LAT_SLICES + 1)];

// Array of index arrays.
static unsigned int indices [LAT_SLICES][2*(LONG_SLICES+1)];

// Array of counts (how many elements in each strip)
static int          count0  [LAT_SLICES];

// Arrays of buffer offsets.
void*               offsets [LAT_SLICES];

// Array of buffer ids.
static unsigned int buffer  [2];

// Bézier's Blending Functions
double b_1(double t)
{
    return (1.0 - t - t*t + t*t*t);
}

double b_2(double t)
{
    return (t - 2.0*t*t + t*t*t);
}

double b_3(double t)
{
    return (t*t - t*t*t);
}

double b_4(double t)
{
    return (2.0*t*t - t*t*t);
}

// Fill the vertex array with co-ordinates of the sample points.
// Notice that the Northpole is repeated (LONG_SLICES + 1) times.
void fillVertexArray(void)
{
    int    i, j, k;
    double P_1[3] = {1.0, 0.0, 0.0};
    double P_2[3] = {3.5, 0.5, 0.0};
    double P_3[3] = {0.0, 2.5, 0.0};
    double P_4[3] = {2.0, 3.0, 0.0};
    double t, r, z;
    

   // Vertices are generated along each parallel, starting from the equator and finishing at the north pole, at the crossings with the meridians.
   // K stores the position of each coordinate x_1 y_1 z_1 x_2 y_2 z_2 ...
   // in the unidimensional vertices[Dimension], where
   // Dimension = 3*(LONG_SLICES + 1) * (LAT_SLICES + 1)
   k = 0;
    
   // This loop generates (LAT_SLICES + 1) parallel circles 0..LAT_SLICES
   for (j = 0; j <= LAT_SLICES; j++)
   // when j=LAT_SLICES in the case of a hemisphere, the north pole is a degenerated circle collapsed to a point (the North Pole)
   {
      // t is the parameter in the range from 0 to 1 to mark the points
      // in the profile (meridian or generatix) parametric curve.
      // j = 0         ==> t = 0 (begining of the profile curve)
      // j =LAT_SLICES ==> t = 1 (end of the profile curve)
      t = (1.0/LAT_SLICES)*j;
      
      // The z axis of the revolution surface corresponds to
      // the y axis of the Bézier curve definition.
      z = P_1[1]*b_1(t) + P_2[1]*b_2(t) + P_3[1]*b_3(t) + P_4[1]*b_4(t);
       
      // This loop generates (LONG_SLICES+1) meridians 0..LONG_SLICES
      for (i = 0; i <= LONG_SLICES; i++)
      {
          // i = 0 generates the first meridian
          // i = LONG_SLICES generates the last meridian, which
          // "coincides" with the first meridian.
          // Although this is easy, it is not a good modeling practice.
          
          /* ===  THIS IS THE CODE FOR THE VERTICES OF A HEMISHPERE ===
         vertices[k++] = RADIUS * cos( (PI/2.0/LAT_SLICES)*(float)j ) * cos( (2.0 * PI/LONG_SLICES)*(float)i ); // node id = k/3
         //vertices[k++] = RADIUS * sin( (PI/2.0/LAT_SLICES)*(float)j );
         vertices[k++] = RADIUS * cos( (PI/2.0/LAT_SLICES)*(float)j ) * sin( (2.0 * PI/LONG_SLICES)*(float)i );
         vertices[k++] = RADIUS * sin( (PI/2.0/LAT_SLICES)*(float)j );
         */
          
          // r is the radius of the parallel circle, which is determined
          // by the x coordinate of P(t) (the Bézier profile curve)
          r = P_1[0]*b_1(t) + P_2[0]*b_2(t) + P_3[0]*b_3(t) + P_4[0]*b_4(t);
          
          // Coordinates x, y, z of the vertices defined when a parallel
          // circle crosses a meridian profile curve.
          vertices[k++] = r * cos( (2.0 * PI/LONG_SLICES)*(float)i );
          vertices[k++] = r * sin( (2.0 * PI/LONG_SLICES)*(float)i );
          vertices[k++] = z;
      }
   }
}

// Fill the array of index arrays.
void fillIndices(void)
{
   // This loop will visit each one of the (LAT_SLICES) triangular strips
   // between two successive parallel circles
   for( int j = 0; j < LAT_SLICES; j++)
   {
      // This loop will visit each one of the (LONG_SLICES +1) vertices
      // along a parallel circle where the meridians cross that circle.
      // Although the last meridian coincides with the first meridian,
      // the index of the first vertex and the index of the last vertex
      // are different because the vertices that occupy the same position
      // are distinct.
      for ( int i = 0; i <= LONG_SLICES; i++)
      {
          // Upper parallel of the strip j
          indices[j][2*i]   = (j+1)*(LONG_SLICES + 1) + i;
          
          // Lower parallel of the strip j
          indices[j][2*i+1] =     j*(LONG_SLICES + 1) + i;
      }
   }
}

// Fill the array of counts (stores the number of vertices in each triangle strip).
void fillCount(void)
{
   int j;
   for(j = 0; j < LAT_SLICES; j++)
   {
       count0[j] = 2*(LONG_SLICES + 1);
   }
}

// Fill the array of buffer offsets.
// Since the indices have type "unsigned int" and each strip has 2*(LONG_SLICES +1) indices,
// the offset for the first strip (j = 0) is 0 and of the second strip (j = 1)
// is (2*(LONG_SLICES+1)*j*sizeof(unsigned int))
void fillOffsets(void)
{
   int j;
   for(j = 0; j < LAT_SLICES; j++)
   {
       offsets[j] = (GLvoid*)(2*(LONG_SLICES+1)*j*sizeof(unsigned int));
   }
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glGenBuffers(2, buffer); // Generate two buffer ids.

   glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.

   // Initialize global arrays.
   fillVertexArray  ();
   fillIndices      ();
   fillCount        ();
   fillOffsets      ();

   // Bind and fill vertex buffer and set vertex pointer to the start of the buffer.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Bind and fill indices buffer.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Specify vertex pointer to the start of vertex data.
   glVertexPointer(3, GL_FLOAT, 0, 0);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

   // Command to push the hemisphere, which is drawn centered at the origin, 
   // into the viewing frustum.
   glTranslatef(0.0, 0.0, -10.0);

   // Commands to turn the hemisphere.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Hemisphere properties.
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glColor3f(3.0, 0.0, 0.0);

   // Multidraw command equivalent to the drawing loop for(j = 0; j < LAT_SLICES; j++){...} in hemisphere.cpp.
   glMultiDrawElements(GL_TRIANGLE_STRIP, count0, GL_UNSIGNED_INT, (const void **)offsets, LAT_SLICES);

   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport       (0, 0, w, h);
    
   glMatrixMode     (GL_PROJECTION);
   glLoadIdentity   ();
   glOrtho          (-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    
   glMatrixMode     (GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion   (4, 3);
   glutInitContextProfile   (GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode      (GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize       (500, 500);
   glutInitWindowPosition   (100, 100);
   glutCreateWindow         ("hemisphereMultidrawVBO.cpp");
   glutDisplayFunc          (drawScene);
   glutReshapeFunc          (resize);
   glutKeyboardFunc         (keyInput);
 
   glewExperimental = GL_TRUE;
   glewInit();
   
   setup(); 

   glutMainLoop(); 
}


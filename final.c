/*
 * Mitchell Allen
 * CSCI5229: Computer Graphics
 * Final Project
 * 2022-06-29
 */


// Includes (from examples)
#include "CSCIx229.h"


//  OpenGL with prototypes for glext (from examples)
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#define HEIGHT 600
#define WIDTH 600

unsigned char matrix[HEIGHT][WIDTH][3]; // r,g,b
double offset=0;


// Light values (adapted from examples)
int light     =   1;       // Lighting
int one       =   1;       // Unit value
int distance  =   5;       // Light distance
int smooth    =   1;       // Smooth/Flat shading
int local     =   0;       // Local Viewer Model
int emission  =   0;       // Emission intensity (%)
int ambient   =  30;       // Ambient intensity (%)
int diffuse   = 100;       // Diffuse intensity (%)
int specular  =   0;       // Specular intensity (%)
int shininess =   0;       // Shininess (power of two)
float shiny   =   1;       // Shininess (value)
int zh        =  90;       // Light azimuth
float ylight  = 9.8;       // Elevation of light
unsigned int texture[29];     // Texture names
int obj;          //  Object display list

int num     =  1;     // Ocean polygons
int axes    =  0;     // Display axes
int ntex    =  0;     // Cube faces
int mode    =  1;     // Projection mode
int move    =  1;     // Move light
int th      = -260;   // Azimuth of view angle
int ph      =  25;    // Elevation of view angle
int fov     =  55;    // Field of view (for perspective)
double asp  =  1;     // Aspect ratio
double dim  =  12.1;  // Size of world
int inc     =  10;    // Ball increment

int HasFog = 1;       // Fog toggle
int FogMode = 2;      // Toggle fog mode
int FogDensity = 1;   // Density of fog
int bob = 80;        //Bobbing

//(adapted from examples with suggestions from Paul Hoffman)
// FPS
int Co = 0;
int Cr = 0;

// Eye Position
double Ex = 1;
double Ey = 1;
double Ez = 1;

// Position of Focus
double Fx = 0;
double Fy = 0;
double Fz = 0;

//  Sine and cosine in degrees
#define Sin(th) sin(3.14159265/180*(th))
#define Cos(th) cos(3.14159265/180*(th))
#define Pi(th) (180*th*sin)

const int width = 16*50;
const int height = 9*50;


/*
 *  Convenience routine to output raster text  (from examples)
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection  (from examples)
 */
void Project(double fov,double asp, double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //if(Co)
   //{
   //   gluPerspective(fov,asp,dim/4,4*dim);
   //}
   if(fov)
   {
      gluPerspective(fov,asp,dim/16,16*dim);
   }
   else
   {
      //  Perspective transformation
      if (mode)
      {
         gluPerspective(fov,asp,dim/4,4*dim);
      }
      //  Orthogonal projection
      else
      {
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
      }
   }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}


/* 
 *  Draw sky box (adapted from examples)
 */
static void Sky(double D)
{
   //  Textured white box dimension (-D,+D)
   glPushMatrix();
   glTranslatef(0,1.8,0);
   glScaled(D,D,D);
   glEnable(GL_TEXTURE_2D);
   //glColor3f(1,1,1);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,texture[19]);
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-1,0,-1);
   glTexCoord2f(0.25,0); glVertex3f(+1,0,-1);
   glTexCoord2f(0.25,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.00,1); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.25,0); glVertex3f(+1,0,-1);
   glTexCoord2f(0.50,0); glVertex3f(+1,0,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.25,1); glVertex3f(+1,+1,-1);

   glTexCoord2f(0.50,0); glVertex3f(+1,0,+1);
   glTexCoord2f(0.75,0); glVertex3f(-1,0,+1);
   glTexCoord2f(0.75,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,+1,+1);

   glTexCoord2f(0.75,0); glVertex3f(-1,0,+1);
   glTexCoord2f(1.00,0); glVertex3f(-1,0,-1);
   glTexCoord2f(1.00,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.75,1); glVertex3f(-1,+1,+1);
   glEnd();

   //  Top
   glBindTexture(GL_TEXTURE_2D,texture[14]);
   glColor3f(0.529,0.808,0.980);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.5,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.5,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,1); glVertex3f(-1,+1,-1);
   glEnd();

   // // Bottom
   // glBindTexture(GL_TEXTURE_2D,texture[2]);
   // glColor4f(1.0,1.0,1.0,0.5);
   // glBegin(GL_QUADS);
   // glTexCoord2f(1.0,1); glVertex3f(-1,0,+1);
   // glTexCoord2f(0.5,1); glVertex3f(+1,0,+1);
   // glTexCoord2f(0.5,0); glVertex3f(+1,0,-1);
   // glTexCoord2f(1.0,0); glVertex3f(-1,0,-1);
   // glEnd();

   //  Undo
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

/* 
 *  Draw ocean box (adapted from examples)
 */
static void ocean(double D)
{
   //  Textured white box dimension (-D,+D)
   glPushMatrix();
   glTranslatef(0,1.8,0);
   glScaled(D,D,D);
   glEnable(GL_TEXTURE_2D);
   //glColor3f(1,1,1);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,texture[17]);
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-1,0,-1);
   glTexCoord2f(0.25,0); glVertex3f(+1,0,-1);
   glTexCoord2f(0.25,1); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.00,1); glVertex3f(-1,-1,-1);

   glTexCoord2f(0.25,0); glVertex3f(+1,0,-1);
   glTexCoord2f(0.50,0); glVertex3f(+1,0,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.25,1); glVertex3f(+1,-1,-1);

   glTexCoord2f(0.50,0); glVertex3f(+1,0,+1);
   glTexCoord2f(0.75,0); glVertex3f(-1,0,+1);
   glTexCoord2f(0.75,1); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,-1,+1);

   glTexCoord2f(0.75,0); glVertex3f(-1,0,+1);
   glTexCoord2f(1.00,0); glVertex3f(-1,0,-1);
   glTexCoord2f(1.00,1); glVertex3f(-1,-1,-1);
   glTexCoord2f(0.75,1); glVertex3f(-1,-1,+1);
   glEnd();

   //  Bottom
   glBindTexture(GL_TEXTURE_2D,texture[18]);
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.5,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.5,1); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.0,1); glVertex3f(-1,-1,-1);
   glEnd();

   // // Top
   // glBindTexture(GL_TEXTURE_2D,texture[2]);
   // glColor4f(1.0,1.0,1.0,0.5);
   // glBegin(GL_QUADS);
   // glTexCoord2f(1.0,1); glVertex3f(-1,0,+1);
   // glTexCoord2f(0.5,1); glVertex3f(+1,0,+1);
   // glTexCoord2f(0.5,0); glVertex3f(+1,0,-1);
   // glTexCoord2f(1.0,0); glVertex3f(-1,0,-1);
   // glEnd();

   //  Undo
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


// Draw a slab (adapted from examples)
static void slab(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glColor3f(0.000, 0.392, 0.000);
   //  Palm Leaf
   glBegin(GL_QUADS);
   //  Front
   glNormal3f(0,0,0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01, 0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01, 0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,+0.01, 0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,+0.01, 0.25);
   //  Back
   glNormal3f(0,0,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,+0.01,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,+0.01,-0.25);
   //  Right
   glNormal3f(+0.5,0,0);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,-0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,+0.01,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,+0.01,+0.25);
   //  Left
   glNormal3f(-0.5,0,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,-0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,+0.01,+0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,+0.01,-0.25);
   //  Top
   glNormal3f(0,+0.01,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,+0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,+0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,+0.01,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,+0.01,-0.25);
   //  Bottom
   glNormal3f(0,-0.01,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,+0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,+0.25);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

// Draw a slab (adapted from examples)
static void seaweed(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   //glBindTexture(GL_TEXTURE_2D,texture[3]);
   glColor3f(0.000, 0.392, 0.000);
   //  Seaweed
   glBegin(GL_QUADS);
   //  Front
   glNormal3f(0,0,0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01, 0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01, 0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01, 0.25+(sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01, 0.25+(sin(bob)/4));

   glNormal3f(0,0.02,0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01, 0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01, 0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03, 0.25+(cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03, 0.25+(cos(bob)/4));
   
   glNormal3f(0,0.04,0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03, 0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03, 0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05, 0.25+(-sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05, 0.25+(-sin(bob)/4));
   
   glNormal3f(0,0.06,0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05, 0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05, 0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07, 0.25+(-cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07, 0.25+(-cos(bob)/4));

   //  Back
   glNormal3f(0,0,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));

   glNormal3f(0,0.02,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));

   glNormal3f(0,0.04,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));

   glNormal3f(0,0.06,-0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));

   //  Right
   glNormal3f(+0.5,0,0);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5,-0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,+0.25+(sin(bob)/4));

   glNormal3f(+0.5,0.02,0);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,+0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,+0.25+(cos(bob)/4));

   glNormal3f(+0.5,0.04,0);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,+0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,+0.25+(-sin(bob)/4));

   glNormal3f(+0.5,0.06,0);
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,+0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07,+0.25+(-cos(bob)/4));

   //  Left
   glNormal3f(-0.5,0,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5,-0.01,+0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,+0.25+(sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));

   glNormal3f(-0.5,0.02,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,-0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(sin(bob)/4),+0.01,+0.25+(sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,+0.25+(cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));

   glNormal3f(-0.5,0.04,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,-0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(cos(bob)/4),+0.03,+0.25+(cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,+0.25+(-sin(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));

   glNormal3f(-0.5,0.06,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,-0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-sin(bob)/4),+0.05,+0.25+(-sin(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07,+0.25+(-cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));

   //  Top
   glNormal3f(0,+0.07,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07,+0.25+(-cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07,+0.25+(-cos(bob)/4));
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5+(-cos(bob)/4),+0.07,-0.25+(-cos(bob)/4));

   //  Bottom
   glNormal3f(0,-0.01,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,-0.25);
   glTexCoord2f(1.0,0.0); glVertex3f(+0.5,-0.01,+0.25);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.01,+0.25);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw a cylinder (adapted from Paul Hoffman)
 *     at (x,y,z)
 *     size (radius, height)
 *     rotated th about the y axis
 */
void palmtrunk(double x,double y,double z,
              double r,double h, 
              double rx, double ry, double rz)
{
   int inc=5;
   int th;

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rx,1,0,0);
   glRotated(ry,0,1,0);
   glRotated(rz,0,0,1);
   glScaled(r,h,r);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   //glBindTexture(GL_TEXTURE_2D,texture[4]);

   glColor3f(1,1,1);
   // Upper Part
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5); 
   glVertex3f(0,1,0);
   glNormal3f(0,1,0);
   for (th=0;th<=360;th+=2*inc)
   {
      glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
      glVertex3d(Sin(th),1,Cos(th));
   }
   glEnd();

   // Side Part
   glBegin(GL_QUAD_STRIP); 
   for (th=0;th<=360;th+=inc)
   {
      double u = th/360.0;
      glNormal3f(Cos(th),0,Sin(th));
      glTexCoord2d(4*u,1.0); glVertex3d(Cos(th),1,Sin(th));
      glTexCoord2d(4*u,0.0); glVertex3d(Cos(th),0,Sin(th));
    }
   glEnd();

   // Base Part
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5); 
   glVertex3f(0,0,0);
   glNormal3f(0,-1,0);
   for (th=0;th<=360;th+=2*inc)
   {
      glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
      glVertex3d(Sin(th),0,Cos(th));
   }
   glEnd();

   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw a cylinder (adapted from Paul Hoffman)
 *     at (x,y,z)
 *     size (radius, height)
 *     rotated th about the y axis
 */
void glass(double x,double y,double z,
              double r,double h, 
              double rx, double ry, double rz)
{
   int inc=5;
   int th;

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rx,1,0,0);
   glRotated(ry,0,1,0);
   glRotated(rz,0,0,1);
   glScaled(r,h,r);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[28]);

   glColor3f(1,1,1);
   // Upper Part
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5); 
   glVertex3f(0,1,0);
   glNormal3f(0,1,0);
   for (th=0;th<=360;th+=2*inc)
   {
      glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
      glVertex3d(Sin(th),1,Cos(th));
   }
   glEnd();

   // Side Part
   glBegin(GL_QUAD_STRIP); 
   for (th=0;th<=360;th+=inc)
   {
      double u = th/360.0;
      glNormal3f(Cos(th),0,Sin(th));
      glTexCoord2d(4*u,1.0); glVertex3d(Cos(th),1,Sin(th));
      glTexCoord2d(4*u,0.0); glVertex3d(Cos(th),0,Sin(th));
    }
   glEnd();

   // Base Part
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5); 
   glVertex3f(0,0,0);
   glNormal3f(0,-1,0);
   for (th=0;th<=360;th+=2*inc)
   {
      glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
      glVertex3d(Sin(th),0,Cos(th));
   }
   glEnd();

   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


static void Vertex(double th,double ph) //(adapted from examples)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}


static void moon(double x,double y,double z,double r) // adapted from examples
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);

   // Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[11]);

   //  Sphere
   //glColor3f(1, 1, 1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

static void beachball(double x,double y,double z,double r) // adapted from examples
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);

   // Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[15]);

   //  Sphere
   //glColor3f(1, 1, 1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


static void sphere(double x,double y,double z,double r) // adapted from examples
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);

   // Set texture
   glEnable(GL_TEXTURE_2D);
   //glBindTexture(GL_TEXTURE_2D,texture[15]);

   //  Sphere
   //glColor3f(1, 1, 1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


//(from ex13)
typedef struct {float x,y,z;} vtx;
typedef struct {int A,B,C;} tri;
#define n 500
vtx is[n];

/*
 *  Icosahedron data (from ex13)
 */
const int N=20;
//  Triangle index list
const tri idx[] =
   {
      { 2, 1, 0}, { 3, 2, 0}, { 4, 3, 0}, { 5, 4, 0}, { 1, 5, 0},
      {11, 6, 7}, {11, 7, 8}, {11, 8, 9}, {11, 9,10}, {11,10, 6},
      { 1, 2, 6}, { 2, 3, 7}, { 3, 4, 8}, { 4, 5, 9}, { 5, 1,10},
      { 2, 7, 6}, { 3, 8, 7}, { 4, 9, 8}, { 5,10, 9}, { 1, 6,10}
   };
//  Vertex coordinates
const vtx xyz[] =
   {
      { 0.000, 0.000, 1.000}, { 0.894, 0.000, 0.447}, { 0.276, 0.851, 0.447},
      {-0.724, 0.526, 0.447}, {-0.724,-0.526, 0.447}, { 0.276,-0.851, 0.447},
      { 0.724, 0.526,-0.447}, {-0.276, 0.851,-0.447}, {-0.894, 0.000,-0.447},
      {-0.276,-0.851,-0.447}, { 0.724,-0.526,-0.447}, { 0.000, 0.000,-1.000}
   };

/*
 *  Draw sphere approximated by the inscribed icosahedron (adapted from ex13)
 *     at (x,y,z)
 *     size  s
 */
static void icosasphere(float x,float y,float z,float s)
{
   //  Set transforms
   glPushMatrix();
   glTranslatef(x,y,z);
   glScalef(s,s,s);
   glRotatef(-90,1,0,0);
   //  Since this is a sphere the vertex and normal values are the same
   glVertexPointer(3,GL_FLOAT,0,xyz);
   glEnableClientState(GL_VERTEX_ARRAY);
   glNormalPointer(GL_FLOAT,0,xyz);
   glEnableClientState(GL_NORMAL_ARRAY);
   //  Draw icosahedron (3*N is number of vertices)
   glDrawElements(GL_TRIANGLES,3*N,GL_UNSIGNED_INT,idx);
   //  Reset state
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glPopMatrix();
}


// Hemisphere (adapted from web and examples)
static void hemisphere(double x,double y,double z,double r,double rx,double ry,double rz)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glRotated(rx,1,0,0);
   glRotated(ry,0,1,0);
   glRotated(rz,0,0,1);

   // Set texture
   glEnable(GL_TEXTURE_2D);
   //glBindTexture(GL_TEXTURE_2D,texture[8]);

   //  Sphere
   //glColor3f(1, 1, 1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   glTexCoord2f(r,r);

   //    // Upper Part
   // glBegin(GL_TRIANGLE_FAN);
   // glTexCoord2f(0.5,0.5); 
   // glVertex3f(0,1,0);
   // glNormal3f(0,1,0);
   // for (th=0;th<=360;th+=2*inc)
   // {
   //    glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
   //    glVertex3d(Sin(th),1,Cos(th));
   // }
   // glEnd();

   // // Side Part
   // glBegin(GL_QUAD_STRIP); 
   // for (th=0;th<=360;th+=inc)
   // {
   //    double u = th/360.0;
   //    glNormal3f(Cos(th),0,Sin(th));
   //    glTexCoord2d(4*u,1.0); glVertex3d(Cos(th),1,Sin(th));
   //    glTexCoord2d(4*u,0.0); glVertex3d(Cos(th),0,Sin(th));
   //  }
   // glEnd();

   // // Base Part
   // glBegin(GL_TRIANGLE_FAN);
   // glTexCoord2f(0.5,0.5); 
   // glVertex3f(0,0,0);
   // glNormal3f(0,-1,0);
   // for (th=0;th<=360;th+=2*inc)
   // {
   //    glTexCoord2f(2*Sin(th)+0.5,2*Cos(th)+0.5);
   //    glVertex3d(Sin(th),0,Cos(th));
   // }
   // glEnd();
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=180;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


/*
 *  Draw a cone with texture (adapted from Paul Hoffman)
 *     at (x,y,z)
 *     size (r(radius), h(height))
 *     rotate (rx, ry, rz)
 */
void cone(double x,double y,double z,
          double r,double h, 
          double rx, double ry, double rz,int temp)
{

   int DEF_D = 5;
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   // float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rx,1,0,0);
   glRotated(ry,0,1,0);
   glRotated(rz,0,0,1);
   glScaled(r,h,r);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1.0, 1.0, 1.0); //Set color to white
   //glBindTexture(GL_TEXTURE_2D,texture[temp]);
    /* sides */
    glBegin(GL_TRIANGLES);
    
    for (int k=0;k<=360;k+=DEF_D){
      glVertex3f(0,0,2);
      // glTexCoord2f(0,0);
      glTexCoord2f(2*Sin(k)+0.5,2*Cos(k)+0.5);
      glVertex3f(Cos(k),Sin(k),0);
      glTexCoord2f(2*Sin(k)+0.5,2*Cos(k)+0.5);
      glNormal3f(Sin(k+DEF_D)-Sin(k),Cos(k)-Cos(k+DEF_D),Sin(DEF_D));
      // glTexCoord2f(0,1);
      glTexCoord2f(2*Sin(k)+0.5,2*Cos(k)+0.5);
      glVertex3f(Cos(k+DEF_D),Sin(k+DEF_D),0);
    }
    glEnd();

    /* bottom circle */ 
    /* rotate back */
    glRotated(90,1,0,0);
    glBegin(GL_TRIANGLES);
    glNormal3f(0,-1,0);
    for (int k=0;k<=360;k+=DEF_D) {
      // glColor3f(1.0,0.0,0.0);
      glVertex3f(0,0,0);
      // glColor3f(1.0,0.0,1.0);
      glVertex3f(Cos(k),0,Sin(k));
      glTexCoord2f(2*Sin(th)+0.5,th/36); 
      // glColor3f(1.0,1.0,0.0);
      glVertex3f(Cos(k+DEF_D),0,Sin(k+DEF_D));
    }
    glEnd();
   
   glDisable(GL_TEXTURE_2D);
   //  Undo transformations and textures
   glPopMatrix();
}


// /*
//  *  Draw IceBerg2
//  *     at (x,y,z)
//  *     Scaled at (dx,dy,dz)
//  *     Rotated along y-axis (th)
//  */
// static void IceBerg2(double x,double y,double z,double dx,double dy,double dz,double rx, double ry, double rz)
// {
//  //  Save transformation
//    glPushMatrix();
//    //  Offset, scale and rotate
//    glTranslated(x,y,z);
//    glRotated(rx,1,0,0);
//    glRotated(ry,0,1,0);
//    glRotated(rz,0,0,1);
//    glScaled(dx,dy,dz);

//    //Texture
//    glEnable(GL_TEXTURE_2D);
//    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
//    glBindTexture(GL_TEXTURE_2D,texture[22]);
//    glColor3f(1,1,1);


//    glBegin(GL_POLYGON);
//    //  Top polygon
//    glNormal3f(-0.04,-0.08,-0.02);
//    glTexCoord2f(0,0); glVertex3f(-0.2,0.2,0.3); //1
//    glTexCoord2f(0,1); glVertex3f(0.2,0,0.3); //2
//    glTexCoord2f(1,1); glVertex3f(0.3,0,0.1);     //3
//    glTexCoord2f(1,0); glVertex3f(0.4,0,0); //4
//    glTexCoord2f(1,1); glVertex3f(0.4,0.2,0); //5
//    glTexCoord2f(0,0); glVertex3f(0.2,0.2,0.1); //6
//    glEnd();
//    // bottom
//    glBegin(GL_QUADS);
//    glNormal3f(0.0,0.0,0.16);
//    glTexCoord2f(0,0); glVertex3f(-0.4,0.2,0); //1
//    glTexCoord2f(0,1); glVertex3f(0.4,0.2,0); //2
//    glTexCoord2f(1,1); glVertex3f(0.4,0,0);     //3
//    glTexCoord2f(1,0); glVertex3f(-0.4,0,0); //4
//    glEnd();


//    //Side 1
//    // glBegin(GL_POLYGON);
//    glBegin(GL_QUADS);
//    glNormal3f(0.0,0.0,1);
//    // glVertex3f(-0.2,0.2,0.3);
//    glTexCoord2f(0,0); glVertex3f(-0.4,0,0);
//    glTexCoord2f(0,1); glVertex3f(0.4,0,0);
//    glTexCoord2f(1,1); glVertex3f(0.3,0,0.1);
//    glTexCoord2f(1,0); glVertex3f(0.2,0,0.3);
//    // glEnd();

//    //Side 2
//    // glBegin(GL_QUADS);
//       glNormal3f(0.0,0.08,0);
//    glTexCoord2f(0,0); glVertex3f(-0.2,0.2,0.3);
//    glTexCoord2f(0,1); glVertex3f(0.2,0,0.3);
//    glTexCoord2f(1,1); glVertex3f(-0.4,0,0);
//    glTexCoord2f(1,0); glVertex3f(-0.4,0.2,0);

//    //Side 3
//       glNormal3f(0,0.22,0);
//    glTexCoord2f(0,0); glVertex3f(-0.2,0.2,0.3);
//    glTexCoord2f(0,1); glVertex3f(-0.4,0.2,0);
//    glTexCoord2f(1,1); glVertex3f(0.4,0.2,0);
//    glTexCoord2f(1,0); glVertex3f(0.2,0.2,0.1);
//    glEnd();

//       //  Undo transofrmations
//     //  Switch off textures so it doesn't apply to the rest
//    //glDisable(GL_TEXTURE_2D);
//    glPopMatrix();

// }


/*
 *  Draw a ball (from examples)
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  White ball with yellow specular
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   //glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);


   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[9]);


   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene // adapted from examples with suggestions from Paul Hoffman
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Enable opacity
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //  Undo previous transformations
   glLoadIdentity();
   
   if (mode)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);


// Murky water
if (HasFog)
{
   GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };  //Fog type storage
   GLfloat fogColor[4]= {0.3, 0.6, 0.6, 1.0};         //Fog color
   glClearColor(0.3,0.6,0.6,1.0);                     //We'll clear to the color of the fog (Modified)
   glFogi(GL_FOG_MODE, fogMode[FogMode]);             //Fog mode
   glFogfv(GL_FOG_COLOR, fogColor);                   //Set Fog Color
   glFogf(GL_FOG_DENSITY, FogDensity);                //How dense the fog will be
   glHint(GL_FOG_HINT, GL_DONT_CARE);                 //Fog hint value
   glFogf(GL_FOG_START, 5.0);                         //Fog start depth (50.0)
   glFogf(GL_FOG_END, 65.0);                          //Fog end depth (400.0)
   glEnable(GL_FOG);                                  //Enables GL_FOG
}
else
   glDisable(GL_FOG);

 //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      float Position2[]  = {((distance*Cos(zh))/22)-4,5.5,((distance*Sin(zh))/22)+9,1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 1);
      //Ligthouse bulb
      ball(Position2[0],Position2[1],Position2[2],0.05);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //  Enable light 1
      glEnable(GL_LIGHT1);
      //  Set ambient, diffuse, specular components and position of light 1
      glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT1,GL_POSITION,Position2);
   }
   else
      glDisable(GL_LIGHTING);

   float BobbingA[]  = {((sin(bob)/8)+0.1),1.0};
   float BobbingB[]  = {((-sin(bob)/8)+0.1),1.0};
   float BobbingC[]  = {((cos(bob)/8)+0.1),1.0};
   float BobbingD[]  = {((-cos(bob)/8)+0.1),1.0};

   // Draw scene

   //  Draw sky
   Sky(3.5*dim);

   // Draw ocean
   ocean(3.5*dim);

   // Island 1
   // Sandy beach
   glColor3f(0.804,0.522,0.247);
   glBindTexture(GL_TEXTURE_2D,texture[8]);
   hemisphere(0, 2, 0, 2, 0, 0, 90);
   glDisable(GL_TEXTURE_2D);
   // glColor3f(0.804,0.522,0.247);
   // glBindTexture(GL_TEXTURE_2D,texture[8]);
   // palmtrunk(0,1.9,0,2.1,0,0,0,0);
   // glDisable(GL_TEXTURE_2D);

   // Palm trunk
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(0,3,0,0.125,2.2,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Palm leaves
   //glColor3f(0.000, 0.392, 0.000);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   slab(0,5.25,0,1,3,1,0);
   slab(0,5.25,0,1,3,1,90);
   glDisable(GL_TEXTURE_2D);

   // Coconut
   glColor3f(0.295, 0.131, 0.035);
   //glBindTexture(GL_TEXTURE_2D,texture[0]);
   icosasphere(0.85,3.65,0.85,0.1);

   // Coconut - floating
   glColor3f(0.295, 0.131, 0.035);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   sphere(-3,(BobbingC[0]/2)+1.9,3,0.1);

   // Coconut - floating2
   glColor3f(0.295, 0.131, 0.035);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   sphere(9.7,(BobbingD[0]/2)+1.9,4.5,0.1);

   // Coconut - floating3
   glColor3f(0.295, 0.131, 0.035);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   sphere(3,(BobbingA[0]/2)+1.9,-4,0.1);

   // Coconut - floating4
   glColor3f(0.295, 0.131, 0.035);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   sphere(-6,(BobbingB[0]/2)+1.9,-4.5,0.1);

   // Trunk - floating
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(-9,(BobbingD[0]/2)+1.9,2,0.125,0.6,0,0,90);
   glDisable(GL_TEXTURE_2D);

   // Trunk - floating2
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(9,(BobbingA[0]/2)+1.9,-3.5,0.125,0.6,0,37,90);
   glDisable(GL_TEXTURE_2D);

   // Trunk - floating3
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(17,(BobbingB[0]/2)+1.9,-14,0.125,0.6,0,290,90);
   glDisable(GL_TEXTURE_2D);

   // Fish object
   //glCallList(obj);

   // Island 2
   // Sandy beach
   glColor3f(0.804,0.522,0.247);
   glBindTexture(GL_TEXTURE_2D,texture[8]);
   hemisphere(4, 2, 4, 2, 0, 0, 90);
   //glDisable(GL_TEXTURE_2D);

   // Palm trunk
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(4,3,4,0.125,2.2,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Palm leaves
   //glColor3f(0.000, 0.392, 0.000);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   slab(4,5.25,4,1,3,1,0);
   slab(4,5.25,4,1,3,1,90);
   glDisable(GL_TEXTURE_2D);

   // Coconut
   glColor3f(0.295, 0.131, 0.035);
   //glBindTexture(GL_TEXTURE_2D,texture[0]);
   icosasphere(4.85,3.65,4.85,0.1);


   // Island 3
   // Rocky beach
   glColor3f(0.4,0.4,0.4);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   hemisphere(-4, 2, 9, 2, 0, 0, 90);
   //glDisable(GL_TEXTURE_2D);

   // Lighthouse
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[16]);
   palmtrunk(-4,3,9,0.425,2.2,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Lighthouse platform
   glColor3f(0, 0, 0);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   palmtrunk(-4,5.2,9,0.525,0.05,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Lighthouse railing
   glColor3f(0,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   palmtrunk(-4.45,5.25,9,0.05,0.5,0,0,0);
   glDisable(GL_TEXTURE_2D);

   glColor3f(0,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   palmtrunk(-3.55,5.25,9,0.05,0.5,0,0,0);
   glDisable(GL_TEXTURE_2D);

   glColor3f(0,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   palmtrunk(-4,5.25,9.45,0.05,0.5,0,0,0);
   glDisable(GL_TEXTURE_2D);

   glColor3f(0,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   palmtrunk(-4,5.25,8.55,0.05,0.5,0,0,0);
   glDisable(GL_TEXTURE_2D);


   // Iceberg
   //IceBerg2(5,2,-5,10,10,10,90,180,90);

   // Lighthouse roof
   glColor3f(0.863,0.078,0.235);
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   cone(-4,5.7,9,0.525,0.525,270,0,0,0);
   glDisable(GL_TEXTURE_2D);


   // Island 4
   // Rocky beach
   glColor3f(0.4,0.4,0.4);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   hemisphere(-3, 2, -7, 2, 0, 0, 90);
   glDisable(GL_TEXTURE_2D);

   // Palm trunk
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(-3,3,-7,0.125,2.2,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Palm leaves
   //glColor3f(0.000, 0.392, 0.000);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   slab(-3,5.25,-7,1,3,1,0);
   slab(-3,5.25,-7,1,3,1,90);
   glDisable(GL_TEXTURE_2D);

   
   // Island 5
   // Rocky beach
   glColor3f(0.4,0.4,0.4);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   hemisphere(27, 2, 29, 2, 0, 0, 90);
   glDisable(GL_TEXTURE_2D);

   // Palm trunk
   glColor3f(0.545, 0.271, 0.075);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   palmtrunk(27,3,29,0.125,2.2,0,0,0);
   glDisable(GL_TEXTURE_2D);

   // Palm leaves
   //glColor3f(0.000, 0.392, 0.000);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   slab(27,5.25,29,1,3,1,0);
   slab(27,5.25,29,1,3,1,90);
   glDisable(GL_TEXTURE_2D);
   
   // Flag - waving
   //glBindTexture(GL_TEXTURE_2D,texture[26]);
   //seaweed(BobbingC[0]-3,10,BobbingC[0]-7,0.1,2,3,0);


   // Flag
   // glColor3f(0.545, 0.271, 0.075);
   // glBindTexture(GL_TEXTURE_2D,texture[16]);
   // flag(-4,3,9,0.425,2.2,0,0,0);
   // glDisable(GL_TEXTURE_2D);

   // Palm leaves
   //glColor3f(0.000, 0.392, 0.000);
   //glBindTexture(GL_TEXTURE_2D,texture[3]);
   //slab(-4,3.25,-4,1,3,1,0);
   //slab(-4,3.25,-4,1,3,1,90);
   //glDisable(GL_TEXTURE_2D);

   // Coconut
   glColor3f(0.295, 0.131, 0.035);
   //glBindTexture(GL_TEXTURE_2D,texture[0]);
   icosasphere(-0.85,3.65,-0.85,0.1);


   // Moon
   //glColor3f(0.855, 0.647, 0.125);
   glColor3f(0.5, 0.7, 0.5);
   moon(2,10,2,1);

   // Beach ball 1
   beachball(1,BobbingA[0]+2,4,0.4);

   // Beach ball 2
   beachball(-7,BobbingB[0]+2,5,0.25);

   // Beach ball 3
   beachball(7,BobbingC[0]+2,3,0.2);

   // Beach ball 4
   beachball(-18,BobbingD[0]+2,-4,0.2);

   // Beach ball 5
   beachball(-4,BobbingC[0]+2,16,0.27);

   // Beach ball 6
   beachball(9,BobbingB[0]+2,-16,0.2);

   // Beach ball 7
   beachball(14,BobbingD[0]+2,6,0.34);

   // Beach ball 8
   beachball(0,BobbingB[0]+2,-5,0.1);

   // Beach ball 9
   beachball(3.9,BobbingC[0]+2,-3,0.8);

   // Beach ball 10
   beachball(-24,BobbingB[0]+2,-7,0.2);


   // Seaweed
   //glColor3f(0.000, 0.392, 0.000);
   glBindTexture(GL_TEXTURE_2D,texture[21]);
   seaweed(BobbingC[0]-1,-30,BobbingC[0]+1,0.1,410,1,0);
   seaweed(BobbingB[0]-3,-30,BobbingD[0]+12,0.1,400,1,90);
   seaweed(BobbingD[0]+2,-30,BobbingA[0],1,380,0.1,200);
   seaweed(BobbingA[0]-5.1,-30,BobbingA[0]+0.4,0.7,320,0.1,290);
   seaweed(BobbingD[0]+3.5,-30,BobbingB[0]-8,0.9,350,0.11,70);
   seaweed(BobbingB[0]+9.5,-30,BobbingA[0]+3.3,1,386,0.1,20);
   seaweed(BobbingC[0]-7.7,-30,BobbingC[0]+1.7,0.1,391,0.8,110);
   seaweed(BobbingD[0]-11.2,-30,BobbingC[0]+2.7,1,430,0.1,330);
   seaweed(BobbingC[0]-21,-30,BobbingC[0]+1,0.1,410,1,0);
   seaweed(BobbingB[0]-3,-30,BobbingD[0]+22,0.1,400,1,90);
   seaweed(BobbingD[0]+22,-30,BobbingA[0],1,380,0.1,200);
   seaweed(BobbingA[0]-5.1,-30,BobbingA[0]+2.4,0.7,320,0.1,290);
   seaweed(BobbingD[0]+23.5,-30,BobbingB[0]-8,0.9,350,0.11,70);
   seaweed(BobbingB[0]+29.5,-30,BobbingA[0]+23.3,1,386,0.1,20);
   seaweed(BobbingC[0]-27.7,-30,BobbingC[0]+1.7,0.1,391,0.8,110);
   seaweed(BobbingD[0]-11.2,-30,BobbingC[0]+22.7,1,430,0.1,330);
   seaweed(BobbingC[0]-21,-30,BobbingC[0]+21,0.14,410,1,0);
   seaweed(BobbingB[0]-23,-30,BobbingD[0]+22,0.04,400,1,90);
   seaweed(BobbingD[0]+22,-30,BobbingA[0]+7.8,0.07,380,0.7,200);
   seaweed(BobbingA[0]-25.1,-30,BobbingA[0]+2.4,0.7,320,0.1,290);
   seaweed(BobbingD[0]+23.5,-30,BobbingB[0]-28,0.9,350,0.11,70);
   seaweed(BobbingB[0]+19.5,-30,BobbingA[0]+28.3,1,386,0.1,20);
   seaweed(BobbingC[0]-17.7,-30,BobbingC[0]+6.7,0.03,391,0.8,110);
   seaweed(BobbingD[0]-21.2,-30,BobbingC[0]+27.7,1,430,0.1,330);
   glDisable(GL_TEXTURE_2D);


   // Lighthouse casing
   glColor4f(0.2, 0.2, 0.2, 0.2);
   //glBindTexture(GL_TEXTURE_2D,texture[28]);
   glass(-4,5.25,9,0.3,0.5,0,0,0);
   //glDisable(GL_TEXTURE_2D);

   // Draw water surface
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   water();
   glDisable(GL_TEXTURE_3D);


   //glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perspective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when the window is resized (from examples)
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);

   // Buoyancy
   //double waves = glutGet(GLUT_ELAPSED_TIME)/100.0;
   bob = fmod(3*t,360.0);

   // Movement (adapted from Paul Hoffman)
   // float step = GLUT_ELAPSED_TIME/1000;
   // IceBerg(0+step*0.01,0,0.3+step*0.01,2.2,2.2,2.2,90,180,90);
   
    
   // static float count = 0.0;

   //  int i,j;
   //  count += 0.05;
   //  offset-=0.14;

   //  for(i=0;i<200;i++)
   //      for(j=0;j<WIDTH;j++)
   //      {
   //          matrix[i][j][0] = 0; // red
   //          matrix[i][j][1] = 200 + 55 * sin(count + j * 0.02); // green
   //          matrix[i][j][2] = 0; // blue
   //      }

   //  for(i=200;i<400;i++)
   //      for(j=0;j<WIDTH;j++)
   //      {
   //          matrix[i][j][0] = 200 + 55 * sin(count + j * 0.02); // red
   //          matrix[i][j][1] = 200 + 55 * sin(count + j * 0.02); // green
   //          matrix[i][j][2] = 200 + 55 * sin(count + j * 0.02); // blue
   //      }

   //  for(i=400;i<600;i++)
   //      for(j=0;j<WIDTH;j++)
   //      {
   //          matrix[i][j][0] = 0; // red
   //          matrix[i][j][1] = 0; // green
   //          matrix[i][j][2] = 200 + 55 * sin(count + j * 0.02); // blue
   //      }

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when an arrow key is pressed (adapted from examples)
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   //else if (key == GLUT_KEY_F8)
   //   inc = (inc==10)?3:10;
   //  Flip sign
   //else if (key == GLUT_KEY_F9)
   //   one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed (adapted from examples with suggestions from Paul Hoffman)
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset camera
   else if (ch == '0')
      {
      th =-280;      
      ph = 25;        
      fov= 43;       
      dim= 10.0;     
      Ex = 0;
      Ey = 1;
      Ez = 15;
      Fx = 0;
      Fy = 0;
      Fz = 0;
      Cr = 0;
      }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Change first-person camera field of view angle
   else if (ch == 'i' || ch == 'I')
      fov--;
   else if (ch == 'u' || ch == 'U')
      fov++;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '5')
      zh += 1;
   else if (ch == '6')
      zh -= 1;
   //  Change number of quadrangles
   else if (ch == '3' && num>1)
      num--;
   else if (ch == '4' && num<100)
      num++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='b' && ambient>0)
      ambient -= 5;
   else if (ch=='B' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='v' && diffuse>0)
      diffuse -= 5;
   else if (ch=='V' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='n' && specular>0)
      specular -= 5;
   else if (ch=='N' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='y' && emission>0)
      emission -= 5;
   else if (ch=='Y' && emission<100)
      emission += 5;
   //  Shininess level
   //else if (ch=='h' && shininess>-1)
   //   shininess -= 1;
   //else if (ch=='H' && shininess<7)
   //   shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   //shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Toggle first-person camera
   else if (ch == '1' || ch == '1')
   {
     Co = 1-Co;
   }
   //  First person camera is On
   else if (Co)
   {
     double c = 0.2;
     //First person camera moves left
     //if (ch == 'a' || ch == 'A')
     //{
     //  Ex -= Cos(th);
     //  Ez -= Sin(th);
     //}
     //First person camera moves right
     //else if (ch == 'd' || ch == 'D')
     //{
     //  Ex += Cos(th);
     //  Ez += Sin(th);
     //}
     //First person camera moves forward
     //else if (ch == 'w' || ch == 'W')
     {
       Ex += c*Fx;
       Ez += c*Fz;
     }
     //First person camera moves back
     if (ch == 's' || ch == 'S')
     {
       Ex -= c*Fx;
       Ez -= c*Fz;
     }
     //First person camera moves up
     else if (ch == 'r' || ch == 'R')
     {
       Ey += c*Fy;
     }
     //First person camera moves down
     else if (ch == 'f' || ch == 'F')
     {
       Ey -= c*Fy;
     }
     //Camera turns left
     else if (ch == 'q' || ch == 'Q')
       Cr -= 2.5;
     //Camera turns right
     else if (ch == 'e' || ch == 'E')
       Cr += 2.5;
     //Camera turns up
     //else if (ch == 'z' || ch == 'Z')
      // Cr -= 4;
     //Camera turns down
     //else if (ch == 'c' || ch == 'C')
       //Cr += 4;
     //  Keep angles to +/-360 degrees
       Cr %= 360;
   }
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   //glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized  (from examples)
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}


/*
 *  Start up GLUT and tell it what to do  (modified from examples)
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);

   //  Request double buffered, true color window with Z buffering at 1000x750
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1000,750);
   glutCreateWindow("Final_Allen_Mitchell");

   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   //  Load texture
   texture[0] = LoadTexBMP("textures/coconut1.bmp");
   texture[1] = LoadTexBMP("textures/ocean1.bmp");
   texture[2] = LoadTexBMP("textures/ocean2.bmp");
   texture[3] = LoadTexBMP("textures/palmleaf1.bmp");
   texture[4] = LoadTexBMP("textures/palmtrunk1.bmp");
   texture[5] = LoadTexBMP("textures/rock1.bmp");
   texture[6] = LoadTexBMP("textures/sand1.bmp");
   texture[7] = LoadTexBMP("textures/sand2.bmp");
   texture[8] = LoadTexBMP("textures/sand3.bmp");
   texture[9] = LoadTexBMP("textures/sun1.bmp");
   texture[10] = LoadTexBMP("textures/sun2.bmp");
   texture[11] = LoadTexBMP("textures/moon1.bmp");
   texture[12] = LoadTexBMP("textures/moon2.bmp");
   texture[13] = LoadTexBMP("textures/moon3.bmp");
   texture[14] = LoadTexBMP("textures/sky1.bmp");
   texture[15] = LoadTexBMP("textures/beachball1.bmp");
   texture[16] = LoadTexBMP("textures/lighthouse1.bmp");
   texture[17] = LoadTexBMP("textures/underwaterside1.bmp");
   texture[18] = LoadTexBMP("textures/seafloor1.bmp");
   texture[19] = LoadTexBMP("textures/skyside1.bmp");
   texture[20] = LoadTexBMP("textures/seaweed1.bmp");
   texture[21] = LoadTexBMP("textures/seaweed2.bmp");
   texture[22] = LoadTexBMP("textures/seaweed3.bmp");
   texture[23] = LoadTexBMP("textures/seaweed4.bmp");
   texture[24] = LoadTexBMP("textures/coloflag1.bmp");
   texture[25] = LoadTexBMP("textures/coloflag2.bmp");
   texture[26] = LoadTexBMP("textures/coloflag3.bmp");
   texture[27] = LoadTexBMP("textures/blackroof.bmp");
   texture[28] = LoadTexBMP("textures/shine1.bmp");

   // Load objects
   //obj = LoadOBJ(argv[1]);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}

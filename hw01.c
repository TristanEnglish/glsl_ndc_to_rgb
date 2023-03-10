/*
 *  Homework 01
 *  NDC to RGB Shader
 *
 *  Takes points in NDC and converts to RGB colors
 * 
 *  Key bindings
 *  z/Z        Move cube in z direction
 *  y/Y        Move cube in y direction
 *  x/X        Move cube in x direction
 *  m          Toggle shader
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx239.h"
int mode=0;    //  Shader
int th=45,ph=30; //  View angles
int fov=57;    //  Field of view (for perspective)
int tex=0;     //  Texture
int obj=0;     //  Object
int shader=0;  //  Shader
float asp=1;   //  Aspect ratio
float dim=4;   //  Size of world
float x = 0;   // x location
float y = 0;   // y location
float z = 0;   // z location
const char* text[] = {"Fixed Pipeline","NDC to RGB"};

//
//  Refresh display
//
void display(GLFWwindow* window)
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set view
   View(th,ph,fov,dim);

   //  Enable shader
   if (mode)
   {
     glUseProgram(shader);
     int id = glGetUniformLocation(shader,"time");
     glUniform1f(id,glfwGetTime());
   }
   else
     glUseProgram(0);
   //  Draw scene
   glPushMatrix();
   glTranslated(x,y,z);
   SolidCube();
   glPopMatrix();
   //  Revert to fixed pipeline
   glUseProgram(0);

   //  Display axes
   Axes(2);
   //  Display parameters
   SetColor(1,1,1);
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s Mode=%s",th,ph,dim,fov>0?"Perpective":"Orthogonal",text[mode]);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glfwSwapBuffers(window);
}

//
//  Key pressed callback
//
void key(GLFWwindow* window,int key,int scancode,int action,int mods)
{
   //  Discard key releases (keeps PRESS and REPEAT)
   if (action==GLFW_RELEASE) return;

   //  Check for shift
   int shift = (mods & GLFW_MOD_SHIFT);

   //  Exit on ESC
   if (key == GLFW_KEY_ESCAPE)
     glfwSetWindowShouldClose(window,1);
   //  Move X
   if (key == GLFW_KEY_X)
   {
      if (!shift)
      {
         x+=.1;
      }
      else
      {
         x-=.1;
      }
   }
   //  Move Y
   if (key == GLFW_KEY_Y)
   {
      if (!shift)
      {
         y+=.1;
      }
      else
      {
         y-=.1;
      }
   }
   //  Move Z
   if (key == GLFW_KEY_Z)
   {
      if (!shift)
      {
         z+=.1;
      }
      else
      {
         z-=.1;
      }
   }
   //  Reset view angle
   else if (key==GLFW_KEY_0)
      th = ph = 0;
   //  Switch shaders
   else if (key==GLFW_KEY_M)
      mode = 1-mode;
   //  Switch objects
   else if (key==GLFW_KEY_O)
      obj = 1-obj;
   //  Switch between perspective/orthogonal
   else if (key==GLFW_KEY_P)
      fov = fov ? 0 : 57;
   //  Increase/decrease asimuth
   else if (key==GLFW_KEY_RIGHT)
      th += 5;
   else if (key==GLFW_KEY_LEFT)
      th -= 5;
   //  Increase/decrease elevation
   else if (key==GLFW_KEY_UP)
      ph += 5;
   else if (key==GLFW_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key==GLFW_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key==GLFW_KEY_PAGE_UP && dim>1)
      dim -= 0.1;

   //  Wrap angles
   th %= 360;
   ph %= 360;
   //  Update projection
   Projection(fov,asp,dim);
}

//
//  Window resized callback
//
void reshape(GLFWwindow* window,int width,int height)
{
   //  Get framebuffer dimensions (makes Apple work right)
   glfwGetFramebufferSize(window,&width,&height);
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Projection(fov,asp,dim);
}

//
//  Main program with GLFW event loop
//
int main(int argc,char* argv[])
{
   //  Initialize GLFW
   GLFWwindow* window = InitWindow("Tristan English - Homework 01",1,600,600,&reshape,&key);

   //  Load shader
   shader = CreateShaderProg("ndctorgb.vert","ndctorgb.frag");

   //  Event loop
   ErrCheck("init");
   while(!glfwWindowShouldClose(window))
   {
      //  Display
      display(window);
      //  Process any events
      glfwPollEvents();
   }
   //  Shut down GLFW
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}

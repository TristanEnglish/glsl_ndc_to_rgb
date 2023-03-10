// Tristan English - NDC to RGB shader
// Converts normalized device coordinates to rgb
#version 120

void main()
{
   // Set position of the vertices
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // Convert NDC to RGB by doing perspective division on gl_Position, then 
   // change range of [-1,1] to [0,1]
   gl_FrontColor = ((gl_Position/gl_Position.w) + 1.0) / 2.0;
}

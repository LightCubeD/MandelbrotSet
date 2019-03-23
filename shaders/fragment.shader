#version 400 core
    
layout(location = 0) out vec4 color;
uniform float angle;
uniform float scaler;
in vec2 FragPos;

double maxIter = 160;
double scaleX = 0.004;
double scaleY = 0.004;

bool usePolar = true;

double offsetX = 0;
double offsetY = 0;

float offsetR = 0.79234145;
float offsetW = 0;

double colorSensitivity = 1;

dvec3 Hue(double H) {
  double R = abs(H * 6 - 3) - 1;
  double G = 2 - abs(H * 6 - 2);
  double B = 2 - abs(H * 6 - 4);
  return dvec3(R,G,B);
}

vec4 HSVtoRGB(in dvec3 HSV) {
  return vec4(((Hue(HSV.x) - 1) * HSV.y + 1) * HSV.z,1);
}

double c_abs(in dvec2 c) {
  return sqrt((c.x*c.x)+(c.y*c.y));
}


void main() {
  scaleX = scaler;
  scaleY = scaler;
  dvec2 complex;
  offsetW = angle;
  if (usePolar == true) {
    offsetX = offsetR * cos(offsetW);
    offsetY = offsetR * sin(offsetW);
  }
  complex.x = (FragPos.x * scaleX) + offsetX;
  complex.y = (FragPos.y * scaleY) + offsetY;
  dvec2 z;
  bool greater2 = false;
  double steps = 0;

  for(int i = 0; i < maxIter; i++) {  //Number of iterations
    
    //Temporary values of z
    double ox = z.x; 
    double oy = z.y;

    //Calculate the new z
    //Complex number c(complex)
    //Complex number z(z, starts with 0)
    //Formula: z(n+1) = z(n)^2 + c
    //If |z(n)| >= 2, c is not in the Mandelbrot-set
    z.x = ((ox*ox)-(oy*oy)) + complex.x;
    z.y = (2*ox*oy) + complex.y;

    if(c_abs(z) >= 2) {  //Calculate |z| and compare to 2
      greater2 = true;

      
      steps = i + 1 - log(log2(float(c_abs(z))));
      //steps = i;

      break;        //stop going deeper because it is already
                    //over or equals 2 and therefore not in
                    //the MBS(MandelBrotSet)
    }
  }

  if(greater2 == true) {
    double hue = steps/(maxIter*colorSensitivity);
    double saturation = 1.0f;
    double brightness = 1.0f;
    //color = vec4(greyTone,greyTone,greyTone,1.0f);
    color = HSVtoRGB(dvec3(hue,saturation,brightness));
  } else {
    color = vec4(0.0f,0.0f,0.0f,1.0f);
  }
};
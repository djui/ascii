#include <stdio.h>
#include <math.h>
#include <vector>

// "Y!" logo
float vertex[] = { 
  -52,-30,  // 0
  -52,-25,
  -7,14,    // 2
  -7,34,
  -23,34,
  -23,39, // 5
  27,39,
  27,34,
  9,34,
  9,14,
  39,-13, // 10
  53,-16,
  56,-21,
  12,-21,
  12,-16,
  27,-14, // 15 (this one might want to move?)
  5,6,    // 16 is the top vertex of the fork in the Y
  -17,-24, // 17  is the y coord right??
  -1,-26,  // 18
  -1,-30,  // 19
  -37,-24,  // 20 = 1.5 (oops)
  106,-67,  // 21 (start of !)
  106,26,
  123,28,
  146,-63,
  102,40, // 25
  100,60,
  122,63,
  125,43,
};

float xscale = 0.300;
float yscale = 0.50;
int angle_quantum = 46;
int offset_quantum = 47;
float offset_max = 39.5*(offset_quantum)/(offset_quantum-1);

int polygon[] = {
  0,1,20,17,18,19,0,-1,  // top left serif
  20,2,16,17,20,-1,      // left diagonal
  2,3,8,9,16,2,-1,       // middle vertical
//  3,4,5,6,7,8,3,-1,      // bottom serif
  3,4,5,6,7,3,-1,      // bottom serif (point 8 omitted as it's colinear with 7 and 3)
  9,10,15,16,9,-1,       // right diagonal
  10,11,12,13,14,15,10,-1, // top right serif
  21,22,23,24,21,-1,     // top of !
  25,26,27,28,25,-1,     // bottom of !
  -2};

void dump_vector(const std::vector<int> &o, const char *var, int offset) {
  printf("// %s=[\"", var, o.size());
  for(int i=0;i<o.size();i++) {
    int c = o[i] + offset;
    if(c == '"' || c == '\\') putchar('\\');
    putchar(c);
  }
  printf("\"]-%d\n", offset);
}

int main()
{
  int i,j,pn=0;
  std::vector<int> output_l, output_a, output_b;
  for(j=0;polygon[j] != -2;pn++) {
    float avgx=0, avgy=0;
    for(i=0;polygon[j+i+1]!=-1;i++) {
      int p0 = polygon[j+i],
          p1 = polygon[j+i+1];
      float x0 = vertex[p0*2]*xscale,
            y0 = vertex[p0*2+1]*yscale,
            x1 = vertex[p1*2]*xscale,
            y1 = vertex[p1*2+1]*yscale,
            dx = x1-x0,
            dy = y1-y0;
      avgx += x0;
      avgy += y0;
      float norm = sqrt(dx*dx + dy*dy);
      // L(x,y) is the half-plane normal = line segment vector (dx,dy) rotated
      // 90 degrees left (on a left-handed coordinate system)
      float Lx = dy;
      float Ly = -dx;
      float angle = round(angle_quantum*atan2(Ly, Lx)/M_PI);
      //if(angle<0) angle += 2*angle_quantum;  // positive angles only?  no need
      // todo: quantize Lx,Ly
      // just use sin/cos?  64 angles?
      float Lx2 = cos(M_PI*angle/angle_quantum);
      float Ly2 = sin(M_PI*angle/angle_quantum);
      // calculate B based on center of segment so any angle errors are
      // propagated equally through the segment
      float B = Lx2*(x0+x1)/2+Ly2*(y0+y1)/2;
      float B2 = ceil(offset_quantum*B/offset_max); // hmm, we need to slightly inflate this away from the polygon centroid but how?
      // round B2 such that ((x0+x1)/2, (y0+y1)/2) is in the interior
      float B2scaled = offset_max*B2/offset_quantum;
      if(Lx2*(x0+x1)/2 + Ly2*(y0+y1)/2 < B2scaled) {
        B2--;
      }

      if(B2 == offset_quantum) B2--;
      B2scaled = offset_max*B2/offset_quantum;
      float ax = avgx/(1+i);
      float ay = avgy/(1+i);
      printf("// polygon %d segment %d (%d-%d): L=(%g,%g) B=%g check(%g,%g)=%g\n", pn, i, p0,p1, Lx,Ly,B, ax/xscale,ay/yscale, ax*Lx+ay*Ly-B);
      //printf("a=%g,(%g,%g)->(%g,%g),%g,\n", angle, Lx/norm,Ly/norm, Lx2,Ly2,B);
      printf("// %d,%d,\n", (int)angle, (int)B2);
      int aa = (int) angle;
      int bb = (int) B2; // -32 to 31
      int e1 = aa, e2 = -bb;
      output_a.push_back(e1);
      output_b.push_back(e2);
      printf("%g,%g,%g,\n", Lx2,Ly2,B2scaled);
    }
    output_l.push_back(output_a.size());
    printf("-99,\n");
    j += i+2;
  }
  printf("-98\n");
  printf("// anglescale = %g ^-1 = %g\n", M_PI/angle_quantum, angle_quantum/M_PI);
  printf("// offsetscale = %g ^-1 = %g\n", offset_max/offset_quantum, offset_quantum/offset_max);

  dump_vector(output_l, "length", 50);
  dump_vector(output_a, "a", 79);
  dump_vector(output_b, "b", 79);
  printf("// total length=%d\n", output_a.size());
}
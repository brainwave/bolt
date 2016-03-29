#include <fstream>
#include <iostream>
#include <vector>

#include <string.h>
#include <stdio.h>



struct point{
  
  float x, y, z;
  
  point( float x, float y, float z ) :
  x(x),
  y(y),
  z(z)
  {}
  
  void fill( float x1, float y1, float z1 )  {
  
    x = x1;
    y = y1;
    z = z1;
      
  }
  
};



class triangle  {
  
  const point p1, p2, p3;
  
public:
  triangle( point &p1, point &p2, point &p3 ) :
  p1( p1.x, p1.y, p1.z ),
  p2( p2.x, p2.y, p2.z ),
  p3( p3.x, p3.y, p3.z )  
  {  
    
    std::cout << "a baby triangle borned!" << std::endl;
    std::cout << "P1: x = " << p1.x << "   y = " << p1.y << " z = " << p1.z << std::endl;
    std::cout << "P2: x = " << p2.x << "   y = " << p2.y << " z = " << p2.z << std::endl;
    std::cout << "P3: x = " << p3.x << "   y = " << p3.y << " z = " << p3.z << std::endl;
    
  }
  
};

void err()  {
  
    std::cerr << "Fail!" << std::endl;
    exit( EXIT_FAILURE );  
  
}

int main()  {
  
  //for sake to have any exmaple
  
  FILE *temp = NULL;
  if( ( temp = fopen( "some", "w+" ) ) == NULL )  err();
  fprintf( temp, "1234xx1234xx1453  8463yyHFSHyyJSHW  KJDSzzWDNJzzJUY&^    moreshit data that will get ignored unless too long like it seems so" );
  fclose( temp );
  
  size_t object = 2 * 2 +  3 * sizeof( float ), moveSize = 2 + sizeof( float );
  size_t floatSize = sizeof( float ), gap = 2;
  char buff[ object ] = {0};
  char *buffPtr = NULL;
  
  float x = 0, y = 0, z = 0;
  point p1(0,0,0), p2(0,0,0), p3(0,0,0);
  std::vector<triangle> triangles;
  
  std::ifstream file;
  file.open( "some", std::ios::out | std::ios::binary );
  if( ! file.good() )   err();
  
  int i = 1;
  for( ; file.read( buff, object ); i++ )  {
    
    buffPtr = &buff[0];
    memcpy( &x, buffPtr, floatSize );
    buffPtr += moveSize;
    memcpy( &y, buffPtr, floatSize );
    buffPtr += moveSize;    
    memcpy( &z, buffPtr, floatSize );
    buffPtr += floatSize;    
    
    if( i == 1 )       p1.fill( x, y, z );
    else if( i == 2 )  p2.fill( x, y, z );
    else  {
      
      p3.fill( x, y, z );
      triangles.push_back( triangle( p1, p2, p3 ) );
      
      i = 0;
      
    }
     
    if( ! file.read( buff, gap ) )  break;
    
  }
  
  file.close();
  
  remove( "some" );
  
  return 0;
  
}

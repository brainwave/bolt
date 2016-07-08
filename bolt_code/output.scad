//Intialising constants
min_x=-90; max_x=90; min_y=-90; max_y=90; height=78; thickness=1; interval = 15; infile = "stlfiles/frustum.stl";

import(infile);

for( x= [min_x: interval :max_x])
    for(y=[min_y: interval :max_y] ){    
      translate([x,y,0]) {
        cylinder(height, thickness, thickness);
    }
}   

    

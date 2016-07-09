//Intialising constants
min_x=-90; max_x=90; min_y=-90; max_y=90; height=78; thickness=1; interval = 30; infile = "support.stl";


union() {
    difference() {
        support();
        import(infile);
    }
    import(infile);    
}   


module support() {
 for( x= [min_x: interval :max_x])
    for(y=[min_y: interval :max_y] ){    
      translate([x,y,0]) {
        union() {
            cylinder(height-1, thickness,thickness);
            //translate ([0,0,height])
                //cylinder(height/10,thickness,0);
            cylinder(height/15, thickness*5, thickness*1.5);
       }       
     }
 }      
}
  
    
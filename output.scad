//Intialising constants
min_x=-90; max_x=90; min_y=-90; max_y=90; height=78; thickness=20; interval = 10; infile = "support.stl";

echo(min_x, max_x, min_y, max_y, height, thickness, interval);

mid_x = (min_x+max_x)/2;
mid_y = (min_y+max_y)/2;
intersection(){
union() {
    difference() {
        support();
        import(infile);
    }
    import(infile);
}

translate([mid_x,mid_y,0])
linear_extrude(height, center = false, convexity=10, twist = 0)
    resize(newsize=[max_x-min_x+thickness*4, max_y-min_y+thickness*4, 0])
        translate([-1*mid_x, -1*mid_y,0])
        projection(cut=false) 
           import(infile);  
 }

module support() {
  for( x= [min_x: interval :max_x])
    for(y=[min_y: interval :max_y] ){    
      translate([x,y,0]) {
        
            cylinder(height,thickness,thickness);
            //translate ([0,0,height])
               //cylinder(height/10,thickness,0);
           //cylinder(height/15, thickness*4  , thickness*1.5);
     }
     
  }
  /*To ensure that last edge always supported
  for(y = [min_y: interval: max_y])
    translate(max_x,y,0) {
      cylinder(height,thickness,thickness); }
*/
}
  
    
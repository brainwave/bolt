
for(i = [min_x: 10: max_x])
    for(j = [min_y: 10: max_y])
        translate([i,j,0])
            cylinder(h=max_z-min_z, r=thickness, r=thickness);
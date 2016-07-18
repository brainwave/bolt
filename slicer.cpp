
bool activeEdgeTableComparision(const EdgeTableRecord &a, const EdgeTableRecord &b){
	
	if( a.x_y_min < b.x_y_min)		
		return true;
	else{
		if(a.x_y_min == b.x_y_min){	
			
			if(a.y_min < b.y_min){
			
				return true;
			}
			else{
		
				if(a.y_min == b.y_min){
				
					if(a.y_max < b.y_max){
	
						return true;
					}
					else{
						if(a.y_max ==  b.y_max){
							
							if(a.inverse_slope<b.inverse_slope){
					
								return true;
							}
							else{
				
								return false;
							}
						}
						else{
						
							return false;
						}
					}
				}
				else{
					
					return false;
				}
			}
		}
		else{
		
			return false;
		}
	}
}

bool globalEdgeTableComparision (const EdgeTableRecord &a, const EdgeTableRecord &b){

		if(a.y_min < b.y_min)
			return true;

		else if(a.y_min == b.y_min){
			
			if(a.x_y_min < b.x_y_min)
				return true;

			else if(a.x_y_min == b.x_y_min){
			
				if(a.y_max < b.y_max)
					return true;
				
				else
					return false;
			}
	
			else
				return false;
		}
		
		else
			return false;
}

void slice::fillSlice(int xres, int yres) {

	if(isFilled)
		return;

	vector <glm::vec3> vertices, intersections;
	int boundaryVertexCount = 0;

	for ( auto it = boundary.begin(); it != boundary.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
		boundaryVertexCount+=2;
	}

	if(boundaryVertexCount == 0){
	
		isFilled = true;
		return;
	}

	float yMin = vertices[0].y, yMax = vertices[0].y, xMin = vertices[0].x, xMax = vertices[0].x, step;
	float zCoord = vertices[0].z;	

	bool first = true;

	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		yMin = (it->y < yMin) ? it->y : yMin; yMax = (it->y > yMax) ? it->y : yMax;
		xMin = (it->x < xMin) ? it->x : xMin; xMax = (it->x > xMax) ? it->x : xMax;
	}

	step =(yMax - yMin) / ( xres<yres ?xres:yres )/1.1;
	
	// EDGE TABLE ALGORITHM

	glm::vec3 vertex1, vertex2;
	
	vector <EdgeTableRecord> global, active;

	// creation of the global edge table
	for(auto it = vertices.begin(); it!=vertices.end(); it++){
	
		if(first){
		
			vertex1 = *it;
			
			first = false;
		}

		else{
			
			vertex2 = *it;
	
			first = true;			

			float y_min, y_max, x_y_min,x_y_max, inverse_slope;
			
			if(vertex1.y<vertex2.y){
	
				y_min = vertex1.y;
				y_max = vertex2.y;
				x_y_min = vertex1.x;
				x_y_max = vertex2.x;
			}
			else if(vertex1.y == vertex2.y){
			
				y_min = y_max = vertex1.y;
				if(vertex1.x<vertex2.x){
					
					x_y_min = vertex1.x;
					x_y_max = vertex2.x;
				}
				else{
			
					x_y_min = vertex2.x;
					x_y_max = vertex1.x;
				}
			}
			else{
	
				y_min = vertex2.y;
				y_max = vertex1.y;
				x_y_min = vertex2.x;
				x_y_max = vertex1.x;
			}


			if(y_max == y_min)
				inverse_slope = numeric_limits<float>::max();
			else
				inverse_slope = (vertex1.x - vertex2.x)/(vertex1.y - vertex2.y);
			
			EdgeTableRecord edge;

			edge.y_min = y_min;
			edge.y_max = y_max;
			edge.x_y_min = x_y_min;
			edge.x_y_max = x_y_max;
			edge.inverse_slope = inverse_slope;
		
			if(y_min != y_max)				
				global.push_back(edge);
		}
	}

	// sort based on y_min, then x_y_min, then y_max	
	sort(global.begin(),global.end(),globalEdgeTableComparision);

	for(float y = yMin; y<=yMax; y+=step){

		// remove active edges with y_max < y
		for(auto it = active.begin(); it!=active.end(); ){

			if(it->y_max<y){
	
				active.erase(it);
			}
			else{
	
				it++;
			}
		}
	
		// add edges with y_min = y to the active edge list
		for(auto it = global.begin(); it!=global.end() && it->y_min<=y; ) {
			
			if(y==it->y_min){
			
				active.push_back(*it);
			}
			else if(y>it->y_min){
				
				it->x_y_min = it->inverse_slope*(y - it->y_max) + it->x_y_max;
				active.push_back(*it);
			}
			global.erase(it);
		}
	
		// sort active edge table records in ascending order of x_y_min
		sort(active.begin(),active.end(),activeEdgeTableComparision);

		intersections.clear();

		for(auto it = active.begin(); it!=active.end(); it++){
	
			vertex1.x = it->x_y_min;
			vertex1.y = y;
			vertex1.z = zCoord;
				
			if(y>it->y_min && y<it->y_max) {
	
				intersections.push_back(vertex1);
			}
			else if(y==it->y_min && y!=it->y_max){	
			
				for(auto iter = active.begin(); iter!=active.end(); iter++){
	
					if(iter->y_max==y){
						
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end())){
	
							intersections.push_back(vertex1);	
						}
					}
				}
			}
			else if(y==it->y_max && y!=it->y_min){
	
				for(auto iter = active.begin(); iter!=active.end(); iter++){		
			
					if(iter->y_min==y){
					
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end()))
	
							intersections.push_back(vertex1);
					}
				}
			}
		}
		
		for(auto it = active.begin(); it!=active.end(); it++){	
			
			if(it->y_max != it->y_min)	
				it->x_y_min = it->x_y_min + step*it->inverse_slope;
		}

		if(intersections.size() % 2 == 1)
			intersections.erase(intersections.end() - 1);

	
		for(auto iter = intersections.begin(); iter!=intersections.end(); iter+=2){	
			
			fill.push_back(linesegment(*iter,*(iter+1)));
		}	

	}

	isFilled = true;
}


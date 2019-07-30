std::vector<int> computeCSCode(std::vector<vec3> clipWindow, const vec3 p){
	
	std::vector<int> code;
	code.resize(4);
	float maxX = clipWindow[0].x;
	float minX = clipWindow[0].x;
	float maxY = clipWindow[0].y;
	float minY = clipWindow[0].y;
	
	for(int i = 0; i < clipWindow.size(); i++){
		if(maxX < clipWindow[i].x)
			maxX = clipWindow[i].x;
		if(minX > clipWindow[i].x)
			minX = clipWindow[i].x;
		if(maxY < clipWindow[i].y)
			maxY = clipWindow[i].y;
		if(minY > clipWindow[i].y)
			minY = clipWindow[i].y;
	}
	
		code.at(0) = 0; //Above
		code.at(1) = 0; //Under
		code.at(2) = 0; //Right
		code.at(3) = 0; //Left
		
		if(p.x < minX)
			code.at(3) = 1;
		if(p.x > maxX)
			code.at(2) = 1;
		if(p.y < minY)
			code.at(1) = 1;
		if(p.y > maxX)
			code.at(0) = 1;
		
		return code;
}

bool simpleAcceptance(std::vector<int> code1, std::vector<int> code2){
	if(code1.at(0) == 0 && code1.at(1) == 0 && code1.at(2) == 0 && code1.at(3) == 0 && code2.at(0) == 0 && code2.at(1) == 0 && code2.at(2) == 0 && code2.at(3) == 0 )
		return true;
	return false;
}

bool simpleRejection(std::vector<int> code1, std::vector<int> code2){
	if( (code1.at(0) & code2.at(0) == 1 ) || (code1.at(1) & code2.at(1) == 1 ) || (code1.at(2) & code2.at(2) == 1 ) || (code1.at(2) & code2.at(2) == 1 ) )
		return true;
	return false;
}

int lineClip_CyrusBeck(std::vector<vec3> clipWindow, vec3& p1, vec3& p2){
	
	std::vector<vec3> N, PEi;
	int i;
	
	vec3 norm = crossProduct(clipWindow.at(0) - clipWindow.at(1), clipWindow.at(2) - clipWindow.at(1));

	for(int i = 0; i < clipWindow.size() - 1; i++){
		vec3 v = crossProduct(norm, clipWindow.at(i+1) - clipWindow.at(i));
		v.normalize();
		N.push_back(v);
	}
	
	vec3 v = crossProduct(norm, clipWindow.at(0) - clipWindow.at(clipWindow.size() - 1);
	v.normalize();
	N.push_back(v);
	
	for(int i = 0; i < clipWindow.size(); i++){
		PEi.push_back(vec3(clipWindow[i].x, clipWindow[i].y, 0));
	}
	
	if(p1 == p2)
		return 0;
	
	else{
		
		float tE = 0.0;
		float tL = 1.0;
		int PE = 0;
		int PL = 0;
		vec3 D = p2 - p1;
		
		for(int i = 0; i < N.size(); i++){
			
			float product = dotProduct(N.at(i), D);
			
			PE = 0;
			PL = 0;
			
			if(product != 0){
				float t = dotProduct(N.at(i), p1 - PEi.at(i)) / dotProduct(-N.at(i), D));
				
				if(product < 0){
					tE = (tE > t) ? tE : t;
				}
				else{
					tL = (tL < t) ? tL : t;
				}
			}
		}
		
		if(tE > tL)
			return -1;
		else{
			vec3 aux1, aux2;
			aux1 = p1 + (p2 - p1)*tE;
			aux2 = p1 + (p2 - p1)*tL;
			p1 = aux1;
			p2 = aux2;
			return 1;
		}
	}
}

int lineClip_CohenSutherland(std::vector<vec3> clipWindow, vec3& p1, vec3& p2){
	
	bool finished = false;
	bool display = false;
	bool rejected;
	
	float maxX = clipWindow[0].x;
	float minX = clipWindow[0].x;
	float maxY = clipWindow[0].y;
	float minY = clipWindow[0].y;
	
	for(int i = 0; i < clipWindow.size(); i++){
		if(maxX < clipWindow[i].x)
			maxX = clipWindow[i].x;
		if(minX > clipWindow[i].x)
			minX = clipWindow[i].x;
		if(maxY < clipWindow[i].y)
			maxY = clipWindow[i].y;
		if(minY > clipWindow[i].y)
			minY = clipWindow[i].y;
	}
	
	egc::vec3 aux1;
	std::vector<int> aux2;
	
	std::vector<int> code1;
	std::vector<int> code2;
	
	code1 = computeCSCode(clipWindow, p1);
	code2 = computeCSCode(clipWindow, p2);
	
	rejected = simpleRejection(code1, code2);
	
	if(rejected)
		return -1;
	
	while(!finished){
		
		code1 = computeCSCode(clipWindow, p1);
		code2 = computeCSCode(clipWindow, p2);
		
		rejected = simpleRejection(code1, code2);
		
		if(rejected){
			finished = true;
			break;
		}
		
		else{
			display = simpleAcceptance(code1, code2);
			if(display){
				finished = true;
				break;
			}
			else{
				if(code1.at(0) == 0 && code1.at(1) == 0 && code1.at(2) == 0 && code1.at(3) == 0){
					
					aux1 = p1;
					p1 = p2;
					p2 = aux1;
					
					aux2.clear();
					aux2.resize(4);
					aux2.assign(code1.begin(), code1.end());
					code1.assign(code2.begin(), code2.end());
					code2.assign(aux2.begin(), aux2.end());
				}
				
				if(code1.at(0) == 1 && (p1.y != p2.y)){
					p1.x = p1.x + (p2.x - p1.x) * (maxY - p1.y) / (p2.y - p1.y);
					p1.y = maxY;
				}
				else if(code1.at(1) == 1 && (p1.y != p2.y)){
					p1.x = p1.x + (p2.x - p1.x) * (minY - p1.y) / (p2.y - p1.y);
					p1.y = minY;
				}
				else if(code1.at(2) == 1 && (p1.x != p2.x)){
					p1.y = p1.y + (p2.y - p1.y) * (maxX - p1.x) / (p2.x - p1.x);
					p1.x = maxX;
				}
				else if(code1.at(3) == 1 && (p1.x != p2.x)){
					p1.y = p1.y + (p2.y - p1.y) * (minX - p1.x) / (p2.x - p1.x);
					p1.x = minX;
				}
			}
		}
	}
	return 0;
}
	
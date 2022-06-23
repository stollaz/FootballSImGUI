#include "include.h"

using namespace std;

#ifdef PPM
    #define HEIGHT 640
    #define WIDTH 640
#endif
#ifndef PPM
    #define HEIGHT 64
    #define WIDTH 64
#endif
    
// #define HEIGHT 64
// #define WIDTH 64

void lookAt(glm::vec3 cameraPosition, glm::mat3 &cameraOrientation, glm::vec3 lookAtPoint){
	glm::vec3 vertical = {0,1,0};
	glm::vec3 forward = glm::normalize(cameraPosition-lookAtPoint);
	glm::vec3 right = glm::normalize(glm::cross(vertical, forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));
	cameraOrientation = {right, up, forward};
}

double Dist(int x1, int y1, int x2, int y2){
    return (sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}

bool intersectPlane(const glm::vec3 center, glm::vec3 normal, glm::vec3 camera_pos, glm::vec3 ray_direction) 
{ 
    float denom = glm::dot(normal, ray_direction);
    if (abs(denom) > 0.0001f) // your favorite epsilon
    {
        float t = glm::dot((center-camera_pos),normal)/denom;
        if (t >= 0) return true; // you might want to allow an epsilon here too
    }
    return false;
} 

    // intersectionPoint = glm::vec3(camera_pos.x+t*ray_direction.x, camera_pos.y+t*ray_direction.y, camera_pos.z+t*ray_direction.z);

float hit_sphere(const glm::vec3 center, float radius, glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3& intersectionPoint){
    glm::vec3 oc = camera_pos - center;
    auto a = glm::length(ray_direction)*glm::length(ray_direction);
    auto half_b = glm::dot(oc, ray_direction);
    auto c = glm::length(oc)*glm::length(oc) - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    auto t = (-half_b - sqrt(discriminant) ) / a;
    intersectionPoint = glm::vec3(camera_pos.x+t*ray_direction.x, camera_pos.y+t*ray_direction.y, camera_pos.z+t*ray_direction.z);

    if (discriminant < 0) {
        return -1.0;
    } else {
        return t;
    }
}

bool Equal(fmt::rgb a, fmt::rgb b){
    return (a.r==b.r && a.g==b.g && a.b==b.b);
}

// Change this to return an id / object of what it hits (as well as colour or instead?)
// This would make it easier to do shadows on "textured" surfaces, e.g. floor with checker pattern
fmt::rgb castRay(glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3 light_pos, Geometry g, float& closeness, glm::vec3& intersection, glm::vec3& normal){
    fmt::rgb ret = fmt::color::cyan;
    float dist_=1000;
    float closeness_ = 0;

    // https://education.siggraph.org/static/HyperGraph/raytrace/rayplane_intersection.htm 
    // Try that for getting coordinate of point it hits
    for (Plane p : g.planes){
        glm::vec3 normal = p.normal;
        float denom = glm::dot(normal,ray_direction);
        if (abs(denom) >= 0) // your favorite epsilon
        {
            float t = glm::dot((glm::vec3(normal.x*p.d,normal.y*p.d,normal.z*p.d) - camera_pos),normal) / denom;
            // glm::vec3 test = (glm::vec3(20,20,-35) - camera_pos)/denom;
            glm::vec3 intersectionPoint = camera_pos + t*ray_direction;
            intersection = intersectionPoint;
            // fmt::print("{},{},{}   ",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
            if (t >= 0) ret = p.col;
        }
    }

    //https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf
    // Try this for getting coordinate of point it hits on sphere
    for (Sphere c : g.spheres){
        glm::vec3 intersectionPoint;
        float discriminant = hit_sphere(c.centre,c.r, camera_pos, ray_direction, intersectionPoint);
        if (discriminant >= 0){
            intersection = intersectionPoint;
            // fmt::print("{}:",discriminant);
            if (glm::distance(camera_pos,c.centre) < dist_){
                ret = c.col;
                dist_ = glm::distance(camera_pos,c.centre);
                closeness_ = discriminant;
                normal = glm::normalize(intersection - c.centre);
                // fmt::print("{}:",(int)closeness_);
            }
        }
    }    

    closeness = closeness_;
    return ret;
}

fmt::rgb castRay(glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3 light_pos, Geometry g, float& closeness, glm::vec3& intersection, glm::vec3& normal, Sphere& sphere_hit){
    fmt::rgb ret = fmt::color::cyan;
    float dist_=1000;
    float closeness_ = 0;

    // https://education.siggraph.org/static/HyperGraph/raytrace/rayplane_intersection.htm 
    // Try that for getting coordinate of point it hits
    for (Plane p : g.planes){
        glm::vec3 normal = p.normal;
        float denom = glm::dot(normal,ray_direction);
        if (abs(denom) >= 0) // your favorite epsilon
        {
            float t = glm::dot((glm::vec3(normal.x*p.d,normal.y*p.d,normal.z*p.d) - camera_pos),normal) / denom;
            // glm::vec3 test = (glm::vec3(20,20,-35) - camera_pos)/denom;
            glm::vec3 intersectionPoint = camera_pos + t*ray_direction;
            intersection = intersectionPoint;
            // fmt::print("{},{},{}   ",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
            if (t >= 0) ret = p.col;
        }
    }

    //https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf
    // Try this for getting coordinate of point it hits on sphere
    for (Sphere c : g.spheres){
        glm::vec3 intersectionPoint;
        float discriminant = hit_sphere(c.centre,c.r, camera_pos, ray_direction, intersectionPoint);
        if (discriminant >= 0){
            intersection = intersectionPoint;
            // fmt::print("{}:",discriminant);
            if (glm::distance(camera_pos,c.centre) < dist_){
                ret = c.col;
                dist_ = glm::distance(camera_pos,c.centre);
                closeness_ = discriminant;
                normal = glm::normalize(intersection - c.centre);
                sphere_hit = c;
                // fmt::print("{}:",(int)closeness_);
            }
        }
    }    

    closeness = closeness_;
    return ret;
}

void Render(Geometry g, glm::vec3 camera_pos, glm::mat3 camera_orientation){
    glm::vec3 light_pos = g.light.pos;
    float f = 1.0;
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    // _setmode(_fileno(stdout), 0x00020000);
    for (int x = 0; x < HEIGHT; x++){
		for (int y = 0; y < WIDTH; y++){
			// cout << "x,y: " << x << ","<<y<<endl;
			float a = (float(x)/float(HEIGHT)-0.5);
			float b = (float(y)/float(WIDTH)-0.5);
			//cout << "a,b: " << a << "," << b << endl;
			//std::cout << "a,b = " << a << "," << b << std::endl;
			glm::vec3 pixel = glm::vec3(a,-b, -f); 
			glm::vec3 ray_direction = camera_orientation * (pixel);

			fmt::rgb c = fmt::color::black;

			//c2 = castRay(camera_pos, ray_direction, light2, lighting_type, t);
			//c1 = castRay(camera_pos, ray_direction, light1, lighting_type, t);
			//c3 = castRay(camera_pos, ray_direction, light3, lighting_type, t);

			//c.blue = (c1.blue+c2.blue+c3.blue)/3;
			//c.green = (c1.green+c2.green+c3.green)/3;
			//c.red = (c1.red+c2.red+c3.red)/3;
			
            glm::vec3 intersectionPoint = glm::vec3(0,0,0);

            float closeness;
            glm::vec3 normal;
            Sphere spherehit = Sphere(0, glm::vec3(0,0,0), fmt::rgb(0,0,0));
			c = castRay(camera_pos, ray_direction, light_pos, g, closeness, intersectionPoint, normal, spherehit);
            
            float light_intensity = 1; // Lighting disabled as it doesnt work properly

            // fmt::print("{},{},{}",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);

            if (Equal(c,g.planes[1].col)) {
                float newclose;
                glm::vec3 newpoint;
                // fmt::rgb newcol = castRay(intersectionPoint, -g.planes[1].normal, light_pos, g, newclose, newpoint);
                fmt::rgb newcol = castRay(intersectionPoint, -(intersectionPoint-light_pos), light_pos, g, newclose, newpoint, normal);
                // if (c != newcol) fmt::print("c");
                // c = newcol;
                // fmt::print("{},{},{}",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
                fmt::rgb checkercolor = c;
                if ((int)(intersectionPoint.z/10-20)%2 == (int)(intersectionPoint.y/10-20)%2) checkercolor = fmt::color::light_gray;

                // Shadows
                if (!Equal(c,newcol)) c = fmt::rgb(checkercolor.r/4,checkercolor.g/4,checkercolor.b/4);
                else c = checkercolor;
                // fmt::print("{},{},{}\n",checker.b,checker.b,checker.b);
                // else{
                //     if ((int)(intersectionPoint.z/10-10)%2 == (int)(intersectionPoint.y/10-10)%2) c = fmt::color::light_gray;       
                //     c = fmt::color((4,4,4));         
                // }
            }
            else if (!Equal(c,g.planes[0].col)) {
                // https://www.geogebra.org/3d?lang=en use this for help?
                if (Equal(c,g.spheres[0].col)){ // Mirror test - acts more like glass at the moment if removing the 2.0f*
                    // glm::vec3 normal_ = glm::normalize(intersectionPoint - g.spheres[0].centre);

                    c = fmt::rgb(255*(normal.x+1)/2,255*(normal.y+1)/2,255*(normal.z+1)/2);

                    // normal = -g.planes[1].normal;

                    glm::vec3 R_i = glm::normalize(ray_direction);
                    glm::vec3 R_r = glm::normalize(R_i-2.0f*glm::dot(R_i,normal)*normal);

                    glm::vec3 point = intersectionPoint + 0.1f*normal;

                    float newclose;
                    glm::vec3 newhit;
                    Sphere newhitsphere = Sphere(0, glm::vec3(0,0,0), fmt::rgb(0,0,0));
                    glm::vec3 newnormal;

                    c = castRay(point, R_r, light_pos, g, newclose, newhit, normal, newhitsphere);
                    if (Equal(c,g.planes[1].col) && (int)(newhit.z/10-20)%2 == (int)(newhit.y/10-20)%2) c = fmt::color::light_gray;
                    else if (Equal(c,g.spheres[4].col)){
                        c = fmt::rgb(255*(normal.x+1)/2,255*(normal.y+1)/2,255*(normal.z+1)/2);
                    }
                    else if (newhitsphere.r != 0){
                        // float len = glm::length(light_pos-newhit)/2;
                        // float intensity = 50/(4*3.14159*len*len);
                        // light_intensity = clamp(intensity,0.5f,1.0f);

                        // c.b *=light_intensity;
                        // c.g *=light_intensity;
                        // c.r *=light_intensity;

                        glm::vec3 light_direction = glm::normalize(light_pos - newhit);
                        float intensity = glm::dot(newnormal, light_direction);
                        light_intensity = clamp(intensity,0.7f,1.0f);
                    }
                }
                else if (Equal(c,g.spheres[4].col)){
                    c = fmt::rgb(255*(normal.x+1)/2,255*(normal.y+1)/2,255*(normal.z+1)/2);
                }
                else{
                    // float newclose;
                    // glm::vec3 newpoint;
                    // float mindist = glm::length(light_pos - spherehit.centre)-spherehit.r;
                    // float maxdist = glm::length(light_pos - spherehit.centre)+spherehit.r;
                    // float distrange = maxdist-mindist;
                    // // fmt::rgb newcol = castRay(intersectionPoint+0.1f*normal, -(intersectionPoint-light_pos), light_pos, g, newclose, newpoint, normal);
                    // // light_intensity = max(0.15,0.2*glm::distance(intersectionPoint, newpoint)*glm::distance(intersectionPoint, newpoint));
                    // // light_intensity = glm::dot(normal,-(intersectionPoint-light_pos));
                    // light_intensity = 1-(glm::length(intersectionPoint - light_pos)-mindist)/maxdist;

                    // float len = glm::length(light_pos-intersectionPoint)/2;
                    // float intensity = 50/(4*3.14159*len*len);
                    // light_intensity = clamp(intensity,0.5f,1.0f);

                    // c.b *=light_intensity;
                    // c.g *=light_intensity;
                    // c.r *=light_intensity;

                    glm::vec3 light_direction = glm::normalize(light_pos - intersectionPoint);
                    float intensity = glm::dot(normal, light_direction);
                    light_intensity = clamp(intensity,0.5f,1.0f);
                }
                
                // fmt::print("{}",light_intensity);
            }

            float dist = glm::distance(intersectionPoint,camera_pos);
            // fmt::print("{}",dist);

            // glm::vec3 newIntersection = glm::vec3(0,0,0);
            // float closeness_prime = 0;
            // glm::vec3 newDirection = glm::normalize(intersectionPoint - light_pos);
            // fmt::rgb c_prime = castRay(light_pos, newDirection, light_pos, g, closeness_prime, newIntersection);
            // if (c_prime != c) c = fmt::color::white;

            // float light_closeness;
            // fmt::rgb lightcol = castRay(light_pos, light_pos*pixel, light_pos, g, light_closeness, intersectionPoint);

            // if (lightcol != c) c = fmt::color::white;

            // fmt::print("{}",light_closeness);

            // float light_intensity = light_closeness/32278.5;     
            // light_intensity = clamp(light_intensity,0.f,1.f);    

            // fmt::print("{}",light_closeness);  
            fmt::rgb rgb = fmt::rgb(c);
            // rgb.r = clamp((float)rgb.r,0.0f,255.0f);
            // rgb.g = clamp((float)rgb.g,0.0f,255.0f);
            // rgb.b = clamp((float)rgb.b,0.0f,255.0f);
            rgb.b = clamp(rgb.b*light_intensity,0.0f,255.0f);
            rgb.g = clamp(rgb.g*light_intensity,0.0f,255.0f);
            rgb.r = clamp(rgb.r*light_intensity,0.0f,255.0f);
            // rgb.b = clamp(rgb.b-(145-closeness),0.0f,255.0f);
            // rgb.g = clamp(rgb.g-(145-closeness),0.0f,255.0f);
            // rgb.r = clamp(rgb.r-(145-closeness),0.0f,255.0f);
            
            // fmt::print(bg(rgb),L"  ");
            #ifdef PPM
                std::cout << int(rgb.r) << " " << int(rgb.g) << " " << int(rgb.b) << "\n";
            #endif
            #ifndef PPM
                fmt::print(bg(rgb),L"  ");
            #endif

            // fmt::print("Col: {},{},{}\n",rgb.r,rgb.g,rgb.b);

            // if (hitDist != 1000) fmt::print("Hit dist: {}\n",hitDist);
			//cout << "x,y: " << x << ","<<y<<endl;
			// window.setPixelColour(x, y, colourToInt(c));
			//cout << "x,y: " << x << ","<<y<<endl;
		}
        #ifndef PPM
            fmt::print("\n");
        #endif
	}
    // _setmode(_fileno(stdout), 0x4000);
}

// void PrintGrid(Geometry g){
//     fmt::print("Geometry Name: {}\n",g.name);
//     _setmode(_fileno(stdout), 0x00020000);
//     for (int i = 0; i < HEIGHT; i++){
//         for (int j = 0; j < WIDTH; j++){
//             fmt::rgb colour = g.background;
//             for (Plane p : g.planes){

//             }
//             for (Sphere c : g.spheres){
//                 if (Dist(j,i,c.x,c.y) <= c.r) colour = c.col;
//             }
//             fmt::print(fg(colour),L"■ ");
//         }
//         fmt::print("\n");
//     }
//     _setmode(_fileno(stdout), 0x4000);
// }

void Gradient(){
    _setmode(_fileno(stdout), 0x00020000);
    for (int x = 0; x < HEIGHT; x++){
		for (int y = 0; y < WIDTH*2; y++){
            int g = ((float)x/(float)HEIGHT)*255;
            int r = ((float)y/(float)(WIDTH*2))*255;
            int b = 100;
            fmt::rgb rgb = fmt::rgb(r,g,b);
            // rgb = fmt::rgb(255,255,255);
            
            fmt::print(bg(rgb),L" ");
        }
        fmt::print("\n");
    }
    _setmode(_fileno(stdout), 0x4000);
}

int getrand(int min, int max){
    int range = max-min;
    int r = rand()%range;
    return r+min;
}

Geometry generateRandomGeometry2(){
    Light l = Light(glm::vec3(10,15,-25),fmt::color::white);
    Geometry g = Geometry("random",fmt::color::white,l);

    srand(time(NULL));

    g.spheres.push_back(Sphere(getrand(3,4),glm::vec3(getrand(10,17),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(getrand(3,4),glm::vec3(getrand(10,17),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));

    g.spheres.push_back(Sphere(2,glm::vec3(20,getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(2,glm::vec3(20,getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(2,glm::vec3(20,getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(2,glm::vec3(20,getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(2,glm::vec3(20,getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));

    Plane wall = Plane("wall",glm::vec3(0,0,-1), -10,fmt::color::cyan);
    g.planes.push_back(wall);
    Plane floor = Plane("floor",glm::vec3(-1,0,0), -22,fmt::color::gray);
    g.planes.push_back(floor);

    return g;
}

Geometry generateRandomGeometry(){
    Light l = Light(glm::vec3(10,15,-25),fmt::color::white);
    Geometry g = Geometry("random",fmt::color::white,l);

    srand(time(NULL));
    g.spheres.push_back(Sphere(1+rand()%3,glm::vec3(getrand(10,20),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(1+rand()%3,glm::vec3(getrand(10,20),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(1+rand()%3,glm::vec3(getrand(10,20),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(1+rand()%3,glm::vec3(getrand(10,20),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));
    g.spheres.push_back(Sphere(1+rand()%3,glm::vec3(getrand(10,20),getrand(10,20),-getrand(15,25)),fmt::rgb(rand()%255,rand()%255,rand()%255)));

    Plane wall = Plane("wall",glm::vec3(0,0,-1), -10,fmt::color::cyan);
    g.planes.push_back(wall);
    Plane floor = Plane("floor",glm::vec3(-1,0,0), -22,fmt::color::gray);
    g.planes.push_back(floor);

    return g;
}

int main(){
    // Light l = Light(glm::vec3(10,15,-20),fmt::color::white);
    Light l = Light(glm::vec3(10,15,-17),fmt::color::white);
	glm::vec3 camera_pos = glm::vec3(15,15,2.0); // define camera position
	glm::mat3 camera_orientation = glm::mat3(1.0); // define camera orientation

    Geometry g = Geometry("test", fmt::color::white,l);
    // Sphere c1 = Sphere(2,glm::vec3(10,10,-20), fmt::color::red);
    // g.spheres.push_back(c1);
    // Sphere c2 = Sphere(2, glm::vec3(10,5,-25),fmt::color::blue);
    // g.spheres.push_back(c2);W
    // g.spheres.push_back(Sphere(2,glm::vec3(20,20,-45),fmt::color::blue));
    g.spheres.push_back(Sphere(2,glm::vec3(20,20,-20),fmt::color::blue));
    // Sphere s = Sphere(2,glm::vec3(20,20,-20),fmt::color::blue);
    // s.func = [](float x, float y, float z) -> fmt::rgb {
    //                    return fmt::rgb(0,0,0);};
    g.spheres.push_back(Sphere(2,glm::vec3(15,15,-20),fmt::color::lime));
    g.spheres.push_back(Sphere(2,glm::vec3(10,10,-22),fmt::color::red));
    g.spheres.push_back(Sphere(2,glm::vec3(20,20,-10),fmt::color::orange));
    g.spheres.push_back(Sphere(2,glm::vec3(20,10,-10),fmt::color::black));
    // g.spheres.push_back(Sphere(2,glm::vec3(15,15,-10),fmt::color::black));
    // g.spheres.push_back(Sphere(1,glm::vec3(15,15,-20),fmt::color::yellow));

    Plane wall = Plane("wall",glm::vec3(0,0,-1), -10,fmt::color::cyan);
    g.planes.push_back(wall);
    Plane floor = Plane("floor",glm::vec3(-1,0,0), -22,fmt::color::gray);
    g.planes.push_back(floor);
    // Plane ceil = Plane("ceil",glm::vec3(-1,0,0), 100,fmt::color::cyan);
    // g.planes.push_back(ceil);

    // PrintGrid(g);
    auto start = chrono::high_resolution_clock::now();
    Render(g, camera_pos, camera_orientation);
    // Render(generateRandomGeometry2(), camera_pos, camera_orientation);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    #ifndef PPM 
        fmt::print("\nExecution took {}ms\n",duration.count());
    #endif
    // system("pause");
    // camera_pos  = glm::vec3(20,15,2);
    // Render(g, camera_pos, camera_orientation);
    // system("pause");
    // Gradient();
    // TODO:
    // - Render is back to front somehow
    // - Do better lighting
}
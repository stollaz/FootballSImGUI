#include "include.h"

using namespace std;

#define HEIGHT 480
#define WIDTH 480

double Dist(int x1, int y1, int x2, int y2){
    return (sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}


float hit_sphere(const glm::vec3 center, float radius, glm::vec3 camera_pos, glm::vec3 ray_direction){
    glm::vec3 oc = camera_pos - center;
    float a = glm::dot(ray_direction, ray_direction);
    float b = 2.0 * glm::dot(oc, ray_direction);
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    // if (discriminant > 0) fmt::print("{}",discriminant);
    return (discriminant);
}


fmt::color castRay(glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3 light_pos, Geometry g, float& closeness){
    fmt::color ret = fmt::color::black;
    float dist_=1000;
    float closeness_ = 0;
    for (Sphere c : g.spheres){
        float discriminant = hit_sphere(c.centre,c.r, camera_pos, ray_direction);
        if (discriminant > 0){
            // fmt::print("{}:",discriminant);
            if (glm::distance(camera_pos,c.centre) < dist_){
                ret = c.col;
                dist_ = glm::distance(camera_pos,c.centre);
                closeness_ = discriminant;
                // fmt::print("{}:",(int)closeness_);
            }
        }
    }



    // for (Plane p : g.planes){
    //     float denom = glm::dot(glm::vec3(0,0,1),ray_direction);
    //     if (abs(denom) > 0.0001f) // your favorite epsilon
    //     {
    //         float t = glm::dot((glm::vec3(0,0,p.z) - camera_pos),glm::vec3(0,0,1)) / denom;
    //         if (t >= 0) ret = p.col;
    //     }
    // }

    closeness = closeness_;
    return ret;
}

void Render(Geometry g, glm::vec3 camera_pos, glm::mat3 camera_orientation){
    glm::vec3 light_pos = g.light.pos;
    float f = 2.0;
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

			fmt::color c = fmt::color::black;

			//c2 = castRay(camera_pos, ray_direction, light2, lighting_type, t);
			//c1 = castRay(camera_pos, ray_direction, light1, lighting_type, t);
			//c3 = castRay(camera_pos, ray_direction, light3, lighting_type, t);

			//c.blue = (c1.blue+c2.blue+c3.blue)/3;
			//c.green = (c1.green+c2.green+c3.green)/3;
			//c.red = (c1.red+c2.red+c3.red)/3;
			
            float closeness;
			c = castRay(camera_pos, ray_direction, light_pos, g, closeness);

            float light_closeness;
            fmt::color lightcol = castRay(light_pos, light_pos*pixel, light_pos, g, light_closeness);

            // if (lightcol != c) c = fmt::color::white;

            // fmt::print("{}",light_closeness);

            float light_intensity = light_closeness/32278.5;     
            light_intensity = clamp(light_intensity,0.f,1.f);    

            light_intensity = 1; // Lighting disabled as it doesnt work properly
            // fmt::print("{}",light_closeness);  
            fmt::rgb rgb = fmt::rgb(c);
            // rgb.b = clamp(rgb.b*light_intensity,0.0f,255.0f);
            // rgb.g = clamp(rgb.g*light_intensity,0.0f,255.0f);
            // rgb.r = clamp(rgb.r*light_intensity,0.0f,255.0f);
            rgb.b = clamp(rgb.b-(145-closeness),0.0f,255.0f);
            rgb.g = clamp(rgb.g-(145-closeness),0.0f,255.0f);
            rgb.r = clamp(rgb.r-(145-closeness),0.0f,255.0f);
            
            // fmt::print(bg(rgb),L"  ");
            std::cout << int(rgb.r) << " " << int(rgb.g) << " " << int(rgb.b) << "\n";

            // fmt::print("Col: {},{},{}\n",rgb.r,rgb.g,rgb.b);

            // if (hitDist != 1000) fmt::print("Hit dist: {}\n",hitDist);
			//cout << "x,y: " << x << ","<<y<<endl;
			// window.setPixelColour(x, y, colourToInt(c));
			//cout << "x,y: " << x << ","<<y<<endl;
		}
        // fmt::print("\n");
	}
    // _setmode(_fileno(stdout), 0x4000);
}

// void PrintGrid(Geometry g){
//     fmt::print("Geometry Name: {}\n",g.name);
//     _setmode(_fileno(stdout), 0x00020000);
//     for (int i = 0; i < HEIGHT; i++){
//         for (int j = 0; j < WIDTH; j++){
//             fmt::color colour = g.background;
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

int main(){
    Light l = Light(glm::vec3(15,15,-15),fmt::color::white);
	glm::vec3 camera_pos = glm::vec3(15,15,2.0); // define camera position
	glm::mat3 camera_orientation = glm::mat3(1.0); // define camera orientation

    Geometry g = Geometry("test", fmt::color::white,l);
    // Sphere c1 = Sphere(2,glm::vec3(10,10,-20), fmt::color::red);
    // g.spheres.push_back(c1);
    // Sphere c2 = Sphere(2, glm::vec3(10,5,-25),fmt::color::blue);
    // g.spheres.push_back(c2);
    g.spheres.push_back(Sphere(2,glm::vec3(20,20,-30),fmt::color::blue));
    g.spheres.push_back(Sphere(3,glm::vec3(15,15,-30),fmt::color::lime));
    g.spheres.push_back(Sphere(2,glm::vec3(10,10,-35),fmt::color::red));
    // g.spheres.push_back(Sphere(1,glm::vec3(15,15,-20),fmt::color::yellow));

    Plane floor = Plane(-20,fmt::color::gray);
    g.planes.push_back(floor);

    // PrintGrid(g);
    Render(g, camera_pos, camera_orientation);
    // system("pause");
    // camera_pos  = glm::vec3(20,15,2);
    // Render(g, camera_pos, camera_orientation);
    // system("pause");
    // Gradient();
    // TODO:
    // - Render is back to front somehow
    // - Do better lighting
}
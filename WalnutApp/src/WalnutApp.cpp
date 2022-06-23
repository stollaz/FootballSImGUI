#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include <stdio.h>
#include <execution>
#include <queue>
#include <wtypes.h>

#include "include.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

class TestLayer : public Walnut::Layer {
public:
	int image_Size = 512;

	bool r_active = true;
	bool g_active = true;
	bool b_active = true;

	int hexcolor(int r, int g, int b)
	{
		return (b << 16) | (g << 8) | r;
	}

	virtual void OnAttach() override {
		int W = image_Size;
		int H = image_Size;

		int* m_Data = (int*)malloc(W * H * sizeof(int));
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < H; y++) {
				float alongX = x / (float)W;
				float alongY = y / (float)H;
				int base = 0x000000FF;
				float R = (int)r_active * (255 * alongX);
				float G = (int)g_active * (255 * (1 - alongX) * (1 - alongY));
				float B = (int)b_active * (255 * alongY);
				//int col = base | (R << 24) | (G << 16) | (B << 8);
				int col = hexcolor((int)R, (int)G, (int)B) | 255 << 24;
				m_Data[x + y * H] = col;
			}
		}
		m_Image = std::make_shared<Walnut::Image>(W, H, Walnut::ImageFormat::RGBA, m_Data);
	}

	void SetSettings(int H, int W, bool r, bool g, bool b) {
		image_Size = H;
		image_Size = W;
		r_active = r;
		g_active = g;
		b_active = b;
	}

	void Render(int H, int W, bool r_active, bool g_active, bool b_active) {
		int* m_Data = (int*)malloc(W * H * sizeof(int)); // Create new image buffer of RGBA ints
		// Loop through each element (pixel)
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < H; y++) {
				float alongX = x / (float)W;
				float alongY = y / (float)H;
				float R = (int)r_active*(255 * alongX);
				float G = (int)g_active * (255 * (1 - alongX) * (1 - alongY));
				float B = (int)b_active * (255 * alongY);
				//int col = base | (R << 24) | (G << 16) | (B << 8);
				int col = hexcolor((int)R, (int)G, (int)B) | 255 << 24; // col is of format #AABBGGRR
				m_Data[x + y * H] = col; // Set data
			}
		}
		m_Image = std::make_shared<Walnut::Image>(W, H, Walnut::ImageFormat::RGBA, m_Data); // Create image bugger with data
		free(m_Data); // Free memory
	}

	virtual void OnUIRender() override {
		ImGui::Begin("Main Window");
		ImGui::SetWindowFontScale(0.8f);

		// Slider to determine image size
		static int s = 512;
		ImGui::SliderInt("Size", &s, 1, 720);
		// Only render image when a setting is explicitly changed
		if (ImGui::IsItemEdited()) {
			Render(s, s, r_active, g_active, b_active);
		}

		static bool r_active = true;
		static bool g_active = true;
		static bool b_active = true;

		ImGui::Checkbox("Red Channel", &r_active);
		if (ImGui::IsItemEdited()) {
			Render(s, s, r_active, g_active, b_active);
		}

		ImGui::Checkbox("Green Channel", &g_active);
		if (ImGui::IsItemEdited()) {
			Render(s, s, r_active, g_active, b_active);
		}

		ImGui::Checkbox("Blue Channel", &b_active);
		if (ImGui::IsItemEdited()) {
			Render(s, s, r_active, g_active, b_active);
		}

		ImGui::Separator();
		static bool display = false;
		ImGui::Checkbox("Show Image", &display);

		// Only show image when button has been toggled on
		if (display) {
			//SetSettings(w, w, r_active, g_active, b_active);
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)s, (float)s });
		}

		ImGui::End();

		// 2nd window purely for debugging
		ImGui::Begin("Debug Window");
		ImGui::SetWindowFontScale(0.8f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;
};

// Test layer to figure out switching of windows
// Doing something like this would work for porting the football simulator
class TestLayer2 : public Walnut::Layer
{
public:

	int state = 0;

	virtual void OnUIRender() override
	{
		switch (state) {
			case 0: 
				BaseState(); 
				break;
			case 1: 
				State1(); 
				break;
			case 2: 
				State2(); 
				break;
		}
	}

	void BaseState() {
		state = 0;
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		if (ImGui::IsItemClicked()) State1();
		ImGui::End();
	}

	void State1() {
		state = 1;
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::Begin("Hello Again!");
		ImGui::Button("Another Button");
		if (ImGui::IsItemClicked()) State2();
		ImGui::End();
	}

	void State2() {
		state = 2;
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::Begin("Hello Once Again!!");
		ImGui::Button("Yet Another Button");
		if (ImGui::IsItemClicked()) BaseState();
		ImGui::End();
	}
};

class Tracer : public Walnut::Layer {
	virtual void OnAttach() override {
		InitialiseGeometry();
	}

	virtual void OnUIRender() override {
		ImGui::Begin("Settings");

		// TODO:	- Separate movement options into expandable menus
		//			- Add button to randomise geometry
		//			- For default geometry, add options to move objects

		// Toggle rendering on edit or just with botton
		static bool renderOnEdit = false;
		ImGui::Checkbox("Render on Settings Edit", &renderOnEdit);

		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) Render(); // Render on button click

		ImGui::Separator();

		static float cam_x = 15.0f;
		static float cam_y = 15.0f;
		static float cam_z = 2.0f;

		// Camera Options
		ImGui::SliderFloat("Camera x", &cam_x, 0.0f, 30.0f);
		if (ImGui::IsItemEdited() && renderOnEdit) Render();
		ImGui::SliderFloat("Camera y", &cam_y, 0.0f, 30.0f);
		if (ImGui::IsItemEdited() && renderOnEdit) Render();
		ImGui::SliderFloat("Camera z", &cam_z, -15.0f, 15.0f);
		if (ImGui::IsItemEdited() && renderOnEdit) Render();

		static float light_x = 10;
		static float light_y = 15.0f;
		static float light_z = -17.0f;

		// Lighting Options
		ImGui::SliderFloat("Light x", &light_x, 0.0f, 30.0f);
		if (ImGui::IsItemEdited()) {
			l.pos = glm::vec3(light_x, light_y, light_z);
			InitialiseGeometry();
			if (renderOnEdit) Render();
		}

		ImGui::SliderFloat("Light y", &light_y, 0.0f, 30.0f);
		if (ImGui::IsItemEdited()) {
			l.pos = glm::vec3(light_x, light_y, light_z);
			InitialiseGeometry();
			if (renderOnEdit) Render();
		}

		ImGui::SliderFloat("Light z", &light_z, -30.0f, 0.0f);
		if (ImGui::IsItemEdited()) {
			l.pos = glm::vec3(light_x, light_y, light_z);
			InitialiseGeometry();
			if (renderOnEdit) Render();
		}

		// Reset Settings
		if (ImGui::Button("Reset")) {
			cam_x = 15.0f;
			cam_y = 15.0f;
			cam_z = 2.0f;
			light_x = 10;
			light_y = 15.0f;
			light_z = -17.0f;
			l.pos = glm::vec3(light_x, light_y, light_z);
			InitialiseGeometry();
			if (renderOnEdit) Render();
		}

		// Random Geometry Stuff (Broken)
		/*static int _numBigSpheres = 0;
		ImGui::SliderInt("Number of Large Spheres", &_numBigSpheres, 0, 10);

		static int _numSmallSpheres = 0;
		ImGui::SliderInt("Number of Small Spheres", &_numSmallSpheres, 0, 10);

		static int _leftBoundary = 10;
		ImGui::SliderInt("Left Boundary", &_leftBoundary, 10, 20);

		static int _rightBoundary = 20;
		ImGui::SliderInt("Right Boundary", &_rightBoundary, 10, 20);

		static int _upBoundary = 10;
		ImGui::SliderInt("Up Boundary", &_upBoundary, 10, 20);

		static int _downBoundary = 20;
		ImGui::SliderInt("Down Boundary", &_downBoundary, 10, 20);

		static int _nearBoundary = -25;
		ImGui::SliderInt("Near Boundary", &_nearBoundary, -25, -15);

		static int _farBoundary = -15;
		ImGui::SliderInt("far Boundary", &_farBoundary, -25, -15);

		if (ImGui::Button("Generate Random Geometry")) {
			GenerateRandomGeometry(_numSmallSpheres, _numBigSpheres, _leftBoundary, _rightBoundary, _upBoundary, _downBoundary, _nearBoundary, _farBoundary);
		}*/

		camera_pos = glm::vec3(cam_x, cam_y, cam_z);

		ImGui::End();

		// ----------------

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		m_VPWidth = ImGui::GetContentRegionAvail().x;
		m_VPHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image) ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	// All the below is taken from ConsoleGraphics - change this to make better

	double Dist(int x1, int y1, int x2, int y2) {
		return (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
	}

	bool intersectPlane(const glm::vec3 center, glm::vec3 normal, glm::vec3 camera_pos, glm::vec3 ray_direction)
	{
		float denom = glm::dot(normal, ray_direction);
		if (abs(denom) > 0.0001f) // your favorite epsilon
		{
			float t = glm::dot((center - camera_pos), normal) / denom;
			if (t >= 0) return true; // you might want to allow an epsilon here too
		}
		return false;
	}

	float hit_sphere(const glm::vec3 center, float radius, glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3& intersectionPoint) {
		glm::vec3 oc = camera_pos - center;
		auto a = glm::length(ray_direction) * glm::length(ray_direction);
		auto half_b = glm::dot(oc, ray_direction);
		auto c = glm::length(oc) * glm::length(oc) - radius * radius;
		auto discriminant = half_b * half_b - a * c;

		auto t = (-half_b - sqrt(discriminant)) / a;
		intersectionPoint = glm::vec3(camera_pos.x + t * ray_direction.x, camera_pos.y + t * ray_direction.y, camera_pos.z + t * ray_direction.z);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			return t;
		}
	}

	bool Equal(fmt::rgb a, fmt::rgb b) {
		return (a.r == b.r && a.g == b.g && a.b == b.b);
	}

	// Change this to return an id / object of what it hits (as well as colour or instead?)
	// This would make it easier to do shadows on "textured" surfaces, e.g. floor with checker pattern
	fmt::rgb castRay(glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3 light_pos, Geometry g, glm::vec3& intersection, glm::vec3& normal) {
		fmt::rgb ret = fmt::color::cyan;
		float dist_ = 1000;
		float closeness_ = 0;

		// https://education.siggraph.org/static/HyperGraph/raytrace/rayplane_intersection.htm 
		// Try that for getting coordinate of point it hits
		for (Plane p : g.planes) {
			glm::vec3 normal = p.normal;
			float denom = glm::dot(normal, ray_direction);
			if (abs(denom) >= 0) // your favorite epsilon
			{
				float t = glm::dot((glm::vec3(normal.x * p.d, normal.y * p.d, normal.z * p.d) - camera_pos), normal) / denom;
				// glm::vec3 test = (glm::vec3(20,20,-35) - camera_pos)/denom;
				glm::vec3 intersectionPoint = camera_pos + t * ray_direction;
				intersection = intersectionPoint;
				// fmt::print("{},{},{}   ",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
				if (t >= 0) ret = p.col;
			}
		}

		//https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf
		// Try this for getting coordinate of point it hits on sphere
		for (Sphere c : g.spheres) {
			glm::vec3 intersectionPoint;
			float discriminant = hit_sphere(c.centre, c.r, camera_pos, ray_direction, intersectionPoint);
			if (discriminant >= 0) {
				intersection = intersectionPoint;
				// fmt::print("{}:",discriminant);
				if (glm::distance(camera_pos, c.centre) < dist_) {
					ret = c.col;
					dist_ = glm::distance(camera_pos, c.centre);
					closeness_ = discriminant;
					normal = glm::normalize(intersection - c.centre);
					// fmt::print("{}:",(int)closeness_);
				}
			}
		}
		return ret;
	}

	fmt::rgb castRay(glm::vec3 camera_pos, glm::vec3 ray_direction, glm::vec3 light_pos, Geometry g, glm::vec3& intersection, glm::vec3& normal, Sphere& sphere_hit) {
		fmt::rgb ret = fmt::color::cyan;
		float dist_ = 1000;
		float closeness_ = 0;

		// https://education.siggraph.org/static/HyperGraph/raytrace/rayplane_intersection.htm 
		// Try that for getting coordinate of point it hits
		for (Plane p : g.planes) {
			glm::vec3 normal = p.normal;
			float denom = glm::dot(normal, ray_direction);
			if (abs(denom) >= 0) // your favorite epsilon
			{
				float t = glm::dot((glm::vec3(normal.x * p.d, normal.y * p.d, normal.z * p.d) - camera_pos), normal) / denom;
				// glm::vec3 test = (glm::vec3(20,20,-35) - camera_pos)/denom;
				glm::vec3 intersectionPoint = camera_pos + t * ray_direction;
				intersection = intersectionPoint;
				// fmt::print("{},{},{}   ",intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
				if (t >= 0) ret = p.col;
			}
		}

		//https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf
		// Try this for getting coordinate of point it hits on sphere
		for (Sphere c : g.spheres) {
			glm::vec3 intersectionPoint;
			float discriminant = hit_sphere(c.centre, c.r, camera_pos, ray_direction, intersectionPoint);
			if (discriminant >= 0) {
				intersection = intersectionPoint;
				// fmt::print("{}:",discriminant);
				if (glm::distance(camera_pos, c.centre) < dist_) {
					ret = c.col;
					dist_ = glm::distance(camera_pos, c.centre);
					closeness_ = discriminant;
					normal = glm::normalize(intersection - c.centre);
					sphere_hit = c;
					// fmt::print("{}:",(int)closeness_);
				}
			}
		}
		return ret;
	}

	void Render() {
		Timer timer;
		int mindim = glm::min(m_VPHeight, m_VPWidth);
		delete[] m_ImageData;
		if (!m_Image || m_VPHeight != m_Image->GetHeight() || m_VPWidth != m_Image->GetWidth()) {
			m_Image = std::make_shared<Image>(mindim, mindim, ImageFormat::RGBA);
			m_ImageData = new uint32_t[mindim * mindim];
		}

		//for (uint32_t i = 0; i < m_VPWidth * m_VPHeight; i++) {
		//	m_ImageData[i] = 0xFF00FFFF; // ABGR
		//}
		RenderScene();

		m_Image->SetData(m_ImageData);
		m_LastRenderTime = timer.ElapsedMillis();
	}

	void RenderPixel(glm::vec2 xy) {
		int H = m_Image->GetHeight();
		int W = m_Image->GetWidth();
		int x = xy.x;
		int y = xy.y;

		glm::vec3 light_pos = g.light.pos;

		float f = 1.0;

		float a = (float(x) / float(H) - 0.5);
		float b = (float(y) / float(W) - 0.5);

		glm::vec3 pixel = glm::vec3(a, -b, -f);
		glm::vec3 ray_direction = camera_orientation * (pixel);

		fmt::rgb c = fmt::color::black;

		glm::vec3 intersectionPoint = glm::vec3(0, 0, 0);

		glm::vec3 normal;
		Sphere spherehit = Sphere(0, glm::vec3(0, 0, 0), fmt::rgb(0, 0, 0));
		c = castRay(camera_pos, ray_direction, light_pos, g, intersectionPoint, normal, spherehit);

		float light_intensity = 1; // Lighting disabled as it doesnt work properly

		if (Equal(c, g.planes[1].col)) {
			glm::vec3 newpoint;
			fmt::rgb newcol = castRay(intersectionPoint, -(intersectionPoint - light_pos), light_pos, g, newpoint, normal);
			fmt::rgb checkercolor = c;
			if ((int)(intersectionPoint.z / 10 - 20) % 2 == (int)(intersectionPoint.y / 10 - 20) % 2) checkercolor = fmt::color::light_gray;

			// Shadows
			if (!Equal(c, newcol)) c = fmt::rgb(checkercolor.r / 4, checkercolor.g / 4, checkercolor.b / 4);
			else c = checkercolor;
		}
		else if (!Equal(c, g.planes[0].col)) {
			// https://www.geogebra.org/3d?lang=en use this for help?
			if (Equal(c, g.spheres[0].col)) { // Mirror test - acts more like glass at the moment if removing the 2.0f*
				c = fmt::rgb(255 * (normal.x + 1) / 2, 255 * (normal.y + 1) / 2, 255 * (normal.z + 1) / 2);

				glm::vec3 R_i = glm::normalize(ray_direction);
				glm::vec3 R_r = glm::normalize(R_i - 2.0f * glm::dot(R_i, normal) * normal);

				glm::vec3 point = intersectionPoint + 0.1f * normal;

				glm::vec3 newhit;
				Sphere newhitsphere = Sphere(0, glm::vec3(0, 0, 0), fmt::rgb(0, 0, 0));
				glm::vec3 newnormal;

				c = castRay(point, R_r, light_pos, g, newhit, normal, newhitsphere);
				if (Equal(c, g.planes[1].col) && (int)(newhit.z / 10 - 20) % 2 == (int)(newhit.y / 10 - 20) % 2) c = fmt::color::light_gray;
				else if (Equal(c, g.spheres[4].col)) {
					c = fmt::rgb(255 * (normal.x + 1) / 2, 255 * (normal.y + 1) / 2, 255 * (normal.z + 1) / 2);
				}
				else if (newhitsphere.r != 0) {
					glm::vec3 light_direction = glm::normalize(light_pos - newhit);
					float intensity = glm::dot(newnormal, light_direction);
					light_intensity = glm::clamp(intensity, 0.7f, 1.0f);
				}
			}
			else if (Equal(c, g.spheres[4].col)) {
				c = fmt::rgb(255 * (normal.x + 1) / 2, 255 * (normal.y + 1) / 2, 255 * (normal.z + 1) / 2);
			}
			else {
				glm::vec3 light_direction = glm::normalize(light_pos - intersectionPoint);
				float intensity = glm::dot(normal, light_direction);
				light_intensity = glm::clamp(intensity, 0.5f, 1.0f);
			}
		}

		c.b = glm::clamp(c.b * light_intensity, 0.0f, 255.0f);
		c.g = glm::clamp(c.g * light_intensity, 0.0f, 255.0f);
		c.r = glm::clamp(c.r * light_intensity, 0.0f, 255.0f);

		int r_ = c.r;
		int g_ = c.g;
		int b_ = c.b;
		m_ImageData[y + x * H] = 0xFF000000 + (b_ << 16) + (g_ << 8) + r_;//AABBGGRR
	}

	void RenderScene() {
		int H = m_Image->GetHeight();
		int W = m_Image->GetWidth();
		glm::vec3 light_pos = g.light.pos;
		float f = 1.0;

		// Maybe parallelise, or loop to move to CUDA kernel or Vulkan GPU stuff
		// https://stackoverflow.com/questions/36246300/parallel-loops-in-c
		/*for (int x = 0; x < H; x++) {
			for (int y = 0; y < W; y++) {
				RenderPixel(glm::vec2(x, y));
			}
		}*/

		// Build vector of pixels to check 
		// Can maybe build this vector on scene initialisation rather than on render, but oh well
		std::vector<glm::vec2> xys;
		for (int x = 0; x < H; x++) {
			for (int y = 0; y < W; y++) {
				xys.push_back(glm::vec2(x, y));
			}
		}

		// Parallelised for loop to render each pixel
		// Pixel colour is written to a buffer array by index so order doesn't matter, and no race conditions / overwrite problems
		std::for_each(std::execution::par, xys.begin(), xys.end(),
			[&](glm::vec2 item) {
				RenderPixel(item);
			});
	}

	int getrand(int min, int max) {
		int range = max - min;
		int r = rand() % range;
		return r + min;
	}

	Geometry GenerateRandomGeometry(int numSmallSpheres, int numBigSpheres, int leftBoundary, int rightBoundary, int upBoundary, int downBoundary, int nearBoundary, int farBoundary) {
		Light l = Light(glm::vec3(10, 15, -25), fmt::color::white);
		g = Geometry("random", fmt::color::white, l);

		srand(time(NULL));

		for (int i = 0; i < numBigSpheres; i++) {
			// Generate Big Spheres
			Sphere s = Sphere(getrand(3, 4), glm::vec3(getrand(upBoundary, downBoundary), getrand(leftBoundary, rightBoundary), getrand(nearBoundary, farBoundary)), fmt::rgb(rand() % 255, rand() % 255, rand() % 255));
			g.spheres.push_back(s);
		}

		for (int i = 0; i < numSmallSpheres; i++) {
			// Generate Small Spheres
			Sphere s = Sphere(getrand(1,2), glm::vec3(getrand(upBoundary, downBoundary), getrand(leftBoundary, rightBoundary), getrand(nearBoundary, farBoundary)), fmt::rgb(rand() % 255, rand() % 255, rand() % 255));
			g.spheres.push_back(s);
		}

		Plane wall = Plane("wall", glm::vec3(0, 0, -1), -10, fmt::color::cyan);
		g.planes.push_back(wall);
		Plane floor = Plane("floor", glm::vec3(-1, 0, 0), -22, fmt::color::gray);
		g.planes.push_back(floor);

		return g;
	}

	// TODO: Add button to generate random geometry
	void InitialiseGeometry() {
		g = Geometry("test", fmt::color::cyan, l);
		g.light = l;

		g.spheres.push_back(Sphere(2, glm::vec3(20, 20, -20), fmt::color::blue));
		g.spheres.push_back(Sphere(2, glm::vec3(15, 15, -20), fmt::color::lime));
		g.spheres.push_back(Sphere(2, glm::vec3(10, 10, -22), fmt::color::red));
		g.spheres.push_back(Sphere(2, glm::vec3(20, 20, -10), fmt::color::orange));
		g.spheres.push_back(Sphere(2, glm::vec3(20, 10, -10), fmt::color::black));

		g.planes.push_back(Plane("wall", glm::vec3(0, 0, -1), -10, fmt::color::cyan));
		g.planes.push_back(Plane("floor", glm::vec3(-1, 0, 0), -22, fmt::color::gray));
	}

private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_VPWidth = 0, m_VPHeight = 0;

	float m_LastRenderTime = 0.0f;

	Light l = Light(glm::vec3(10, 15, -17), fmt::color::white);
	Geometry g = Geometry("test", fmt::color::cyan, l);

	glm::vec3 camera_pos = glm::vec3(15, 15, 2.0); // define camera position
	glm::mat3 camera_orientation = glm::mat3(1.0); // define camera orientation
};

class TextItem {
public:
	std::string content;
	ImVec4 colour;
	bool newline;

	TextItem(std::string _content, ImVec4 _colour, bool _newline) : content(_content), colour(_colour), newline(_newline) {}
	TextItem(std::string _content, bool _newline) : content(_content), colour(ImVec4(255,255,255,255)), newline(_newline) {}
};

class TextLine {
public:
	std::vector<TextItem> parts;

	void AddItem(TextItem t) {
		parts.push_back(t);
	}

	TextLine(){};
};

class TextTest : public Walnut::Layer {
public:
	void InitialiseBuffers() {
		writeBuffer.clear();
		screenBuffer.clear();

		for (int i = 0; i < maxIndex; i++) {
			writeBuffer.push_back(fmt::format("{}", i));
		}

		for (int i = 0; i < bufferSize; i++) {
			screenBuffer.push_back(writeBuffer.at(i));
		}

		maxitemindex = bufferSize - 1;
	}

	void InitialiseEventBuffers() {
		eventBuffer.clear();
		eventScreenBuffer.clear();

		TextLine t = TextLine();
		t.AddItem(TextItem("RED CARD! ", GetColour(255, 0, 0), false));
		t.AddItem(TextItem("Player has been sent off.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t);

		TextLine t1 = TextLine();
		t1.AddItem(TextItem("YELLOW CARD! ", GetColour(255, 255, 0), false));
		t1.AddItem(TextItem("Player has been booked.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t1);

		TextLine t2 = TextLine();
		t2.AddItem(TextItem("GOAL! Player scored a goal.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t2);

		TextLine t3 = TextLine();
		t3.AddItem(TextItem("RED! ", GetColour(255, 0, 0), false));
		t3.AddItem(TextItem("GREEN! ", GetColour(0, 255, 0), false));
		t3.AddItem(TextItem("BLUE! ", GetColour(0, 0, 255), false));
		t3.AddItem(TextItem("DEFAULT. ", true));
		eventBuffer.push_back(t3);

		for (int i = 0; i < bufferSize; i++) {
			eventScreenBuffer.push_back(eventBuffer.at(i));
		}
	}

	virtual void OnAttach() override {
		InitialiseEventBuffers();
	}

	ImVec4 GetColour(int r, int g, int b) {
		return ImVec4(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255), 255);
	}

	void RenderText(TextItem t) {
		ImGui::TextColored(t.colour, t.content.c_str());
		if (!t.newline) ImGui::SameLine(0, 0);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		//if (maxitemindex != maxIndex-1) {
		//	if (ImGui::Button("Next")) {
		//		maxitemindex++;
		//		screenBuffer.erase(screenBuffer.begin());
		//		//screenBuffer.pop_back();
		//		screenBuffer.push_back(writeBuffer.at(maxitemindex));
		//	}
		//}
		//else if (ImGui::Button("Reset")) {
		//	InitialiseEventBuffers();
		//}

		for (int i = 0; i < bufferSize; i++) {
			//ImGui::Text(screenBuffer.at(i).c_str());
			TextLine t = eventScreenBuffer.at(i);
			for (TextItem ti : t.parts) RenderText(ti);
		}

		ImGui::End();
	}
private:
	const int maxIndex = 4;
	const int bufferSize = 4;
	int maxitemindex = 0;
	std::vector<std::string> writeBuffer;
	std::vector<std::string> screenBuffer;

	std::vector<TextLine> eventBuffer;
	std::vector<TextLine> eventScreenBuffer;
};

enum WindowState
{
	MainMenu,
	Debug1,
	Debug2,
	Debug3,
	Options
};

class Player {
public:
	ImVec2 position;
	int number;
	char* name;

	Player(ImVec2 _p, int _num, char* _name) : position(_p), number(_num), name(_name) {};
};

class FootballUITest : public Walnut::Layer
{
public:
	ImVec2 GetDesktopResolution()
	{
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(0, 0);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		int w = desktop.right;
		int h = desktop.bottom;
		return ImVec2((float)w, (float)h);
	}

	virtual void OnAttach() override {
		// Demonstrate the various window flags. Typically you would just use the default!
		/*static bool no_titlebar = true;
		static bool no_scrollbar = false;
		static bool no_menu = true;
		static bool no_move = true;
		static bool no_resize = false;
		static bool no_collapse = true;
		static bool no_close = false;
		static bool no_nav = true;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool no_docking = true;
		static bool unsaved_document = false;*/

		static bool no_titlebar = false;
		static bool no_scrollbar = false;
		static bool no_menu = false;
		static bool no_move = false;
		static bool no_resize = false;
		static bool no_collapse = true;
		static bool no_close = true;
		static bool no_nav = false;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool no_docking = false;
		static bool unsaved_document = false;

		if (no_titlebar)        _windowFlags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       _windowFlags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           _windowFlags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            _windowFlags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          _windowFlags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        _windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             _windowFlags |= ImGuiWindowFlags_NoNav;
		if (no_background)      _windowFlags |= ImGuiWindowFlags_NoBackground;
		if (no_bring_to_front)  _windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (no_docking)         _windowFlags |= ImGuiWindowFlags_NoDocking;
		if (unsaved_document)   _windowFlags |= ImGuiWindowFlags_UnsavedDocument;
		if (no_close)           _pOpen = NULL; // Don't pass our bool* to Begin

		/*const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);*/

		//ImGui::SetWindowSize(ImVec2(1920, 1080));
	}

	// TODO: Fix options so window can be resized
	virtual void OnUIRender() override
	{
		char* name = "_badFootballSimulator (C++ Edition)";
		//ImGui::Begin("Hello");

		ImVec2 ws = ImGui::GetWindowSize();

		if (_windowState == WindowState::MainMenu) {
			ImGui::Begin("Main Window", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x/2-textSize.x/2, ThisRegionSize.y/3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("MAIN MENU");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)ws.x, (int)ws.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;
			/*ImGui::SliderFloat("Spacing 1", &spacing1, -10, 10);
			ImGui::SliderFloat("Spacing 2", &spacing2, -10, 10);*/

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x/4 -50,2.0f*ThisRegionSize.y/3));
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(2*ThisRegionSize.x / 4 -50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100,30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::SetCursorPos(ImVec2(3*ThisRegionSize.x / 4 -50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.5f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Options", ImVec2(100, 30))) {
				_windowState = WindowState::Options;
			}
			/*ImGui::SameLine(spacing1, spacing2);
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}*/
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug1) {
			ImGui::Begin("Simulate Single Game", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			static float GKLineHeight = 7; // ?? to ??
			static float DefensiveLineHeight = 20; // ?? to ??
			static float MidfieldLineHeight = 37; // ?? to ??
			static float AttackingLineHeight = 50; // ?? to ??

			static float WingerWidth = 35; // 0 to 45
			static float FBWidth = 35; // 0 to 45
			static float CMWidth = 10; // 0 to 45
			static float CBWidth = 10; // 0 to 45

			static float FullBackHeightOffset = 0;
			static float DMHeightOffset = -5;
			static float WingerHeightOffset = -3;

			static float MarkerSize = 10;

			static bool showNumbers = true;

			ImGui::Begin("Settings");

			ImGui::SliderFloat("Goalkeeper Height", &GKLineHeight, 0, 20);
			ImGui::SliderFloat("Defensive Line Height", &DefensiveLineHeight, 10, 30);
			ImGui::SliderFloat("Midfield Line Height", &MidfieldLineHeight, 20, 40);
			ImGui::SliderFloat("Attacking Line Height", &AttackingLineHeight, 40, 60);
			ImGui::NewLine();

			ImGui::SliderFloat("Full Back Width", &FBWidth, 0, 45);
			ImGui::SliderFloat("Winger Width", &MidfieldLineHeight, 0, 45);
			ImGui::SliderFloat("Centre Back Width", &CBWidth, 0, 45);
			ImGui::SliderFloat("Centre Mid Width", &CMWidth, 0, 45);
			ImGui::NewLine();

			ImGui::SliderFloat("Full Back Height Offset", &FullBackHeightOffset, -10, 10);
			ImGui::SliderFloat("Defensive Mid Height Offset", &DMHeightOffset, -10, 10);
			ImGui::SliderFloat("Winger Height Offset", &WingerHeightOffset, -10, 10);
			ImGui::NewLine();

			ImGui::SliderFloat("Marker Size", &MarkerSize, 5, 15);

			ImGui::Checkbox("Show Numbers", &showNumbers);

			if (ImGui::Button("Reset")) {
				GKLineHeight = 7; // ?? to ??
				DefensiveLineHeight = 20; // ?? to ??
				MidfieldLineHeight = 37; // ?? to ??
				AttackingLineHeight = 50; // ?? to ??

				WingerWidth = 35; // 0 to 45
				FBWidth = 35; // 0 to 45
				CMWidth = 10; // 0 to 45
				CBWidth = 10; // 0 to 45

				FullBackHeightOffset = 0;
				DMHeightOffset = -5;
				WingerHeightOffset = -3;
			}

			ImGui::End();

			ImGui::Begin("Pitch");

			ImVec2 WindowPos = ImGui::GetWindowPos();

			ImVec2 WindowSize = ImGui::GetWindowSize();

			int ShortestDim = std::min(WindowSize.x, WindowSize.y);

			// Dimensions:
			// Outline: 120x90 (metres, WxH)
			// Centre Circle: Radius 9.15m
			// Penalty Area: 16.5x40.3m
			// 6yd Box: 5.5x18.32m
			// Goal Width: 7.32m
			// Penalty Spot: 11m from byline
			// Corner segment: Radius 1m

			ImVec2 Pitch = ImVec2(120, 90);
			ImVec2 PitchToWindow = ImVec2(WindowSize.x / Pitch.x, WindowSize.y / Pitch.y);

			//int markerSize = ShortestDim/50;
			float markerSize = MarkerSize;
			auto draw = ImGui::GetForegroundDrawList();
			ImU32 team1Col = IM_COL32(255, 0, 0, 255);
			ImU32 team2Col = IM_COL32(3, 202, 252, 255);
			ImU32 lineCol = IM_COL32(255, 255, 255, 255);
			ImU32 grassCol = IM_COL32(23, 141, 23, 255);
			ImVec2 Offset = ImVec2(WindowPos.x + 5, WindowPos.y + 30);

			int xOffset = (int)Offset.x;
			int yOffset = (int)Offset.y;

			/*ImVec2 TopLeft = ImVec2(Offset.x, Offset.y);
			ImVec2 TopRight = ImVec2(Offset.x + WindowSize.x - 10, Offset.y);
			ImVec2 BottomLeft = ImVec2(Offset.x, Offset.y + WindowSize.y - 35);
			ImVec2 BottomRight = ImVec2(Offset.x + WindowSize.x - 10, Offset.y + WindowSize.y - 35);*/

			int TopEdge = Offset.y;
			int BottomEdge = Offset.y + WindowSize.y - 35;
			int LeftEdge = Offset.x;
			int RightEdge = Offset.x + WindowSize.x - 10;

			ImVec2 TopLeft = ImVec2(LeftEdge, TopEdge);
			ImVec2 TopRight = ImVec2(RightEdge, TopEdge);
			ImVec2 BottomLeft = ImVec2(LeftEdge, BottomEdge);
			ImVec2 BottomRight = ImVec2(RightEdge, BottomEdge);

			ImVec2 PitchSize = ImVec2(RightEdge - LeftEdge, BottomEdge - TopEdge);

			int localHalfway = (LeftEdge + RightEdge) / 2;

			ImVec2 Centre = ImVec2((LeftEdge + RightEdge) / 2, (TopEdge + BottomEdge) / 2);

			/*ImVec2 TopLeft = ImVec2(xOffset - 15, yOffset - 15);
			ImVec2 TopRight = ImVec2(xOffset + 2 * localHalfway + 15, yOffset - 15);
			ImVec2 BottomLeft = ImVec2(xOffset - 15, yOffset + 105);
			ImVec2 BottomRight = ImVec2(xOffset + 2 * localHalfway + 15, yOffset + 105);*/

			draw->AddRectFilled(TopLeft, BottomRight, grassCol); // Grass
			draw->AddRect(TopLeft, BottomRight, lineCol); // Outline
			draw->AddLine(ImVec2(localHalfway, TopEdge), ImVec2(localHalfway, BottomEdge), lineCol); // Halfway Line
			draw->AddCircle(Centre, (9.15/120)*PitchSize.x, lineCol); // Centre Circle
			draw->AddCircleFilled(Centre, 3, lineCol); // Centre Dot

			draw->AddRect(ImVec2(LeftEdge, Centre.y - (20.15/90) * PitchSize.y), ImVec2(LeftEdge + (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Left Penalty Area
			draw->AddRect(ImVec2(RightEdge-1, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(RightEdge - (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Right Penalty Area

			draw->AddRect(ImVec2(LeftEdge, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(LeftEdge + (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Left 6yd Area
			draw->AddRect(ImVec2(RightEdge-1, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(RightEdge - (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Right 6yd Area

			draw->AddCircleFilled(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Left Penalty Spot
			draw->AddCircleFilled(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Right Penalty Spot

			// Left D
			draw->PathArcTo(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120)* PitchSize.x, -0.94, 0.94);
			draw->PathStroke(lineCol);

			// Right D
			draw->PathArcTo(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120)* PitchSize.x, 3.14159 - 0.94, 3.14159 + 0.94);
			draw->PathStroke(lineCol);

			// Top Left Corner
			draw->PathArcTo(TopLeft, (1.0 / 120)* PitchSize.x, 0, 3.14159/2);
			draw->PathStroke(lineCol);

			// Top Right Corner
			draw->PathArcTo(TopRight, (1.0 / 120)* PitchSize.x, 3.14159/2, 3.14159);
			draw->PathStroke(lineCol);

			// Bottom Left Corner
			draw->PathArcTo(BottomLeft, (1.0 / 120)* PitchSize.x, 0, -3.14159 / 2);
			draw->PathStroke(lineCol);

			// Bottom Right Corner
			draw->PathArcTo(BottomRight, (1.0 / 120)* PitchSize.x, 3.14159, 3*3.14159 / 2);
			draw->PathStroke(lineCol);

			// Players

			// TODO: LIsts of player locations
			// Have a player object, as well as a location (relative to the 120x90 pitch, which will be translated to screen space)
			// Could also contain some information on momentum / inertia for movement
			// For testing now though just use ImVec2 for pure positions and testing

			// Add players to Team 1
			// Players have a position, number and name
			std::vector<Player> team1Players;

			team1Players.push_back(Player(ImVec2(GKLineHeight, Pitch.y/2), 1, "David De Gea")); // GK

			team1Players.push_back(Player(ImVec2(DefensiveLineHeight+FullBackHeightOffset, (Pitch.y / 2) - FBWidth), 23, "Luke Shaw")); // LB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 5, "Harry Maguire")); // LCB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 19, "Raphael Varane")); // RCB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight + FullBackHeightOffset, (Pitch.y / 2) + FBWidth), 20, "Diogo Dalot")); // RB

			team1Players.push_back(Player(ImVec2(MidfieldLineHeight, (Pitch.y / 2)-CMWidth), 8, "Bruno Fernandes")); // LCM
			team1Players.push_back(Player(ImVec2(MidfieldLineHeight+DMHeightOffset, (Pitch.y / 2)), 6, "Frenkie de Jong")); // CDM
			team1Players.push_back(Player(ImVec2(MidfieldLineHeight, (Pitch.y / 2)+CMWidth), 17, "Fred")); // RCM

			team1Players.push_back(Player(ImVec2(AttackingLineHeight+WingerHeightOffset, (Pitch.y / 2)-WingerWidth), 25, "Jadon Sancho")); // LW
			team1Players.push_back(Player(ImVec2(AttackingLineHeight, (Pitch.y / 2)), 7, "Cristiano Ronaldo")); // ST
			team1Players.push_back(Player(ImVec2(AttackingLineHeight + WingerHeightOffset, (Pitch.y / 2)+WingerWidth), 11, "Antony")); // RW

			//for (Player p : team1Players) draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x/Pitch.x)*PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team1Col);

			ImFont *font = ImGui::GetFont(); // Get Font

			// Iterate over players in Team 1
			for (Player &p : team1Players) {
				ImVec2 pos = p.position; // Get position part
				draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team1Col); // Draw marker for player

				// Draw player number on marker, centering on marker
				float fontSize = 2 * MarkerSize;
				auto TextSize = ImGui::CalcTextSize(std::to_string(p.number).c_str());
				TextSize.x *= (fontSize / 20);
				TextSize.y *= (fontSize / 20);
				if (showNumbers) draw->AddText(font, fontSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x * 0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y * 0.5f), lineCol, std::to_string(p.number).c_str());
			}

			std::vector<Player> team2Players;

			team2Players.push_back(Player(ImVec2(Pitch.x - GKLineHeight, Pitch.y / 2), 31, "Ederson")); // GK

			team2Players.push_back(Player(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) - FBWidth), 27, "Joao Cancelo")); // LB
			team2Players.push_back(Player(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 14, "Aymeric Laporte")); // LCB
			team2Players.push_back(Player(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 5, "Ruben Dias")); // RCB
			team2Players.push_back(Player(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) + FBWidth), 2, "Kyle Walker")); // RB

			team2Players.push_back(Player(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) - CMWidth), 17, "Kevin De Bruyne")); // LCM
			team2Players.push_back(Player(ImVec2(Pitch.x - (MidfieldLineHeight + DMHeightOffset), (Pitch.y / 2)), 16, "Rodri")); // CDM
			team2Players.push_back(Player(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) + CMWidth), 20, "Bernardo Silva")); // RCM

			team2Players.push_back(Player(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) - WingerWidth), 47, "Phil Foden")); // LW
			team2Players.push_back(Player(ImVec2(Pitch.x - AttackingLineHeight, (Pitch.y / 2)), 9, "Erling Haaland")); // ST
			team2Players.push_back(Player(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) + WingerWidth), 26, "Riyad Mahrez")); // RW

			// Iterate over players in Team 2
			for (Player& p : team2Players) {
				ImVec2 pos = p.position; // Get position part
				draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team2Col); // Draw marker for player

				// Draw player number on marker, centering on marker
				float fontSize = 2 * MarkerSize;
				auto TextSize = ImGui::CalcTextSize(std::to_string(p.number).c_str());
				TextSize.x *= (fontSize / 20);
				TextSize.y *= (fontSize / 20);
				if (showNumbers) draw->AddText(font, 2*MarkerSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x*0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y*0.5f), lineCol, std::to_string(p.number).c_str());
			}

			// Do name hover text at the very end so that markers are never rendered on top of the text
			// If the user hovers over the player on the screen, display their name
			for (Player& p : team1Players) {
				if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x - markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y - markerSize),
					ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x + markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y + markerSize))) { // Check if mouse is in bounding box around player marker
					draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineCol, p.name); // Draw text above the cursor
				}
			}
			for (Player& p : team2Players) {
				if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x - markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y - markerSize),
					ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x + markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y + markerSize))) { // Check if mouse is in bounding box around player marker
					draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineCol, p.name); // Draw text above the cursor
				}
			}

			ImGui::End();

			// Dimensions:
			// Outline: 120x90 (metres, WxH)
			// Centre Circle: Radius 9.15m
			// Penalty Area: 16.5x40.3m
			// 6yd Box: 5.5x18.32m
			// Goal Width: 7.32m
			// Penalty Spot: 11m from byline
			// Corner segment: Radius 1m

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Simulate Single Game");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug2) {
			ImGui::Begin("Simulate Full Season", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Simulate Full Season");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug3){
			ImGui::Begin("Debug", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Debug");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Options) {
			ImGui::Begin("Options", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);

			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Options");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.5f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Back", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}

			static int x = (int)_windowSize.x;
			static int y = (int)_windowSize.y;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 500, 2.0f * ThisRegionSize.y / 3));

			//ImVec2 ScreenResolution = GetDesktopResolution();
			ImGui::SliderInt("Window Width", &x, 160, 2560);
			if (ImGui::IsItemEdited()) _windowSize.x = (float)x;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 500, 2.2f * ThisRegionSize.y / 3));
			ImGui::SliderInt("Window Height", &y, 90, 1440);
			if (ImGui::IsItemEdited()) _windowSize.y = (float)y;

			ImGui::End();
		}

		//else if (_windowState == WindowState::Debug3) {
		//	ImGui::Begin("Debug 2", p_open, window_flags);

		//	ImGui::SetWindowFontScale(2.0f);
		//	ImGui::Text("DEBUG 3");
		//	ImGui::SetWindowFontScale(1.0f);

		//	static float spacing1 = 0.0f;
		//	static float spacing2 = 15.0f;
		//	/*ImGui::SliderFloat("Spacing 1", &spacing1, -10, 10);
		//	ImGui::SliderFloat("Spacing 2", &spacing2, -10, 10);*/

		//	if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
		//		_windowState = WindowState::MainMenu;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug1;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug2;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug3;
		//	}
		//	/*ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("New Window")) {
		//		ImGui::BeginPopupContextWindow("Testing");
		//		ImGui::Begin("New Window");
		//		ImGui::End();
		//	}*/
		//	ImGui::End();
		//}

		//ImGui::ShowDemoWindow();
	}
private:
	WindowState _windowState = WindowState::MainMenu;
	ImGuiWindowFlags _windowFlags = 0;
	ImVec2 _windowSize = ImVec2(1600, 900);
	bool* _pOpen = (bool*)true;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	//app->PushLayer<TextTest>();
	app->PushLayer<FootballUITest>();

	/*app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});*/
	return app;
}
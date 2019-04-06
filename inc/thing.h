#ifndef THING_H
#define THING_H

#include <geometry.h>
#include <time.h>

class Thing
{
  public:
	//   Thing() {}

	void add_geometry(Geometry geometry)
	{
		geometries.push_back(geometry);
	}

	void generate_mesh()
	{
		std::vector<glm::vec3> vertex_data;

		for (unsigned int i = 0; i < geometries.size(); i++)
		{
			// vertex_data.push_back(geometries.at(i).vertex_coordinates_mesh);
		}
	}

	void update_shader()
	{
		shader.set_mat4("model", model);
		shader.set_vec4("color", color);
		shader.set_mat4("view", camera.get_view());
		shader.set_mat4("aspect", camera.get_aspect());
		shader.set_mat4("proj", camera.get_proj());

		shader.update_time(tiktok.get());

		// send light info to shader
		// if (light_ID == -1)
		// 	lighting.update_shader(shader);

		shader.use();
	}

	/*** VARIABLE ACCESS ***/

	void set_shader(Shader shader_in)
	{
		shader = shader_in;
	}

	// void set_light_source()
	// {
	// 	light_ID = lighting.add_source(1);
	// }

	void set_model(glm::mat4 model_temp)
	{
		model = model_temp;

		// update light (if applicable)
		// if (light_ID != -1)
		//     lighting.set_position(light_ID, glm::vec3(model_temp[3][0], model_temp[3][1], model_temp[3][2]));
	}

	void set_color(glm::vec4 color_temp)
	{
		color = color_temp;

		// update light (if applicable)
		// if (light_ID != -1)
		// 	lighting.set_color(light_ID, color_temp);
	}

  private:
	glm::mat4 model = glm::mat4(0.0f);
	glm::vec4 color = glm::vec4(0.0f);

	Shader shader;
	int light_ID = -1;

	std::vector<Geometry> geometries;
};

#endif
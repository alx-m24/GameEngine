#include "Object.hpp"

void Objects::draw(Shader& shader)
{
	shader.setFloat("material.shininess", shininess);

	if (diffuse != 0) {
		shader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}
	if (specular != 0) {
		shader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	for (Transformations& object : *this) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, object.position);

		glm::vec3 rot = {
			object.rotation.x,
			object.rotation.y,
			object.rotation.z
		};
		model = glm::rotate(model, glm::radians(object.rotation.w), rot);

		model = glm::scale(model, object.scale);

		shader.setMat4("model", model);
		if (useColor) shader.setVec3("material.color", object.color);
		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
	}
}

#include "models.h"

models::models(char* filePath, cwc::glShader* shader, GLuint texture) : myShader(shader), myTexture(texture) {
	this->myModel = glmReadOBJ(filePath);
	if (!this->myModel)
		exit(0);

	glmUnitize(this->myModel);
	glmFacetNormals(this->myModel);
	glmVertexNormals(this->myModel, 90.0);

}

void models::draw(float x, float y, float z) {
	if (this->myShader) this->myShader->begin();
	glPushMatrix();

	glTranslatef(x, y, z);

	// revisar esto
	if (this->myTexture != -1) glBindTexture(GL_TEXTURE_2D, this->myTexture);

	glmDraw(this->myModel, GLM_SMOOTH | GLM_MATERIAL | ((this->myTexture != -1) ? GLM_TEXTURE : 0));

	glPopMatrix();
	if (this->myShader) this->myShader->end();
}

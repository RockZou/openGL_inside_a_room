#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);

	void Bind();
	void Load(const std::string& fileName);
	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	GLuint m_texture;
};

#endif

#ifndef Example_Texture_h
#define Example_Texture_h

#include "GLIncludes.h"
#include <string>
#include <glm/vec2.hpp>

namespace cgCourse
{
	class Texture
	{
	public:
		Texture();

        void bind() const;
        void unbind() const;

    void loadFromFile(const std::string & filename,bool _SRGB=false);
		void setTextureParameters();
		GLuint getTexHandle() const;

	private:
		GLuint texhandle;

		// Texture info
		glm::vec2 size;
		uint8_t bitsPerPixel;
		

	};
}

#endif

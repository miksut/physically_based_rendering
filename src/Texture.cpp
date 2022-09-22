#include "Texture.h"
#include <FreeImage.h>
#include <iostream>
#ifdef __APPLE__
#include <FreeImagePlus.h>
#endif

namespace cgCourse
{
	Texture::Texture(): texhandle(0)
	{
	}

  void Texture::loadFromFile(const std::string& _filename,bool _SRGB)
    {
		// .tif images have to be loaded with freeimage
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(_filename.c_str(), 0);
		FIBITMAP* rawimage = FreeImage_Load(imageFormat, _filename.c_str());
		this->size = glm::vec2(FreeImage_GetWidth(rawimage),
							   FreeImage_GetHeight(rawimage));
		this->bitsPerPixel = FreeImage_GetBPP(rawimage);
		FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(rawimage);
        
        unsigned char* data = nullptr;
        
        if (bitsPerPixel != 32){
            FIBITMAP* image = FreeImage_ConvertTo32Bits(rawimage);
            if (!image) {
                std::cout << "Freeimage loading: go mad here" << std::endl;
            }
            FREE_IMAGE_COLOR_TYPE newcolorType = FreeImage_GetColorType(rawimage);
            data = new unsigned char[int(this->size.x * this->size.y * 4)];
            BYTE* rawArray = FreeImage_GetBits(image);
            memcpy(data, (BYTE*)FreeImage_GetBits(image), this->size.x * this->size.y * 4);
            
        }else{
            data = new unsigned char[int(this->size.x * this->size.y * 4)];
            memcpy(data, (BYTE*)FreeImage_GetBits(rawimage), this->size.x * this->size.y * 4);
        }
		glGenTextures(1, &this->texhandle);
		glBindTexture(GL_TEXTURE_2D, this->texhandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (_SRGB){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8,
			this->size.x, this->size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
      }else{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      this->size.x, this->size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    }
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		// this function releases the memory previously occupied by freeimage for
		// image loading
		FreeImage_Unload(rawimage);
		// after copying the data to the GPU we can also delete data again.
		delete[] data;
    }

	void Texture::setTextureParameters() {

	}

	GLuint Texture::getTexHandle() const
	{
		return this->texhandle;
	}

	void Texture::bind() const {
		if(texhandle != 0) {
			glBindTexture(GL_TEXTURE_2D, texhandle);
		}
    }
    
    void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
    }
}


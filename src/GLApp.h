#ifndef Example_GLApp_h
#define Example_GLApp_h

#include "GLIncludes.h"
#include <glm/vec2.hpp>
#include <string>

namespace cgCourse
{
	class GLApp
	{
	public:
		GLApp(glm::uvec2 _windowSize, std::string _title, std::string _exepath, bool resizeable = true);
		virtual ~GLApp();

		bool run();

        static std::string getPathToExecutable();
	protected:
		void stop();

		// Virtual methods for initialization, rendering and termination
		virtual bool init() = 0;
		virtual bool update(double dt) = 0;
        virtual bool render() = 0;
		virtual bool end() = 0;

		// Utility methods
        glm::uvec2 getFramebufferSize() const;
        glm::uvec2 getWindowSize() const;
		bool checkGlError(const char* message) const;
        void setTitle(std::string _title);

		GLFWwindow* window_;
		std::string const title;
        static std::string pathToExecutable;
        double previousTime = 0;
        double previousFPSTime = glfwGetTime();
        int frameTick = 0 ;
	};
}


#endif

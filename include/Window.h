#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <iostream>
#include <SDL.h>
//#include <GLFunctions.h>


/// \class Window
/// \brief initializes OpenGL and SDL and creates a window

class Window
{
private:
	int m_width;
	int m_height;
	SDL_Window * m_window;
	SDL_Renderer * m_renderer;
	SDL_GLContext m_context;
	SDL_GLContext createContext();
	void glInit();
public:
	Window(const int _width, const int _height);
	int getWidth() const{ return this -> m_width;}
	int getHeight() const{ return this -> m_height;}
	void setWindowSize(const int &_width, const int &_height);
	SDL_Window * getWindow() const{ return m_window;}
	SDL_Renderer * getRenderer() const { return m_renderer;}
};
#endif // WINDOW_H

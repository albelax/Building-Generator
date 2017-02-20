#include "Window.h"

Window::Window(const int _width, const int _height)
{
	m_width = _width;
	m_height = _height;
	SDL_Init(SDL_INIT_TIMER);
	m_window = SDL_CreateWindow("pippo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (m_window == 0)
	{
		printf("%s\n",SDL_GetError());
	}

	this ->m_renderer = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == 0)
	{
		m_renderer = SDL_GetRenderer(m_window);
		printf("%s\n",SDL_GetError());
	}

	m_context = createContext();
}

//----------------------------------------------------------------------------------------------------------------------

void Window::setWindowSize(const int &_width, const int &_height)
{
	m_width = _width;
	m_height = _height;
}

//----------------------------------------------------------------------------------------------------------------------

SDL_GLContext Window::createContext()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // 4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // 1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return SDL_GL_CreateContext(m_window);
}

//----------------------------------------------------------------------------------------------------------------------

void Window::glInit()
{

}

//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <entt\entt.hpp>

class Game {
public:
	Game(const char* title, int width, int height);
	~Game();

	void handleEvents();
	void update();
	void render();

private:
	
	entt::registry mRegistry;


};
#pragma once
#include "settings.h"
#include "meteor.h"
#include "player.h"
#include "background.h"
#include "enemy.h"
#include "bonus.h"


class Game {
private:
	sf::RenderWindow window;
	std::vector<Meteor*> meteors;
	Player player;
	Enemy enemy;
	Background background;
	std::list<Bonus*> bonuses;
	int prevTime=0, nowTime;
	sf::Clock timer;
public:
	Game() {
		window.create(sf::VideoMode{ (size_t)WINDOW_WIDTH, (size_t)WINDOW_HEIGHT }, "game", sf::Style::Fullscreen);
		window.setFramerateLimit(FPS);
	}

	void checkEvents() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}
	}
	void update() {
		player.update();
		enemy.update(player.getPosition());
		for (auto& bonus : bonuses) {
			bonus->update();
		}
	}

	void checkColisions() {
		sf::FloatRect playerBounds = player.getHitBox();
		sf::FloatRect enemyBounds = enemy.getHitBox();
		if (enemyBounds.intersects(playerBounds)) {
			window.close();
		}
		for (auto& bonus : bonuses) {
			sf::FloatRect bonusBounds = bonus->getHitBox();
			if (bonusBounds.intersects(playerBounds)) {
				bonus->act(player);
				bonus->setDel();
			}
		}
	}
	void SpawnBonuses() {
		nowTime = timer.getElapsedTime().asMilliseconds();
		if (prevTime - nowTime > 10000) {
			int chance = rand() % 30000;
			int bonusType = rand() % Bonus::BonusType::MAX_BONUS_TYPE;
			if (chance < 15000) {
				Bonus* bonus = new Bonus(BonusPosition, (Bonus::BonusType)bonusType
				);
				bonuses.push_back(bonus);
			}
			prevTime = nowTime;
		}
	}

	void draw() {
		window.clear();
		window.draw(background.getSprite());
		window.draw(enemy.getSprite());
		window.draw(player.getSprite());
		for (auto& bonus : bonuses) {
			bonus->draw(window);
		}
		window.display();
	}

	void play() {
		while (window.isOpen())
		{
			checkEvents();
			update();
			checkColisions();
			draw();
		}
	}
};
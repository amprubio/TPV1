#include "PlayState.h"
#include "SDLApp.h"
#include "MenuButton.h"
#include <math.h>
#include <string>

PlayState::~PlayState()
{
}

void PlayState::init()
{
	// add scoreboard
	score_ = new ScoreBoard(Vector2D(WIN_WIDTH/2,0 ), 20, 20, app_->getTexture(TextureOrder::SCOREBOARD), app_->getTexture(TextureOrder::ARROW_2), this);
	addGameObject(score_);
	score_->setPoints(0);
	score_->setArrows(20);
	
	//add bow
	bow_ = new Bow(Vector2D(0, 0), Vector2D(0, 5), 100, 150, app_->getTexture(TextureOrder::BOW_1), app_->getTexture(TextureOrder::BOW_2), app_->getTexture(TextureOrder::ARROW_1), this);
	addGameObject(bow_);
	addEventHandler(bow_);
	
	//add n butterflies
	createButterflies(20);

}

void PlayState::render() const
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = WIN_WIDTH;
	dest.h = WIN_HEIGHT;
	app_->getTexture(TextureOrder::BACKGROUND2)->render(dest);
	GameState::render();
	score_->render(); //esto es para que se renderice encima del resto de objetos 
}

void PlayState::update()
{
	GameState::update();
	createBalloon();
	cleanMemory();
}

void PlayState::handleEvents(SDL_Event& event)
{
	if (event.type==SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			app_->toPause(app_);
		}
	}
	GameState::handleEvents(event);
}

void PlayState::saveToFile(int seed)
{
	std::ofstream file;
	file.open(std::to_string(seed) + ".usr \n");

	//guardar nivel, puntuacion y flechas

	//arco

	//flechas

	//globos

	//mariposas

	//premios



}

void PlayState::loadFromFile(int seed)
{
}


void PlayState::addReward(Reward* reward)
{
	createReward(reward);
}

void PlayState::addButterfly(int n)
{
	createButterflies(n);
}

void PlayState::addArrows(int n)
{
	score_->setArrows(score_->getArrows() + n);
}

void PlayState::addPoints(int n)
{
	score_->setPoints(score_->getPoints() + n);
}


void PlayState::createButterflies(int n)
{
	for (int i = 0; i < n; i++) {
		Butterfly* b = new Butterfly(Vector2D(120 + rand() % (WIN_WIDTH - 220), rand() % WIN_HEIGHT), Vector2D(0.15,0.15), 50, 50, app_->getTexture(TextureOrder::BUTTERFLY), this);
		addGameObject(b);
	}
}

void PlayState::createBalloon()
{
	if (rand() % 150 == 0) {
		Balloon* b = new Balloon(Vector2D(100 + rand() % (WIN_WIDTH - 150), WIN_HEIGHT), Vector2D(0, 0.5), 400, 400, true, app_->getTexture(TextureOrder::BALLOONS), this);
		addGameObject(b);
		balloons_.push_back(b);
	}
}

void PlayState::createReward(Reward* reward)
{
	addGameObject(reward);
	addEventHandler(reward);
	rewards_.push_back(reward);
}


bool PlayState::collisionWithBalloon(Balloon* balloon)
{
	bool col = false;
	auto it = arrows_.begin();

	while (it != arrows_.end() && !col) {

		SDL_Rect* dest_A = &(*it)->getCollisionRect();
		col = SDL_HasIntersection(&(balloon->getCollisionRect()),dest_A );
		if (col) {
			(*it)->setNumHits((*it)->getNumHits() + 1);
			
			int p = pow((static_cast<Arrow*>(*it))->getNumHits() - 1, 2)* POINTS_PER_BALLON;
			score_->setPoints(score_->getPoints() + p);
		}
		else {
			it++;
		}

	}
	return col;

}

bool PlayState::collisionWithButterfly(Butterfly* butterfly)
{
	bool col = false;
	auto it = arrows_.begin();

	while (it != arrows_.end() && !col) {

		SDL_Rect* dest_A = &(*it)->getCollisionRect();
		col = SDL_HasIntersection(&(butterfly->getCollisionRect()), dest_A);
		if (col) {
			score_->setPoints(score_->getPoints() - 5);

		}
		else {
			it++;
		}

	}
	return col;

}

bool PlayState::collisionWithReward(Reward* reward)
{

	bool col = false;
	auto it = arrows_.begin();

	while (it != arrows_.end() && !col) {

		SDL_Rect* dest_A = &(*it)->getCollisionRect();
		col = SDL_HasIntersection(&(reward->getCollisionRect()), dest_A);
		if (col) {
		//aqui podemos sumar puntos???
		}
		else {
			it++;
		}

	}
	return col;
}


void PlayState::shoot(Arrow* arrow)
{
	gObjects_.push_back(arrow);
	arrows_.push_back(arrow);
	score_->setArrows(score_->getArrows() - 1);
}


void PlayState::cleanMemory()
{
	auto it = gObjectsToErase_.begin();
	while (it != gObjectsToErase_.end())
	{
		delete (**it);
		gObjects_.erase(*it);
		it++;
	}
	gObjectsToErase_.clear();


}


void PlayState::deleteGameObject(std::list<GameObject*>::iterator go)
{
	gObjectsToErase_.emplace_back(go);
}

void PlayState::deleteArrow(std::list<GameObject*>::iterator it)
{
	arrows_.remove(static_cast<Arrow*>((*it)));
	int p = pow((static_cast<Arrow*>(*it))->getNumHits() - 1,2)* POINTS_PER_BALLON ;
	score_->setPoints(score_->getPoints() + p);
	deleteGameObject(it);

}

void PlayState::deleteBalloon(std::list<GameObject*>::iterator it)
{
	balloons_.remove(static_cast<Balloon*>((*it)));
	deleteGameObject(it);
}

void PlayState::deleteButterfly(std::list<GameObject*>::iterator it)
{
	butterflies_.remove(static_cast<Butterfly*>((*it)));
	deleteGameObject(it);
}

void PlayState::deleteReward(std::list<GameObject*>::iterator it)
{
	//rewards_.remove(static_cast<Reward*>((*it)));
	deleteGameObject(it);
}
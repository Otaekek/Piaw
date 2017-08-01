#include <piawMissile.class.hpp>
#include <piawLineEntity.class.hpp>
#include <SFML/Audio.hpp>

class piawPlayerSpaceShip: public piawLineEntity {
public:
	piawPlayerSpaceShip();
	piawPlayerSpaceShip(float camDist);
	~piawPlayerSpaceShip();

	void update();

private:

	float	_camDist;
	void	generate_particle(glm::vec3 dir, t_transform *t);
	void 	key_update();
	void	speedUpdate();
	float	upSpeed;
	sf::SoundBuffer missileSoundBuffer;
	sf::Sound		missileSound;
	float	missileTimer[4];
	float	rightSpeed;
};
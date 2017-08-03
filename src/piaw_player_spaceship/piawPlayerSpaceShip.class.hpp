#include <piawMissile.class.hpp>
#include <piawLineEntity.class.hpp>
#include <SFML/Audio.hpp>

class piawPlayerSpaceShip: public piawLineEntity {
public:
	piawPlayerSpaceShip();
	piawPlayerSpaceShip(float camDist);
	~piawPlayerSpaceShip();

	void update();

	sf::Sound		missileSound;
private:

	float	_camDist;
	void	generate_particle(glm::vec3 dir, t_transform *t);
	void 	key_update();
	void	speedUpdate();
	float	upSpeed;
	float	rot;
	sf::SoundBuffer missileSoundBuffer;
	float	missileTimer[4];
	float	rightSpeed;
};
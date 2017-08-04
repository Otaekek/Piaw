#include <piawMissile.class.hpp>
#include <piawLineEntity.class.hpp>
#include <SFML/Audio.hpp>
#include <staticMemoryManager.class.hpp>

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
	float	rot;

	float	missileTimer[4];
	float	rightSpeed;
	uint32_t	soundHandler;
};
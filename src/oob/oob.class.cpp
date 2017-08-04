#include "oob.class.hpp"

uint32_t obb::assetHandler = 0;

void obb::init() {
	assetHandler = fileLoader::load_fs_asset_sync("assets/graphic/mesh/cube/oob.obj", E_3D);
}

obb::obb() {

}
obb::~obb() {
}

void obb::init_render() {
	renderGoHandler = renderBuiltIn::create();
	transformHandler = transformBuiltin::create();
	renderBuiltIn::get_renderGO(renderGoHandler)->transformHandler = transformHandler;
	renderBuiltIn::get_renderGO(renderGoHandler)->assetHandler = assetHandler;
}

void obb::shutdown_render() {
	renderBuiltIn::destroy(renderGoHandler);
	transformBuiltin::destroy(transformHandler);
}

bool		obb::intersect(obb &obb) {
//	if (glm::distance(obb.pos, pos) > obb.maxSphere().w + maxSphere().w)
//		return false;
//	if (glm::distance(obb.pos, pos) < obb.minSphere().w + minSphere().w)
//		return true;
	return s_a_t(obb);
}

glm::vec4	obb::minSphere() {
	return glm::vec4(pos, fmin(r.x, fmin(r.y, r.z)));
}

glm::vec4	obb::maxSphere() {
	return glm::vec4(pos, r.length());
}

void obb::set_transform() {
	//transformBuiltin::LookAtObject(-glm::cross(axis[0], axis[2]));
	transformBuiltin::get_transform(transformHandler)->rotation = glm::lookAt(glm::vec3(0,0,0), axis[2], axis[1]);
	//glm::lookAt(position, lookAtPt, up);
	transformBuiltin::get_transform(transformHandler)->position = pos;
	transformBuiltin::scale(transformHandler, r[0] * 2, r[1] * 2, r[2] * 2);
//	transformBuiltin::translate(transformHandler, -r[0] * 2, -r[1] * 2, -r[2] * 2);
}

uint32_t	obb::render() {
	return 0;
	set_transform();
	renderBuiltIn::get_renderGO(renderGoHandler)->assetHandler = assetHandler;
	renderBuiltIn::render_me(renderGoHandler);
}

void		obb::from_quat(glm::quat b) {

	glm::mat3 m = glm::mat3(1.0f);

	m = glm::mat3_cast(b);
	axis[0].x = m[0][0];
	axis[0].y = m[1][0];
	axis[0].z = m[2][0];
	axis[1].x = m[0][1];
	axis[1].y = m[1][1];
	axis[1].z = m[2][1];
	axis[2].x = m[0][2];
	axis[2].y = m[1][2];
	axis[2].z = m[2][2];
}

bool		obb::s_a_t(obb &b) {

	float R[3][3];
	float fabsR[3][3];

	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			R[i][j] = glm::dot(axis[i], b.axis[j]);

	glm::vec3 t = b.pos - pos;

	t = glm::vec3(glm::dot(t, axis[0]), glm::dot(t, axis[1]), glm::dot(t, axis[2]));

	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			fabsR[i][j] = fabs(R[i][j]) + 0.0001;

	for(int i = 0; i < 3; ++i)
	{
		float ra = r[i];
		float rb = glm::dot(b.r, glm::vec3(fabsR[i][0], fabsR[i][1], fabsR[i][2]));
		if (fabs(t[i]) > ra + rb)
			return false;
	}
	for(int i = 0; i < 3; ++i)
	{
		float ra = r[0] * fabsR[0][i] + r[1] * fabsR[1][i] + r[2] * fabsR[2][i];
		float rb = b.r[i];
		if (fabs(t.x * R[0][i] + t.y * R[1][i] + t.z * R[2][i]) > ra + rb)
			return false;
	}
	float ra = r.y * fabsR[2][0] + r.z * fabsR[1][0];
	float rb = b.r.y * fabsR[0][2] + b.r.z * fabsR[0][1];
	if (fabs(t.z * R[1][0] - t.y * R[2][0]) > ra + rb)
		return false;
	ra = r.y * fabsR[2][1] + r.z * fabsR[1][1];
	rb = b.r.x * fabsR[0][2] + b.r.z * fabsR[0][0];
	if (fabs(t.z * R[1][1] - t.y * R[2][1]) > ra + rb)
		return false;
	ra = r.y * fabsR[2][2] + r.z * fabsR[1][2];
	rb = b.r.x * fabsR[0][1] + b.r.y * fabsR[0][0];
	if (fabs(t.z * R[1][2] - t.y * R[2][2]) > ra + rb)
		return false;
	ra = r.x * fabsR[2][0] + r.z * fabsR[0][0];
	rb = b.r.y * fabsR[1][2] + b.r.z * fabsR[1][1];
	if (fabs(t.x * R[2][0] - t.z * R[0][0]) > ra + rb)
		return false;
	ra = r.x * fabsR[2][1] + r.z * fabsR[0][1];
	rb = b.r.x * fabsR[1][2] + b.r.z * fabsR[1][0];
	if (fabs(t.x * R[2][1] - t.z * R[0][1]) > ra + rb)
		return false;
	ra = r.x * fabsR[2][2] + r.z * fabsR[0][2];
	rb = b.r.x * fabsR[1][1] + b.r.y * fabsR[1][0];
	if (fabs(t.x * R[2][2] - t.z * R[0][2]) > ra + rb)
		return false;
	ra = r.x * fabsR[1][0] + r.y * fabsR[0][0];
	rb = b.r.y * fabsR[2][2] + b.r.z * fabsR[2][1];
	if (fabs(t.y * R[0][0] - t.x * R[1][0]) > ra + rb)
		return false;
	ra = r.x * fabsR[1][1] + r.y * fabsR[0][1];
	rb = b.r.x * fabsR[2][2] + b.r.z * fabsR[2][0];
	if (fabs(t.y * R[0][1] - t.x * R[1][1]) > ra + rb)
		return false;
	ra = r.x * fabsR[1][2] + r.y * fabsR[0][2];
	rb = b.r.x * fabsR[2][1] + b.r.y * fabsR[2][0];
	if (fabs(t.y * R[0][2] - t.x * R[1][2]) > ra + rb)
		return false;
	return true;
}


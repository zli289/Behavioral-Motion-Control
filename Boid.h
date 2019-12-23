#ifndef __GWU_BOID__
#define __GWU_BOID__

class boid
{
public:
	vec3 velocity, position;
	GLfloat matrix[16]={0};

public:
	boid();
	boid(vec3 position);
	void updateMatrix();
};

boid::boid()
{
	matrix[0]=1.0f;
	matrix[5]=1.0f;
	matrix[10]=1.0f;
	matrix[15]=1.0f;
}

boid::boid(vec3 position)
{
	matrix[0]=1.0f;
	matrix[5]=1.0f;
	matrix[10]=1.0f;
	matrix[15]=1.0f;
	this->position=position;

	matrix[12]=position[0];
	matrix[13]=position[1];
	matrix[14]=position[2];
}

void boid::updateMatrix()
{
	matrix[12]=position[0];
	matrix[13]=position[1];
	matrix[14]=position[2];
}
#endif // __GWU_BOID____
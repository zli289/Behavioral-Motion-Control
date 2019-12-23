#ifndef __GWU_QUAN__
#define __GWU_QUAN__

class quanternion {
public :
	GLfloat w;
	vec3 v;
	GLfloat matrix[16]={0};
	vec3 pos;
public :
	quanternion();
	quanternion( GLfloat w, vec3 v);

	quanternion operator*(const quanternion &q);
	quanternion operator*(const GLfloat a);
	quanternion operator+(const quanternion &q);
	GLfloat dot(quanternion &q);
	void updatematrix(vec3 pos);
};

quanternion::quanternion()
{
	w=0.0;
	v=vec3(0.0,0.0,0.0);
}

quanternion::quanternion(GLfloat angle, vec3 v)
{
	this->w=angle;
	this->v=v;
}

quanternion quanternion::operator*(const quanternion &q)
{
	quanternion res;
	res.w=w*q.w-v.dot(q.v);
	res.v=q.v*w+v*q.w+v.crossproduct(q.v);
	return res;
}

quanternion quanternion::operator*(const GLfloat a)
{
	quanternion res;
	res.w=w*a;
	res.v=v*a;
	return res;
}

quanternion quanternion::operator+(const quanternion &q)
{
	quanternion  res;
	res.w=w+q.w;
	res.v.x=v.x+q.v.x;
	res.v.y=v.y+q.v.y;
	res.v.z=v.z+q.v.z;
	return res;
}

GLfloat quanternion::dot(quanternion &q)
{
	GLfloat res=0;
	res+=w*q.w;
	res+=v.x*q.v.x;
	res+=v.y*q.v.y;
	res+=v.z*q.v.z;
	return res;
}

void quanternion::updatematrix(vec3 pos)
{
	GLfloat x=v.x;
	GLfloat y=v.y;
	GLfloat z=v.z;
	GLfloat temp[16]=
	{1-2*y*y-2*z*z, 2*x*y-2*w*z, 2*x*z+2*w*y, 0.0f,
	2*x*y+2*w*z, 1-2*x*x-2*z*z, 2*y*z-2*w*x, 0.0f,
	2*x*z-2*w*y, 2*y*z+2*w*x, 1-2*x*x-2*y*y, 0.0f,
	pos.x, pos.y, pos.z, 1.0f};
	for(int i=0;i<16;i++)
			matrix[i]=temp[i];

	this->pos=pos;
}

#endif // __GWU_QUAN____